#ifndef OMNIWHEEL_H__
#define OMNIWHEEL_H__

#include <robosim/base.h>
#include <robosim/ext.h>

class IEntity;

class OmniWheel : public IPart
{
public:
    OmniWheel(IEntity* owner, float radius, float length, float mass);
    virtual ~OmniWheel();

    IEntity* getOwner();

    dBodyID getBody();

    void setSurface(const dSurfaceParameters& sp);
    void setMu2Dir(float x, float y, float z);
    void setColor(float r, float g, float b, float a);
    const dSurfaceParameters* getSurface() const;
    const float* getMu2Dir() const;

    void  render(IDrawing *draw) const;

private:
    IPart* wheel_;
};

inline
IEntity* OmniWheel::getOwner()
{
    return wheel_->getOwner();
}

inline
dBodyID OmniWheel::getBody()
{
    return wheel_->getBody();
}
inline
void OmniWheel::setSurface(const dSurfaceParameters& sp)
{
    wheel_->setSurface(sp);
}
inline
void OmniWheel::setMu2Dir(float x, float y, float z)
{
    wheel_->setMu2Dir(x,y,z);
}
inline
void OmniWheel::setColor(float r, float g, float b, float a)
{
    wheel_->setColor(r,g,b,a);
}
inline
const dSurfaceParameters* OmniWheel::getSurface() const
{
    return wheel_->getSurface();
}
inline
const float* OmniWheel::getMu2Dir() const
{
    return wheel_->getMu2Dir();
}
inline
void OmniWheel::render(IDrawing *draw) const
{
    wheel_->render(draw);
}

#endif
