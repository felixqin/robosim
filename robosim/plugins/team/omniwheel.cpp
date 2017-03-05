#include "omniwheel.h"

static const dSurfaceParameters OMNI_WHEEL_SP =
{
    dContactMu2 | dContactFDir1,
    dInfinity,
    0
};
static const float OMNI_WHEEL_MU2DIR [] = { 0, 0, 1, 0 };


OmniWheel::OmniWheel(IEntity* owner, float radius, float length, float mass)
{
    dBodyID wheel_body = dBodyCreate(owner->getWorld());
    dMass m;
    //m.setCylinder(1.0f,3,radius,length);
    //m.adjust(mass);
    dMassSetCylinderTotal(&m,mass,3,radius,length);
    dBodySetMass(wheel_body,&m);

    wheel_ = CreateGenericPart(owner,wheel_body);

    dGeomID cylinder = dCreateCylinder(owner->getInnerSpace(),radius,length);
    GPAddGeom(wheel_,cylinder);

    wheel_->setSurface(OMNI_WHEEL_SP);
    wheel_->setMu2Dir(OMNI_WHEEL_MU2DIR[0],OMNI_WHEEL_MU2DIR[1],OMNI_WHEEL_MU2DIR[2]);
}

OmniWheel::~OmniWheel()
{
    delete wheel_;
}
