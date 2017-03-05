#include <vector>
#include <string>
#include <robosim/util/debug.h>
#include "platform/platform.h"
#include "sim/simulator.h"
#include "app/dllplugin.h"
#include "app/pluginmgr.h"

using namespace std;

PluginManager::PluginManager(const char* plugin_dir)
{
    vector<string> dllfiles;
    if (platform::SearchFiles(dllfiles,plugin_dir,DLL_EXTNAME))
    {
        vector<string>::iterator it = dllfiles.begin();
        for ( ; it != dllfiles.end(); it++ )
        {
            DllPlugin* plugin = new DllPlugin(it->c_str());
            if (plugin->isValid())
            {
				TRACE("load plugin: %s\n",it->c_str());
                plugins_.push_back(plugin);
            }
            else
            {
				TRACE("load plugin failured: %s\n",it->c_str());
                delete plugin;
            }
        }
    }
}

PluginManager::~PluginManager()
{
    std::vector<IPlugin*>::const_iterator it = plugins_.begin();
    for ( ; it != plugins_.end(); it++ )
    {
        delete *it;
    }
}

void PluginManager::install(const SystemParameters* sp, ISimulator* sim, IResourceManager* rm)
{
    std::vector<IPlugin*>::const_iterator it = plugins_.begin();
    for ( ; it != plugins_.end(); it++ )
    {
        (*it)->install(sp,sim,rm);
    }
}

void PluginManager::uninstall()
{
    std::vector<IPlugin*>::iterator it = plugins_.begin();
    for ( ; it != plugins_.end(); it++ )
    {
        (*it)->uninstall();
    }
    plugins_.clear();
}

void PluginManager::timeslice()
{
    std::vector<IPlugin*>::const_iterator it = plugins_.begin();

    for ( ; it != plugins_.end(); it++ )
    {
        (*it)->timeslice();
    }

}
