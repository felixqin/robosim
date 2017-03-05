#include "robosim/base/ientity.h"
#include "robosim/base/idrawing.h"
#include "gpartimpl.h"

static const dSurfaceParameters SP_NULL =
{
    0,
    dInfinity

};

typedef std::vector<dGeomID> GeomTable;

GenericPart::GenericPart( IEntity* owner, dBodyID body )
    :
    owner_(owner),
    body_(body),
    surface_(SP_NULL)
{
}

GenericPart::~GenericPart()
{
    GeomTable::iterator it = geoms_.begin();
    for ( ; it != geoms_.end(); it++ )
    {
        dGeomDestroy(*it);
    }
    if (body_) dBodyDestroy(body_);
}
void GenericPart::render(IDrawing* draw) const
{
    draw->setColor(color_[0],color_[1],color_[2],color_[3]);

    GeomTable::const_iterator it = geoms_.begin();
    for ( ; it != geoms_.end(); it++ )
    {
        const dReal* p = dGeomGetPosition(*it);
        dReal q[4]; dGeomGetQuaternion(*it,q);

        switch ( dGeomGetClass(*it) )
        {
        case dSphereClass:
            {
                dReal radius = dGeomSphereGetRadius(*it);
                draw->pushMatrix();
                draw->transform(p[0],p[1],p[2],q[0],q[1],q[2],q[3]);
                draw->drawSphere(radius);
                draw->popMatrix();
            }
            break;
        case dBoxClass:
            {
                dVector3 length;
                dGeomBoxGetLengths(*it,length);
                draw->pushMatrix();
                draw->transform(p[0],p[1],p[2],q[0],q[1],q[2],q[3]);
                draw->drawBox(length[0],length[1],length[2]);
                draw->popMatrix();
            }
            break;
        case dCylinderClass:
            {
                dReal r,l;
                dGeomCylinderGetParams(*it,&r,&l);
                draw->pushMatrix();
                draw->transform(p[0],p[1],p[2],q[0],q[1],q[2],q[3]);
                draw->drawCylinder(r,l);
                draw->popMatrix();
            }
            break;
        case dCapsuleClass:
            {
                dReal r,l;
                dGeomCapsuleGetParams(*it,&r,&l);
                draw->pushMatrix();
                draw->transform(p[0],p[1],p[2],q[0],q[1],q[2],q[3]);
                draw->drawCylinder(r,l);
                draw->popMatrix();
            }
            break;
        }
    }
}
