#ifndef ROBOT_H__
#define ROBOT_H__

#include <string>
#include <vector>
#include <memory>

#include <robosim/base.h>
#include <robosim/ext.h>
#include "robotai.h"

/******************************************************************************/

class TrimeshDataManager;
class OmniWheel;
class Robot : public IEntity, public IMessageHandler
{
public:
    Robot(Tag tag, int number, dWorldID world, dSpaceID space, const RobotParam &rp);
    virtual ~Robot();

    Tag group() const { return tag_; }
    int number() const { return number_; }

    dWorldID getWorld() { return world_; }
    dSpaceID getOuterSpace() { return outer_space_; }
    dSpaceID getInnerSpace() { return inner_space_; }

    const IMessageHandler* getMsgHandler() const { return dynamic_cast<const IMessageHandler*>(this); }

    void getPosition(float &x,float &y,float &z) const;
    void setPosition(float x,float y,float z);
    void getQuaternion(float &w,float &x,float &y,float &z) const;
    void setQuaternion(float w, float x, float y, float z);

    void render(IDrawing*) const;

    void handleMessage(MessageID msg);

protected:
	dBodyID get_main_body() const { return parts_[0]->getBody(); }
    void create_main_body(const RobotParam& rp);
    void create_wheels(const RobotParam& rp);
    void create_dribbler(const RobotParam& rp);

protected:
    void handle_set_velocity(Message const* msg);
    void handle_set_dribbler(Message const* msg);

private:
    Tag tag_;
    int number_;

    dWorldID world_;
    dSpaceID outer_space_;
    dSpaceID inner_space_;

	typedef std::tr1::shared_ptr<IPart> PartSP;
	typedef std::vector<PartSP> PartVector;

	PartVector parts_;
	dJointGroupID joint_group_;

	std::vector<dJointID> wheel_motors_;
	dJointID dribbler_motor_;

    RobotAI ai_;
};


#endif
