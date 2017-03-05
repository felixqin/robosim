#ifndef BALL_H__
#define BALL_H__

#include <string>
#include <robosim/base.h>


class Ball : public IEntity
{
public:
    Ball(dWorldID world, dSpaceID space, Tag tag, int number, float radius_=0.043, float mass_=0.046);
    virtual ~Ball();

    Tag group() const { return tag_; }
    int number() const { return number_; }

    const IMessageHandler* getMsgHandler() const { return 0; }

    dWorldID getWorld() { return world_; }
    dSpaceID getOuterSpace() { return space_; }
    dSpaceID getInnerSpace() { return space_; }

    void getPosition(float &x,float &y,float &z) const;
    void setPosition(float x,float y,float z);
    void getQuaternion(float &w,float &x,float &y,float &z) const;
    void setQuaternion(float w, float x, float y, float z);

    void render(IDrawing*) const;

private:
    Tag tag_;
    int number_;

    dWorldID world_;
    dSpaceID space_;

    IPart* sphere_;
};

#endif
