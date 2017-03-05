#pragma once

#include <cmath>

/**
  角度标准化
  将角度限定在(-pi,pi]之间
*/
inline
double angle_normalize(double a)
{
    double a1 = fmod(a,M_PI*2); // a1 is between (-2pi,2pi)
    if (!std::isgreater(a1,-M_PI))
    {
        a1 += M_PI*2;
    }
    else if (std::isgreater(a1,M_PI))
    {
        a1 -= M_PI*2;
    }
    return a1;
}

/**
  计算平面内向量与x轴的夹角
  返回值范围为[-pi,pi]
*/
inline
double get_angle(double x,double y)
{
    double r = sqrt(y*y+x*x);
    double tan__a_2 = (r-x)/y;    // tan(a/2)=csc(a/2)-cot(a/2)=r/y-x/y=(r-x)/y
    double a_2 = std::isnan(tan__a_2)?0:atan(tan__a_2);
    return a_2*2;
}

/**
  速度分解
  沿向量(px,py)方向的速度为v, 求其x,y两个坐标方向的速度(vx,vy)
*/
inline
void vel_depart(double px, double py, double v, double& vx, double& vy)
{
    using namespace std;

    double r = sqrt(px*px+py*py);
    double cosa = px/r;
    double sina = py/r;
    vx = isnan(cosa) ? 0 : v*cosa;
    vy = isnan(sina) ? 0 : v*sina;
}

/**
  坐标变换
  全局参考系中两物体s0,s1的位置分别为p0(x0,y0,theta0),p1(x1,y1,theta1)
  求s1在s0参考系中的位置p(x,y,theta)
*/
inline
void coord_trans(double x0,double y0,double theta0,
                 double x1,double y1,double theta1,
                 double& x,double& y,double& theta)
{
    double cosa = cos(theta0);
    double sina = sin(theta0);
    double dx = x1-x0;
    double dy = y1-y0;
    x = dx*cosa+dy*sina;
    y = dy*cosa-dx*sina;
    theta = theta1-theta0;
}

/**
  产生瞬时速度
    物体原位置(x0,y0,theta0),目标位置(x1,y1,theta1)
    预计最大直线速度maxv,最大旋转角速度maxw
    求出相对于原物体的参考系中的x,y方向的速度分量和旋转角速度
*/
inline
void gen_vel(double x0,double y0,double theta0,
             double x1,double y1,double theta1,
             double maxv,double maxw,
             double& vx,double& vy,double& w)
{
    double x,y,theta;
    coord_trans(x0,y0,theta0,x1,y1,theta1,x,y,theta);
    vel_depart(x,y,maxv,vx,vy);
    static double const k=500;   //每一弧度差值的角速度(w/dtheta)
    w=angle_normalize(theta1-theta0)*k;
    if(std::isgreater(w,maxw))
    {
        w=maxw;
    }
    else if(std::isless(w,-maxw))
    {
        w=-maxw;
    }
}
