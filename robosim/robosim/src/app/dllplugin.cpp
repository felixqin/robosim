#include "app/dllplugin.h"

using namespace platform;

DllPlugin::DllPlugin(const char* dllpath)
    :
    install_(0),
    uninstall_(0),
    timeslice_(0),
    dll_(dllpath)
{
    if (dll_.is_open())
    {
        install_ = reinterpret_cast<FuncPluginInstall>
                        (dll_.getSymbol(FUNC_PLUGIN_INSTALL));
        uninstall_ = reinterpret_cast<FuncPluginUninstall>
                        (dll_.getSymbol(FUNC_PLUGIN_UNINSTALL));
        timeslice_ = reinterpret_cast<FuncPluginTimeslice>
                        (dll_.getSymbol(FUNC_PLUGIN_TIMESLICE));
    }
    valid_ = install_ && uninstall_ && timeslice_;
}

DllPlugin::~DllPlugin()
{
}
