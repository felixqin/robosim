#pragma once

/******************************************************************************/

struct CylinderParam
{
    Real radius;
    Real length;
    Real mass;
};

struct TrimeshParam
{
    Real*           vertices;
    size_t          vertices_size;
    unsigned int*   indices;
    size_t          indices_size;
    Real*           normals;
    Real            mass;
};

struct MotorParam
{
    Real fmax;
};

struct WheelParam
{
    Real            distance;
    Real            angle;
    CylinderParam   cylinder;
    MotorParam      motor;
};

struct DribblerParam
{
    Real distance; //与中心轴距离
    Real height;   //离地面高度
    CylinderParam cylinder;
    MotorParam  motor;
};

struct KickerParam
{
    Real distance;
    Real height;
    Real x;
    Real y;
    Real z;
    MotorParam motor;
};

struct RobotParam
{
    Real            first_x,first_y;    //第一个队员初始位置
    Real            offset_x,offset_y;  //以后每个成员相对于前一成员的位移量
    TrimeshParam    main_body;
    DribblerParam   dribbler;
    KickerParam     kicker;
    size_t          wheels_count;
    WheelParam*     wheels;
};


