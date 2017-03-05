#ifndef WORLD_H__
#define WORLD_H__

#include <ode/ode.h>
#include <vector>
#include <robosim/base/iworld.h>


class IEntity;
class IPart;
class ModelParameters;

typedef std::vector<IEntity*> EntityTable;


class World : public IWorld
{
public:
    World(const ModelParameters* mp);
    ~World();

    void render(IDrawing* drawing) const;
    void step(double delta);

    dWorldID getWorld();
    dSpaceID getSpace();
    double getTime() const;

    bool addEntity(IEntity*);
    //TODO: removeEntity
    //bool removeEntity(int id);
    //bool removeEntity(Entity*);

    //Entity* getEntity(int id);
    //IEntity* getEntity(const Tag tag, const int number);

    const EntityTable& getEntityTable() const { return entities_; }
    //void foreachEntity(FuncPtr fn, void* data) const;

protected:
    friend void nearCallback(void* data, dGeomID o1, dGeomID o2);
    void onCollide(dGeomID o1, dGeomID o2);

    void correctContact(IPart* p1, IPart* p2);
    void calcSurface(dSurfaceParameters* r, const dSurfaceParameters* s1, const dSurfaceParameters* s2);
    bool calcFDir1(dVector3 fdir1, const dVector3 normal, IPart* p1, IPart* p2);

private:
    const ModelParameters* mp_;

    double time_;

    dWorldID world_;
    dSpaceID space_;

    dJointGroupID contactgroup_;
    dContact* contacts_;
    int numc_;

    EntityTable entities_;
};


#endif //WORLD_H__
