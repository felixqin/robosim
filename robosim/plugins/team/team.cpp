#include <boost/make_shared.hpp>
#include <robosim/robosim.h>
#include "team.h"
#include "robot.h"
#include "config.h"

RobotTeam::RobotTeam(ISimulator* sim, Tag group, const TeamConfig* cfg)
{
    IWorld *model = sim->getModel();
    dWorldID world = model->getWorld();
    dSpaceID space = model->getSpace();

    RobotParam const* rp = cfg->getRobotParam();

    for ( size_t i=0; i<cfg->getRobotCount(); ++i )
    {
        RobotSP robot(new Robot(group,i,world,space,*rp));
        //RobotSP robot = boost::make_shared<Robot>(group,i,world,space,*cfg->getRobotParam());
        float x,y,z;
        robot->getPosition(x,y,z);
        robot->setPosition(rp->first_x+rp->offset_x*i,rp->first_y+rp->offset_y*i,z);
        TRACE("Position: (%f,%f)",rp->first_x+rp->offset_x*i,rp->first_y+rp->offset_y*i);
        model->addEntity(robot.get());
        robotv_.push_back(robot);
    }
}

RobotTeam::~RobotTeam()
{
}
