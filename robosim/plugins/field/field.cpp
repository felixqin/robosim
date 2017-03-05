#include <fstream>
#include <robosim/robosim.h>
#include "plane.h"
#include "ball.h"
#include "config.hpp"


static const char* plane_tex = "field_plane.ppm";
static const char* config_file = "/field.ini";

Plane* plane = 0;
Ball* ball = 0;

extern "C" bool PluginInstall(const SystemParameters* sp, ISimulator* sim, IResourceManager* rm)
{
    std::string inifile(sp->plugin_path);
    inifile.append(config_file);

    std::ifstream ifs(inifile.c_str());
    ini_variables ini(parse_ini_file(ifs));

    FieldParam fp;
    BallParam bp;
    load_field_config(ini,fp);
    load_ball_config(ini,bp);
#if 1
    TRACE("field.door.thickness:%f",fp.door.thickness);
#endif

    // load texture
    TextureID plane_tex_id = rm->registerTexture(plane_tex);

    // create Entity
    IWorld *model = sim->getModel();
    dWorldID world = model->getWorld();
    dSpaceID space = model->getSpace();

    plane = new Plane(world,space,MakeTag("FLD"),0,fp);
    plane->setGroundTexture(plane_tex_id);

    ball = new Ball(world,space,MakeTag("BALL"),0,bp.radius,bp.mass);
    ball->setPosition(-0.5,0.8,1);

    model->addEntity(plane);
    model->addEntity(ball);

    return true;
}

extern "C" void PluginUninstall()
{
    delete plane;
    delete ball;
}

extern "C" void PluginTimeslice()
{
}
