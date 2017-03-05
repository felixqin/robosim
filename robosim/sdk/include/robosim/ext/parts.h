#pragma once

#include <ode/ode.h>
#include "../precomp.h"
#include "../types.h"

class IPart;
class IEntity;

extern "C"
{
    SDK_API IPart* CreateGenericPart(IEntity* owner, dBodyID body);
    SDK_API void DestroyGenericPart(IPart*);
    SDK_API void GPAddGeom(IPart* gp,dGeomID geom);

    // 如ODE的dGeomTriMeshDataBuildSingle()一样,网格数据指针必须保持有效
    SDK_API IPart* CreateTrimeshPart(IEntity* owner,
        const Real* vertices, size_t vertices_size,
        const unsigned int* indices, size_t indices_size,
        const Real* normals,
		Real mass);
    SDK_API void DestroyTrimeshPart(IPart*);
}
