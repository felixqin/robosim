#include "robosim/ext/parts.h"
#include "impl/gpartimpl.h"
#include "impl/tmpartimpl.h"


IPart* CreateGenericPart(IEntity* owner, dBodyID body)
{
    return new GenericPart(owner,body);
}
void DestroyGenericPart(IPart* gp)
{
    GenericPart* gpart = dynamic_cast<GenericPart*>(gp);
    delete gpart;
}

void GPAddGeom(IPart* gp,dGeomID geom)
{
    GenericPart* gpart = dynamic_cast<GenericPart*>(gp);
    if (gpart) gpart->addGeom(geom);
}

/******************************************************************************/


IPart* CreateTrimeshPart(IEntity* owner,
        const Real* vertices, size_t vertices_size,
        const unsigned int* indices, size_t indices_size,
        const Real* normals,
		Real mass)
{
    return new TrimeshPart(owner,
            vertices,vertices_size,
            indices,indices_size,
            normals,
            mass);
}

void DestroyTrimeshPart(IPart* part)
{
    TrimeshPart* tmpart = dynamic_cast<TrimeshPart*>(part);
    delete tmpart;
}
