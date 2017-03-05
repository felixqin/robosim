#include <cmath>
#include <cassert>
#include <algorithm>
#include <functional>
#include <robosim/util.h>
#include "robot.h"
#include "omniwheel.h"


//static const Real PI = 3.1415926535897932384626;

/***************************************************************************/

Robot::Robot(Tag tag, int number, dWorldID world, dSpaceID space, const RobotParam &rp)
    :
    tag_(tag),
    number_(number),
    world_(world),
    outer_space_(space),
    ai_(rp)
{
    inner_space_ = dSimpleSpaceCreate(space);
	joint_group_ = dJointGroupCreate(0);

    create_main_body(rp);
    create_wheels(rp);
    create_dribbler(rp);
}

Robot::~Robot()
{
	dJointGroupDestroy(joint_group_);
    //必须在释放space前,释放所有geom
	parts_.clear();
    dSpaceDestroy(inner_space_);
}

void Robot::create_main_body(const RobotParam &rp)
{
    PartSP part(
        CreateTrimeshPart(this,
            rp.main_body.vertices,
            rp.main_body.vertices_size,
            rp.main_body.indices,
            rp.main_body.indices_size,
            rp.main_body.normals,
            rp.main_body.mass),
        DestroyTrimeshPart
    );
	part->setColor(0.8,0.8,0.99,0);

	parts_.push_back(part);

}

void Robot::create_wheels(const RobotParam &rp)
{
    for (size_t i=0; i<rp.wheels_count; ++i)
    {
        //create wheel
        float radius = rp.wheels[i].cylinder.radius;
        float length = rp.wheels[i].cylinder.length;
        float mass = rp.wheels[i].cylinder.mass;
        PartSP wheel(new OmniWheel(this,radius,length,mass));

        wheel->setColor(0.3,0.2,0.1,1);

        //set rotation and position of wheel
        float sina = std::sin(rp.wheels[i].angle);
        float cosa = std::cos(rp.wheels[i].angle);
        dMatrix3 R;
        dRFrom2Axes(R,0,0,1,sina,-cosa,0);
        dBodySetRotation(wheel->getBody(),R);

        float x = rp.wheels[i].distance * cosa;
        float y = rp.wheels[i].distance * sina;
        float z = rp.wheels[i].cylinder.radius;
        dBodySetPosition(wheel->getBody(),x,y,z);

        //create motor, attach wheel and main body
        dJointID motor = dJointCreateHinge(world_,joint_group_);
        dJointAttach(motor,get_main_body(),wheel->getBody());
        dJointSetHingeAnchor(motor,x,y,z);
        dJointSetHingeAxis(motor,cosa,sina,0);
        dJointSetHingeParam(motor,dParamFMax,rp.wheels[i].motor.fmax);
        TRACE("wheel%d axis: (%f,%f,0)",i,cosa,sina);

        parts_.push_back(wheel);
        wheel_motors_.push_back(motor);
    }
}
void Robot::create_dribbler(const RobotParam& rp)
{
    //TODO create dribbler
    Real radius = rp.dribbler.cylinder.radius;
    Real length = rp.dribbler.cylinder.length;

    dMass m;
    m.setCapsule(1,3,radius,length);
    m.adjust(rp.dribbler.cylinder.mass);

    dBodyID body = dBodyCreate(world_);
    dBodySetMass(body,&m);
    dGeomID geom = dCreateCapsule(getInnerSpace(),radius,length);

    PartSP dribbler(CreateGenericPart(this,body),DestroyGenericPart);
    GPAddGeom(dribbler.get(),geom);
    dribbler->setColor(0,1,1,1);

    //set dribbler position and rotation
    float x = rp.dribbler.distance;
    float y = 0;
    float z = rp.dribbler.height;
    dBodySetPosition(body,x,y,z);

    dQuaternion q;
    dQFromAxisAndAngle(q,1,0,0,-M_PI_2);   //绕x轴旋转-90度
    dBodySetQuaternion(body,q);

    //attach body and motor
    dribbler_motor_ = dJointCreateHinge(world_,joint_group_);
    dJointAttach(dribbler_motor_,get_main_body(),body);
    dJointSetHingeAnchor(dribbler_motor_,x,y,z);
    dJointSetHingeAxis(dribbler_motor_,0,1,0);
    dJointSetHingeParam(dribbler_motor_,dParamFMax,rp.dribbler.motor.fmax);

    parts_.push_back(dribbler);
}

//以main_body_的中心点为机器人中心点
void Robot::getPosition(float& x, float &y, float &z) const
{
    const dReal* p = dBodyGetPosition(get_main_body());
    x = p[0];
    y = p[1];
    z = p[2];
}

void Robot::setPosition(float x, float y, float z)
{
    float px,py,pz;
    getPosition(px,py,pz);
    float dx = x-px;
    float dy = y-py;
    float dz = z-pz;

	for (PartVector::iterator it = parts_.begin(); it != parts_.end(); ++it)
	{
		BodyTranslate((*it)->getBody(),dx,dy,dz);
	}
}

//以main_body_的方位为机器人方位
void Robot::getQuaternion(float &w,float &x,float &y,float &z) const
{
    const dReal* q = dBodyGetQuaternion(get_main_body());
    w = q[0];
    x = q[1];
    y = q[2];
    z = q[3];
}

void Robot::setQuaternion(float w,float x,float y,float z)
{
    float px,py,pz;
    getPosition(px,py,pz);

    float oqw,oqx,oqy,oqz;
    getQuaternion(oqw,oqx,oqy,oqz);

    dQuaternion nq = { w,x,y,z };
    dQuaternion oq = { oqw,oqx,oqy,oqz };

    dQuaternion q;
    dQMultiply2(q,nq,oq);   // q = nq*oq^-1

	for (PartVector::iterator it = parts_.begin(); it != parts_.end(); ++it)
	{
		BodyRotate((*it)->getBody(),px,py,pz,q[0],q[1],q[2],q[3]);
	}
}

void Robot::render(IDrawing* draw) const
{
    draw->setTexture(TEXTURE_NONE);
    //draw->setColor(1,0,0,0);

	std::for_each(parts_.begin(),parts_.end(),
		std::tr1::bind(&IPart::render,std::tr1::placeholders::_1,draw));
}

/******************************************************************************/
// Robot AI

void Robot::handleMessage(MessageID id)
{
    Message const* msg = ToMessage(id);
    if (!msg) return;

    switch (msg->type)
    {
    case MSG_SET_VELOCITY:
        handle_set_velocity(msg);
        break;
    case MSG_SET_DRIBBLER:
        handle_set_dribbler(msg);
        break;
    }
}

void Robot::handle_set_velocity(Message const* msg)
{
    ai_.setWheelVel(&wheel_motors_[0],msg->real[0],msg->real[1],msg->real[2]);
}

void Robot::handle_set_dribbler(Message const* msg)
{
    Real w = msg->integer[0] ? 0.1 : 0;
    dJointSetHingeParam(dribbler_motor_,dParamVel,w);
}

