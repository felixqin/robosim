#ifndef IENTITY_H__
#define IENTITY_H__

#include <ode/ode.h>
#include "../precomp.h"
#include "../types.h"

class IDrawing;
class IMessageHandler;

class IEntity
{
public:
    virtual ~IEntity() {}

    virtual Tag group() const = 0;
    virtual int number() const = 0;

    virtual const IMessageHandler* getMsgHandler() const = 0;

    virtual dWorldID getWorld() = 0;
    virtual dSpaceID getOuterSpace() = 0;
    virtual dSpaceID getInnerSpace() = 0;

    virtual void getPosition(float &x,float &y,float &z) const = 0;
    virtual void setPosition(float x,float y,float z) = 0;
    virtual void getQuaternion(float &w,float &x,float &y,float &z) const = 0;
    virtual void setQuaternion(float w,float x,float y,float z) = 0;

    virtual void render(IDrawing*) const = 0;
};

#endif //IENTITY_H__
