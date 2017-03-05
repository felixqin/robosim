#ifndef IWORLD_H__
#define IWORLD_H__

#include <ode/ode.h>
#include "../precomp.h"
#include "../types.h"

class IEntity;
class IDrawing;


typedef void (*ErgodFuncPtr)(void* data, const IEntity*);


class IWorld
{
public:
    virtual ~IWorld() {}

    virtual void render(IDrawing* drawing) const = 0;
    virtual void step(double delta) = 0;

    virtual dWorldID getWorld() = 0;
    virtual dSpaceID getSpace() = 0;
    virtual double getTime() const = 0;

    virtual bool addEntity(IEntity*) = 0;
    //TODO: removeEntity
    //virtual IEntity* getEntity(const Tag tag, const int number) = 0;
    //virtual void ergodEntities(ErgodFuncPtr fn, void* data) const = 0;
};

#endif //IWORLD_H__
