#ifndef PLUGIN_DLL_H__
#define PLUGIN_DLL_H__

#include "../precomp.h"


#define FUNC_PLUGIN_INSTALL     "PluginInstall"
#define FUNC_PLUGIN_UNINSTALL   "PluginUninstall"
#define FUNC_PLUGIN_TIMESLICE   "PluginTimeslice"

class SystemParameters;
class ISimulator;
class IResourceManager;

//void (*FuncPluginVersion(PluginVersion*);
typedef bool (*FuncPluginInstall)(const SystemParameters*, ISimulator*, IResourceManager*);
typedef void (*FuncPluginUninstall)();
typedef void (*FuncPluginTimeslice)();


#ifdef PLUGIN_EXPORTS
extern "C"
{
    bool PLUGIN_API PluginInstall(const SystemParameters*, ISimulator*, IResourceManager*);
    void PLUGIN_API PluginUninstall();
    void PLUGIN_API PluginTimeslice();
}
#endif



#endif
