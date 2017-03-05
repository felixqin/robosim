#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <iterator>
#include <robosim/util/ini.hpp>
#include <robosim/util/debug.h>

#include "config.h"


//static const Real PI = 3.1415926585897932384626f;
//static const Real ANGLE_UNIT = PI/180;


void load_wheel(ini_variables& ini, WheelParam* wheels, size_t count)
{
    std::string wheel_name="wheel.";
    unsigned size = wheel_name.size();
    for (size_t i=0; i<count; ++i)
    {
        wheel_name.push_back('0'+i);
        wheel_name.push_back('.');

        wheels[i].distance = ini.get(wheel_name+"distance",0.085f);
        wheels[i].angle = M_PI/180 * ini.get(wheel_name+"angle",90.0f*i-45.0f);
        wheels[i].cylinder.radius = ini.get(wheel_name+"radius",0.03f);
        wheels[i].cylinder.length = ini.get(wheel_name+"length",0.01f);
        wheels[i].cylinder.mass = ini.get(wheel_name+"mass",0.2f);
        wheels[i].motor.fmax = ini.get(wheel_name+"motor_fmax",36.0f);

        wheel_name.resize(size);
    }
}

void load_dribbler(ini_variables& ini, DribblerParam& dribbler)
{
    dribbler.distance = ini.get("dribbler.distance",0.045);
    dribbler.height = ini.get("dribbler.height",0.043);
    dribbler.cylinder.radius = ini.get("dribbler.radius",0.006);
    dribbler.cylinder.length = ini.get("dribbler.length",0.08);
    dribbler.cylinder.mass = ini.get("dribbler.mass",0.1);
    dribbler.motor.fmax = ini.get("dribbler.motor_fmax",30);
}


void load_trimesh(std::string const& dat_file,
            std::vector<Real>& vertices,
            std::vector<unsigned int>& indices,
            std::vector<Real>& normals)
{
    try
    {
        std::ifstream ifs(dat_file.c_str());
        if (!ifs.is_open())
        {
            throw std::runtime_error("Can not open the trimesh file!");
        }

        int vertex_count=0;
        ifs>>vertex_count;

        int index_count=0;
        ifs>>index_count;

        if (!(vertex_count && vertex_count%3==0 && index_count && index_count%3==0))
        {
            throw std::runtime_error("Error count of vertex and index!");
        }

        vertices.reserve(vertex_count);
        for (int i=0;i<vertex_count;++i)
        {
            Real vertex=0;
            ifs>>vertex;
            vertices.push_back(vertex);
        }

        indices.reserve(index_count);
        for (int i=0;i<index_count;++i)
        {
            int index;
            ifs>>index;
            indices.push_back(index);
        }

        normals.reserve(index_count);
        for (int i=0;i<index_count;++i)
        {
            double v;
            ifs>>v;
            normals.push_back(v);
        }
    }
    catch(std::exception const& e)
    {
        std::cerr<<e.what()<<std::endl;

        extern void load_default_trimesh(std::vector<Real>&,std::vector<unsigned int>&,std::vector<Real>&);
        load_default_trimesh(vertices,indices,normals);
    }
}

void TeamConfig::loadConfig(const char* ini_file)
{
    std::ifstream is(ini_file);
    ini_variables ini(parse_ini_file(is));

    robot_count_ = ini.get("robot.count",5);

    robot_param_.first_x = ini.get("robot.position.first.x",-0.4);
    robot_param_.first_y = ini.get("robot.position.first.y",1.5);
    robot_param_.offset_x = ini.get("robot.position.offset.x",-0.3);
    robot_param_.offset_y = ini.get("robot.position.offset.y",0.0);
    TRACE("robot.position: %f,%f,%f,%f",robot_param_.first_x,robot_param_.first_y,robot_param_.offset_x,robot_param_.offset_y);

    robot_param_.wheels_count = ini.get("robot.wheel_count",(int)4);
    wheels_.resize(robot_param_.wheels_count);
    load_wheel(ini,&wheels_[0],wheels_.size());
    robot_param_.wheels = &wheels_[0];

    load_dribbler(ini,robot_param_.dribbler);
    //todo: load kicker
    //todo: load kicker2

    //load trimesh data
    std::string trimesh_file = ini.get<std::string>("robot.shelter_data","");
    load_trimesh(trimesh_file,robot_vertices_,robot_indices_,robot_normals_);
    robot_param_.main_body.vertices_size = robot_vertices_.size();
    robot_param_.main_body.vertices = &robot_vertices_[0];
    robot_param_.main_body.indices_size = robot_indices_.size();
    robot_param_.main_body.indices = &robot_indices_[0];
    robot_param_.main_body.normals = &robot_normals_[0];
    robot_param_.main_body.mass = ini.get("robot.main_body_mass",2.0f);
}

