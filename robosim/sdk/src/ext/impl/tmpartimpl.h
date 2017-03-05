#pragma once

#include <robosim/base.h>
#include <robosim/ext.h>


class IEntity;

class TrimeshPart : public IPart
{
    TrimeshPart(const TrimeshPart&);
    TrimeshPart& operator=(const TrimeshPart&);
public:
    TrimeshPart(IEntity* owner,
        const Real* vertices, size_t vertices_size,
        const unsigned int* indices, size_t indices_size,
        const Real* normals,
		Real mass);
    virtual ~TrimeshPart();

    IEntity* getOwner();
    dBodyID getBody();

    void setSurface(const dSurfaceParameters& sp);
    void setMu2Dir(float x, float y, float z);
    void setColor(float r, float g, float b, float a);
    const dSurfaceParameters* getSurface() const;
    const float* getMu2Dir() const;

    void  render(IDrawing *draw) const;

private:
    IPart* gpart_;

    dTriMeshDataID trimesh_data_;
    dGeomID trimesh_geom_;

    const dReal* vertices_;
    const dTriIndex* indices_;
    size_t index_count_;

	float color_[4];
};

inline
IEntity* TrimeshPart::getOwner()
{
    return gpart_->getOwner();
}

inline
dBodyID TrimeshPart::getBody()
{
    return gpart_->getBody();
}
inline
void TrimeshPart::setSurface(const dSurfaceParameters& sp)
{
    gpart_->setSurface(sp);
}
inline
void TrimeshPart::setMu2Dir(float x, float y, float z)
{
    gpart_->setMu2Dir(x,y,z);
}
inline
void TrimeshPart::setColor(float r, float g, float b, float a)
{
    color_[0]=r;
	color_[1]=g;
	color_[2]=b;
	color_[3]=a;
}
inline
const dSurfaceParameters* TrimeshPart::getSurface() const
{
    return gpart_->getSurface();
}
inline
const float* TrimeshPart::getMu2Dir() const
{
    return gpart_->getMu2Dir();
}


