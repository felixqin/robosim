#include <stdexcept>
#include <robosim/util/debug.h>
#include "tmpartimpl.h"

TrimeshPart::TrimeshPart(IEntity* owner,
        const Real* vertices, size_t vertices_size,
        const unsigned int* indices, size_t indices_size,
        const Real* normals,
		Real mass)
    :
    vertices_(vertices),
    indices_(indices),
	index_count_(indices_size)
{
	if ( sizeof(dReal) != sizeof(float) && sizeof(dTriIndex) != sizeof(unsigned int) )
	{
		throw std::runtime_error("dReal and dTriIndex must be equivalent to float and uint!");
	}

    dBodyID body = dBodyCreate(owner->getWorld());
    gpart_ = CreateGenericPart(owner,body);

    trimesh_data_ = dGeomTriMeshDataCreate();
    dGeomTriMeshDataBuildSingle1(trimesh_data_,
        vertices,
		3*sizeof(vertices[0]),
        vertices_size/3,
        indices,
        index_count_,
		3*sizeof(indices[0]),
		normals
    );
    trimesh_geom_ = dCreateTriMesh(0,trimesh_data_,0,0,0);

	//设置质量
    dMass m;
    //m.setTrimesh(1.0f,trimesh_geom_);
    //m.adjust(mass);
    dMassSetTrimeshTotal(&m,mass,trimesh_geom_);
	float mc[3];
	std::copy(&m.c[0],&m.c[3],mc);
	TRACE("mass center: %f,%f,%f\n",mc[0],mc[1],mc[2]);
    dGeomSetPosition(trimesh_geom_, -mc[0], -mc[1], -mc[2]);
	m.translate(-mc[0], -mc[1], -mc[2]);
    dBodySetMass(body,&m);

	//用TransformGeom将网格几何体包含进去
	dGeomID t_geom = dCreateGeomTransform(owner->getInnerSpace());
	dGeomTransformSetGeom(t_geom,trimesh_geom_);
	dGeomTransformSetCleanup(t_geom,1);
	dGeomTransformSetInfo(t_geom,1);

	GPAddGeom(gpart_,t_geom);

	//质点移动原质心位置,这样网格的位置与网格数据定义相同
	dBodySetPosition(body,mc[0],mc[1],mc[2]);

#ifdef _DEBUG
	const dReal* p = dBodyGetPosition(gpart_->getBody());
	TRACE("body position: %f,%f,%f\n",p[0],p[1],p[2]);
#endif
}

TrimeshPart::~TrimeshPart()
{
    dGeomTriMeshClearTCCache(trimesh_geom_);
    dGeomTriMeshDataDestroy(trimesh_data_);
    DestroyGenericPart(gpart_);
}

void TrimeshPart::render(IDrawing* draw) const
{
    draw->setColor(color_[0],color_[1],color_[2],color_[3]);

	const dReal* p = dBodyGetPosition(gpart_->getBody());
	const dReal* q = dBodyGetQuaternion(gpart_->getBody());
	const dReal* p2 = dGeomGetPosition(trimesh_geom_);

    draw->pushMatrix();
    draw->transform(p[0],p[1],p[2],q[0],q[1],q[2],q[3]);
	draw->transform(p2[0],p2[1],p2[2],1,0,0,0);
    draw->drawTrimesh(vertices_,indices_,index_count_);
    draw->popMatrix();
}
