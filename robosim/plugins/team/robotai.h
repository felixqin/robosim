#pragma once

#include <vector>
#include <array>
#include <ode/ode.h>
#include <robosim/types.h>
#include "robotparam.h"

class RobotParam;
class RobotAI
{
public:
    RobotAI(const RobotParam&);

    void setWheelVel(dJointID wheeel_motors[], float vx, float vy, float vtheta);

private:
    std::vector<std::tr1::array<Real,3> > vel_t_;   //速度转换矩阵;机器人速度到车轮角速度转换
};

