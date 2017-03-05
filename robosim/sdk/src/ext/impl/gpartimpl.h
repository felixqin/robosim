#ifndef GENERIC_PART_H__
#define GENERIC_PART_H__

#include <vector>
#include <ode/ode.h>

#include "robosim/base/ipart.h"

class IEntity;
class IDrawing;

class IPart;

class GenericPart : public IPart
{
    GenericPart(const GenericPart&);
    GenericPart& operator=(const GenericPart&);
public:
    GenericPart(IEntity* owner, dBodyID body);
    virtual ~GenericPart();

    IEntity* getOwner();
    dBodyID getBody();

    void addGeom(dGeomID geom);

    void setSurface(const dSurfaceParameters&);
    void setMu2Dir(float x, float y, float z);
    void setColor(float r, float g, float b, float a);
    const dSurfaceParameters* getSurface() const;
    const float* getMu2Dir() const;

    void render(IDrawing *draw) const;

protected:
    IEntity*    owner_;
    dBodyID     body_;
    std::vector<dGeomID> geoms_;

    dSurfaceParameters surface_;
    float mu2dir_[3];
    float color_[4];
};

/******************************************************************************/


inline
IEntity* GenericPart::getOwner()
{
    return owner_;
}
inline
dBodyID GenericPart::getBody()
{
    return body_;
}
inline
void GenericPart::addGeom(dGeomID geom)
{
    dGeomSetBody(geom,body_);
    setPart(geom,this);
    geoms_.push_back(geom);
}
inline
void GenericPart::setSurface(const dSurfaceParameters& surface)
{
    surface_ = surface;
}
inline
void GenericPart::setMu2Dir(float x, float y, float z)
{
    mu2dir_[0] = x;
    mu2dir_[1] = y;
    mu2dir_[2] = z;
}
inline
void GenericPart::setColor(float r, float g, float b, float a)
{
    color_[0] = r;
    color_[1] = g;
    color_[2] = b;
    color_[3] = a;
}
inline
const dSurfaceParameters* GenericPart::getSurface() const
{
    return &surface_;
}
inline
const float* GenericPart::getMu2Dir() const
{
    return mu2dir_;
}


#endif //GENERIC_PART_H__
