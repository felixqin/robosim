#pragma once

#include <robosim/util/ini.hpp>


struct GroundParam
{
    Real length;
    Real width;
    dSurfaceParameters surface;
};

struct BarParam
{
    Real width;
    Real height;
    dSurfaceParameters surface;
};

struct DoorParam
{
    Real width;
    Real height;
    Real deep;
    Real thickness;
    dSurfaceParameters surface;
};

struct FieldParam
{
    GroundParam ground;
    BarParam bar;
    DoorParam door;
};

struct BallParam
{
    Real radius;
    Real mass;
};

inline
void load_surface_config(ini_variables& ini, std::string const& section, dSurfaceParameters& surface)
{
    memset(&surface,0,sizeof(surface));
    surface.mu = ini.get(section+".mu",dInfinity);
}

inline
void load_field_config(ini_variables& ini, FieldParam& field)
{
    field.ground.length = ini.get("ground.length",6.6);
    field.ground.width = ini.get("ground.width",4.7);
    load_surface_config(ini,"ground.surface",field.ground.surface);

    field.bar.width = ini.get("bar.width",0.05);
    field.bar.height = ini.get("bar.height",0.25);
    load_surface_config(ini,"bar.surface",field.bar.surface);

    field.door.width = ini.get("door.width",0.7);
    field.door.height = ini.get("door.height",0.2);
    field.door.deep = ini.get("door.deep",0.25);
    field.door.thickness = ini.get("door.thickness",0.02);
    load_surface_config(ini,"door.surface",field.door.surface);
}

inline
void load_ball_config(ini_variables& ini, BallParam& ball)
{
    ball.radius = ini.get("ball.radius",0.043);
    ball.mass = ini.get("ball.mass",0.046);
}
