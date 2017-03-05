#include <stdexcept>
#include <robosim/base/itimer.h>
#include <robosim/base/isim.h>
#include <robosim/base/iworld.h>
#include <robosim/util/debug.h>
#include "app/isimui.h"
#include "app/app.h"
#include "app/pluginmgr.h"


Application::Application(const SystemParameters* sp,
                ITimer* sysTimer,
                ITimer* simTimer,
                ISimUI* ui,
                ISimulator* sim,
                IResourceManager* rm)
    :
    sp_(sp),
    sysTimer_(sysTimer),
    simTimer_(simTimer),
    ui_(ui),
    simulator_(sim),
    rm_(rm),
    pluginmgr_(sp->plugin_path)
{
}
Application::~Application()
{
    pluginmgr_.uninstall();
}

void Application::start(int w, int h)
{
    pluginmgr_.install(sp_,simulator_,rm_);

    if (!ui_->open(w,h))
    {
        throw std::runtime_error("cannot open ui!");
    }
}
void Application::loop()
{
    double t1,t2,tf=-sp_->frame_time;
    sysTimer_->reset();
    sysTimer_->start();
    simTimer_->reset();
    simTimer_->start();
    TRACE("simulator start...\n");

    for(;;)
    {
        //t1 = simulator.getRealTime();
        t1 = sysTimer_->elapsed();

        pluginmgr_.timeslice();
        simulator_->timeslice();

        if ( t1-tf >= sp_->frame_time )
        {
#if 0
            LogPrint("t1-tf: %f",t1-tf);
#endif
            tf = t1;
            if (ui_->closed()) break;

            IDrawing* draw = ui_->beginPaint();
            simulator_->getModel()->render(draw);
            ui_->endPaint();
        }
        //t2 = simulator.getRealTime();
        t2 = sysTimer_->elapsed();
#if 0
        LogPrint("t1: %f\tt2: %f\tdt: %f",t1,t2,t2-t1);
#endif
        float dt = sp_->sim_step_time - ( t2 - t1 );
        if (dt>0) ui_->sleep(dt);
    }
}
