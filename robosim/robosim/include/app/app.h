#pragma once

#include <robosim/base/param.h>
#include "pluginmgr.h"

class ISimUI;
class ITimer;

class Application
{
    const SystemParameters* sp_;

    ITimer*         sysTimer_;
    ITimer*         simTimer_;
    ISimUI*         ui_;
    ISimulator*     simulator_;
    IResourceManager* rm_;

    PluginManager   pluginmgr_;

public:
    Application(const SystemParameters* sp,
                ITimer* sysTimer,
                ITimer* simTimer,
                ISimUI* ui,
                ISimulator* sim,
                IResourceManager* rm);
    ~Application();

    void start(int w=640, int h=480);
    void loop();
};
