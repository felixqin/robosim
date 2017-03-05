#ifndef DLL_PLUGIN_H__
#define DLL_PLUGIN_H__

#include <robosim/base/iplugin.h>
#include <robosim/base/plugindll.h>
#include "platform/platform.h"

class DllPlugin : public IPlugin
{
    DllPlugin(const DllPlugin&);
    DllPlugin& operator=(const DllPlugin&);
public:
    DllPlugin(const char* dllpath);
    virtual ~DllPlugin();

    bool isValid() {return valid_;}

    bool install(const SystemParameters* sp, ISimulator* sim, IResourceManager* rm)
    {
        if (install_) return install_(sp,sim,rm);
        return false;
    }
    void uninstall()
    {
        if (uninstall_) uninstall_();
    }
    void timeslice()
    {
        if (timeslice_) timeslice_();
    }

private:
    FuncPluginInstall install_;
    FuncPluginUninstall uninstall_;
    FuncPluginTimeslice timeslice_;

    platform::DynamicLibrary dll_;
    bool valid_;
};



#endif
