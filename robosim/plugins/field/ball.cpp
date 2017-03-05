#include <robosim/ext/parts.h>
#include "ball.h"

Ball::Ball(dWorldID world, dSpaceID space, Tag tag, int number, float radius, float mass_)
    :
    tag_(tag),
    number_(number),
    world_(world),
    space_(space)
{
    dBodyID sphere_body = dBodyCreate(world);
    dMass m;
    m.setSphere(1.0f,radius);
    m.adjust(mass_);
    dBodySetMass(sphere_body,&m);

    sphere_ = CreateGenericPart(this,sphere_body);

    dGeomID geom = dCreateSphere(space,radius);
    GPAddGeom(sphere_,geom);
    sphere_->setColor(1,1,0,1);
}
Ball::~Ball()
{
    DestroyGenericPart(sphere_);
}

void Ball::getPosition(float &x,float &y,float &z) const
{
    const dReal* p = dBodyGetPosition(sphere_->getBody());
    x = p[0];
    y = p[1];
    z = p[2];
}

void Ball::setPosition(float x,float y,float z)
{
    dBodySetPosition(sphere_->getBody(),x,y,z);
}

void Ball::getQuaternion(float &w,float &x,float &y,float &z) const
{
    const dReal* q = dBodyGetQuaternion(sphere_->getBody());
    w = q[0];
    x = q[1];
    y = q[2];
    z = q[3];
}

// 对于球体,忽略转动量
void Ball::setQuaternion(float,float,float,float)
{
}

void Ball::render(IDrawing* draw) const
{
    sphere_->render(draw);
}
