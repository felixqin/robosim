#include <cstring>
#include <fstream>
#include <sstream>
#include <robosim/util/ini.hpp>

#include <robosim/util/debug.h>
#include "platform/platform.h"
#include "app/configure.h"


////////////////////////////////////////////////////////////////////////////////

static const char* CONFIG_FILENAME = "config.ini";

Configure::Configure()
{
    std::string path = platform::GetAppPath();

    std::strcpy(sys_param_.app_path, path.c_str());
    std::strcpy(sys_param_.install_path, path.c_str());

    path += DIRECTORY_SEPERATOR;
    std::string plugin_path = path + "plugins";
    std::string res_path = path + "resources";

    std::strcpy(sys_param_.plugin_path, plugin_path.c_str());
    std::strcpy(sys_param_.res_path, res_path.c_str());

    load_config();
}

Configure::~Configure()
{
}

void Configure::load_config()
{
    std::string fn = sys_param_.install_path;
    fn += DIRECTORY_SEPERATOR;
    fn += CONFIG_FILENAME;

    std::ifstream ini_file(fn.c_str());
    ini_variables ini(parse_ini_file(ini_file));

    sys_param_.frame_time = ini.get("System.FRAME_TIME",0.05f);
    sys_param_.sim_step_time = ini.get("System.SIM_STEP_TIME",0.01f);

    mod_param_.gravity = ini.get("Model.GRAVITY",9.81f);
    mod_param_.cfm = ini.get("Model.CFM",1e-5f);
    mod_param_.erp = ini.get("Model.ERP",0.2f);
    mod_param_.bounce = ini.get("Model.BOUNCE",0.8f);
    mod_param_.bounce_vel = ini.get("Model.BOUNCE_VEL",0.1f);

    mod_param_.max_step_time = ini.get("Model.MAX_STEP_TIME",0.2f);    //最大系统步进时间
    mod_param_.max_contacts = ini.get("Model.MAX_CONTACTS",10);
}
