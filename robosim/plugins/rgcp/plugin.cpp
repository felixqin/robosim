#include <functional>
#include <stdexcept>
#include <boost/thread.hpp>
#include <robosim/base.h>
#include "dispatcher.h"


boost::thread_group     thread_group;
Dispatcher*             dispatcher = NULL;
rgcp::server*           server = NULL;

void destroy_objects()
{
    if (server) server->stop();
    thread_group.join_all();
    delete server;
    delete dispatcher;
}

extern "C" bool PluginInstall(const SystemParameters* sp, ISimulator* sim, IResourceManager* rm)
{
    try
    {
        dispatcher = new Dispatcher(sim);

        rgcp::server::reply_function_type reply
            = std::tr1::bind(&Dispatcher::dispatch,dispatcher,std::tr1::placeholders::_1);
        server = new rgcp::server(reply);

        thread_group.create_thread(std::tr1::bind(&rgcp::server::run,server));
    }
    catch (std::exception const& e)
    {
        destroy_objects();
        return false;
    }
    return true;
}

extern "C" void PluginUninstall()
{
    destroy_objects();
}

extern "C" void PluginTimeslice()
{
}

