#ifndef PLANE_H__
#define PLANE_H__

#include <string>
#include <robosim/robosim.h>
#include "config.hpp"


class Plane : public IEntity
{
public:
    Plane(dWorldID world, dSpaceID space, Tag tag, int number, FieldParam const& fp);
    virtual ~Plane();

    Tag group() const { return tag_; }
    int number() const { return number_; }

    dWorldID getWorld() { return world_; }
    dSpaceID getOuterSpace() { return space_; }
    dSpaceID getInnerSpace() { return space_; }

    const IMessageHandler* getMsgHandler() const { return 0; }

    void getPosition(float &x,float &y,float &z) const;
    void setPosition(float x,float y,float z);
    void getQuaternion(float &w,float &x,float &y,float &z) const;
    void setQuaternion(float w, float x, float y, float z);

    void render(IDrawing*) const;

    void setGroundTexture(int tex_id) { tex_id_ = tex_id; }

protected:
    void create_bar(dSpaceID space, BarParam const& bp);
    void create_door(dSpaceID space, FieldParam const& dp);

private:
    dWorldID world_;
    dSpaceID space_;

    Tag tag_;
    int number_;

    float lx_,ly_;

    IPart* ground_;
    IPart* bar_;
    IPart* door1_;
    IPart* door2_;

    int tex_id_;
};

#endif

