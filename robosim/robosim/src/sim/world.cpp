#include <cstring>
#include <cassert>
#include <algorithm>
#include <functional>
#include <robosim/base/ientity.h>
#include <robosim/base/ipart.h>
#include "sim/world.h"
#include "app/configure.h"

using namespace std;


/******************************************************************************/

void nearCallback(void* data, dGeomID o1, dGeomID o2)
{
    if ( dGeomIsSpace(o1) || dGeomIsSpace(o2) )
    {
        dSpaceCollide2(o1,o2,data,&nearCallback);
        if (dGeomIsSpace(o1))
        {
            dSpaceCollide(reinterpret_cast<dSpaceID>(o1),data,&nearCallback);
        }
        if (dGeomIsSpace(o2))
        {
            dSpaceCollide(reinterpret_cast<dSpaceID>(o2),data,&nearCallback);
        }
    }
    else
    {
        World* world = static_cast<World*>(data);
        assert(world);
        world->onCollide(o1,o2);
    }
}

/******************************************************************************/

World::World(const ModelParameters* mp)
    :
    mp_(mp),
    time_(0),
    numc_(0)
{
    dInitODE();

    world_ = dWorldCreate();
    space_ = dSimpleSpaceCreate(0);
    contactgroup_ = dJointGroupCreate(0);

    dWorldSetGravity(world_,0,0,-mp_->gravity);
    dWorldSetCFM(world_,mp_->cfm);
    dWorldSetERP(world_,mp_->erp);

    contacts_ = new dContact[mp_->max_contacts];
}

World::~World()
{
    dJointGroupDestroy(contactgroup_);
    dSpaceDestroy(space_);
    dWorldDestroy(world_);
    dCloseODE();

    delete contacts_;
}

dWorldID World::getWorld()
{
    return world_;
}

dSpaceID World::getSpace()
{
    return space_;
}

double World::getTime() const
{
    return time_;
}

void World::render(IDrawing* drawing) const
{
//    EntityTable::const_iterator iter = entities_.begin();
//    for ( ; iter != entities_.end(); iter++ )
//    {
//        (*iter)->render(drawing);
//    }

    using std::tr1::bind;
    using std::tr1::placeholders::_1;

    for_each(entities_.begin(), entities_.end(), bind(&IEntity::render,_1,drawing));
}

void World::step(double delta)
{
    if (delta<=0) return;
    if (delta > mp_->max_step_time) delta = mp_->max_step_time;

    dSpaceCollide(space_,this,&nearCallback);
    dWorldQuickStep(world_,(dReal)delta);
    dJointGroupEmpty(contactgroup_);
    time_ += delta;
}

bool World::addEntity(IEntity* entity)
{
    entities_.push_back(entity);

    return true;
}
/*
IEntity* World::getEntity(const Tag tag, const int number)
{
    using namespace std;

    EntityTable::iterator it = entities_.begin();
    for ( ; it != entities_.end(); ++it )
    {
        if ( (*it)->tag() == tag && (*it)->number() == number )
        {
            return *it;
        }
    }
    return 0;
}
*/
/*
void World::foreachEntity(ErgodFuncPtr fn, void* data) const
{
    using std::tr1::bind;
    using std::tr1::placeholders::_1;

    for_each(entities_.begin(),entities_.end(),bind(fn,data,_1));
}
*/
void World::onCollide(dGeomID o1, dGeomID o2)
{
#if 0
    DEBUGLOG("World::onCollide()");
#endif
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);

    if (b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)) return;

    int skip = sizeof(dContact);

    numc_ = dCollide(o1, o2, mp_->max_contacts, &contacts_[0].geom, skip);
    if (numc_)
    {
        IPart* p1 = IPart::getPart(o1);
        IPart* p2 = IPart::getPart(o2);

        correctContact(p1,p2);

        for ( int i=0; i<numc_; ++i )
        {
            dJointID c = dJointCreateContact(world_, contactgroup_, &contacts_[i]);
            dJointAttach(c, b1, b2);
        }
    }
}

void World::correctContact(IPart* p1, IPart* p2)
{
    dSurfaceParameters surface;
    surface.mode = dContactBounce | dContactSoftCFM;
    surface.mu = 0;
    surface.mu2 = 0;
    surface.bounce = mp_->bounce;
    surface.bounce_vel = mp_->bounce_vel;
    surface.soft_cfm = mp_->cfm;

    const dSurfaceParameters *s1=0, *s2=0;
    if (p1) s1 = p1->getSurface();
    if (p2) s2 = p2->getSurface();

    calcSurface(&surface,s1,s2);

#if 0
    Log::instance()->out()<<"surface.mode:"<<surface.mode<<std::endl;
#endif

    if ( surface.mode & ( dContactFDir1 ) )
    {
        for ( int i=0; i<numc_; ++i )
        {
            contacts_[i].surface = surface;
            if (!calcFDir1(contacts_[i].fdir1,contacts_[i].geom.normal,p1,p2))
            {
                contacts_[i].surface.mode |= ~dContactFDir1;
            }
        }
    }
    else
    {
        for ( int i=0; i<numc_; ++i )
        {
            contacts_[i].surface = surface;
        }
    }
}

void World::calcSurface(dSurfaceParameters* r, const dSurfaceParameters* s1, const dSurfaceParameters* s2)
{
    using namespace std;
    if (s1&&s2)
    {
        if ( s1->mode && s2->mode )
        {
            r->mode        = s1->mode | s2->mode;
            r->mu2         = max(s1->mu2,s2->mu2);
            r->bounce      = min(s1->bounce,s2->bounce);
            r->bounce_vel  = min(s1->bounce,s2->bounce_vel);
            r->soft_erp    = min(s1->soft_erp,s2->soft_erp);
            r->soft_cfm    = min(s1->soft_cfm,s2->soft_cfm);
            r->motion1     = min(s1->motion1,s2->motion1);
            r->motion2     = min(s1->motion2,s2->motion2);
            r->slip1       = max(s1->slip1,s2->slip1);
            r->slip2       = max(s1->slip2,s2->slip2);
        }
        else if ( s1->mode )
        {
            *r = *s1;
        }
        else if ( s2->mode )
        {
            *r = *s2;
        }
        r->mu = max(s1->mu,s2->mu);
    }
    else if (s1)
    {
        *r = *s1;
    }
    else if (s2)
    {
        *r = *s2;
    }
}

bool World::calcFDir1(dVector3 fdir1, const dVector3 normal, IPart* p1, IPart* p2)
{
    const float* mu2dir = 0;
    const dReal* R = 0;
    if ( p1 )
    {
        mu2dir = p1->getMu2Dir();
        R = dBodyGetRotation(p1->getBody());
    }
    if ( p2 && mu2dir == 0 )
    {
        mu2dir = p2->getMu2Dir();
        R = dBodyGetRotation(p1->getBody());
    }
    if (mu2dir)
    {
        dVector3 loc_dir2,fdir2;
        loc_dir2[0] = mu2dir[0];
        loc_dir2[1] = mu2dir[1];
        loc_dir2[2] = mu2dir[2];
        dMULTIPLY0_331(fdir2,R,loc_dir2);
        dCROSS(fdir1,=,fdir2,normal);
        return true;
    }
    return false;
}
