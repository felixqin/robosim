#include <cmath>
#include <robosim/util/debug.h>
#include "robotai.h"

RobotAI::RobotAI(const RobotParam& rp)
{
    // 初始化速度转换矩阵
    vel_t_.resize(rp.wheels_count);
    for ( size_t i=0; i<vel_t_.size(); ++i )
    {
        using namespace std;
        float ri = rp.wheels[i].cylinder.radius;
        vel_t_[i][0] = -sin(rp.wheels[i].angle) / ri;
        vel_t_[i][1] = cos(rp.wheels[i].angle) / ri;
        vel_t_[i][2] = rp.wheels[i].distance / ri;
    }
}

void RobotAI::setWheelVel(dJointID wheel_motors[], float vx, float vy, float vtheta)
{
    TRACE("(vx,vy,w): (%f,%f,%f",vx,vy,vtheta);
    for ( size_t i=0; i<vel_t_.size(); ++i )
    {
        float wi = vx * vel_t_[i][0] + vy * vel_t_[i][1] + vtheta * vel_t_[i][2];
        dJointSetHingeParam(wheel_motors[i],dParamVel,wi);
        TRACE("w%d:%f",i,wi);
    }
}
