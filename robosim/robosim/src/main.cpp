#include <stdexcept>
#include <robosim/base/itimer.h>
#include <robosim/util.h>
#include "glfw/window.h"
#include "glfw/timer.h"
#include "app/isimui.h"
#include "app/app.h"
#include "app/resmgr.h"
#include "app/configure.h"
#include "sim/simulator.h"

int main(int argc, char** argv)
{
    try
    {
        //throw std::runtime_error("test1");
        const SystemParameters& sp = Configure::instance()->getSysParam();
        const ModelParameters&  mp = Configure::instance()->getSimParam();

        //std::string logfile = sp.install_path;
        //logfile += "/log.txt";
        //SetLogFileName(logfile.c_str());
        //TRACE(logfile.c_str());

		glfw::Window    window;
        glfw::Timer     sysTimer;
        glfw::Timer     simTimer;

        Simulator simulator(&mp,&simTimer);

        IResourceManager* rm = ResourceManager::instance();
        Application app(&sp,&sysTimer,&simTimer,&window,&simulator,rm);

        app.start();
        app.loop();
    }
    catch(const std::exception& e)
    {
        TRACE(e.what());
        return EXIT_FAILURE;
    }
    catch(...)
    {
        //TODO: add show error msg
        TRACE("unexcepted error!");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
