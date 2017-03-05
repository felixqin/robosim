#include <string>
#include <robosim/base.h>
#include <robosim/util/debug.h>
#include "team.h"
#include "config.h"

static const char* blue_team_ini = "blue_team.ini";
static const char* yellow_team_ini = "yellow_team.ini";

RobotTeam*  blue_team = 0;
TeamConfig* blue_config = 0;
RobotTeam*  yellow_team = 0;
TeamConfig* yellow_config = 0;

extern "C" bool PluginInstall(const SystemParameters* sp, ISimulator* sim, IResourceManager* rm)
{
    std::string path(sp->plugin_path);
    path.push_back('/');

    std::string blue_inifile = path+blue_team_ini;
    TRACE("blue ini file: %s",blue_inifile.c_str());
    blue_config = new TeamConfig(blue_inifile.c_str());
    blue_team = new RobotTeam(sim,MakeTag("BLUE"),blue_config);

    std::string yellow_inifile = path+yellow_team_ini;
    TRACE("yellow ini file: %s",yellow_inifile.c_str());
    yellow_config = new TeamConfig(yellow_inifile.c_str());
    yellow_team = new RobotTeam(sim,MakeTag("YLLW"),yellow_config);

    return true;
}

extern "C" void PluginUninstall()
{
    delete blue_team;
    delete blue_config;
    delete yellow_team;
    delete yellow_config;
}

extern "C" void PluginTimeslice()
{
}

