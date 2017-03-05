#ifndef IPART_H__
#define IPART_H__

#include <ode/ode.h>
#include "../precomp.h"

class IDrawing;
class IEntity;

class IPart
{
public:
    virtual ~IPart() {}

    virtual IEntity* getOwner() = 0;

    virtual dBodyID getBody() = 0;

    //virtual void enable(bool enable) = 0;
    //virtual bool enabled() = 0;

    virtual void setSurface(const dSurfaceParameters&) = 0;
    virtual void setMu2Dir(float x, float y, float z) = 0;
    virtual void setColor(float r, float g, float b, float a) = 0;
    virtual const dSurfaceParameters* getSurface() const = 0;
    virtual const float* getMu2Dir() const = 0;

    virtual void  render(IDrawing *draw) const = 0;

public:
    static IPart* getPart(dGeomID geom);
    static void  setPart(dGeomID geom, IPart* part);
};


inline IPart* IPart::getPart(dGeomID geom)
{
    return reinterpret_cast<IPart*>(dGeomGetData(geom));
}

inline void IPart::setPart(dGeomID geom, IPart* part)
{
    dGeomSetData(geom,part);
}


#endif
