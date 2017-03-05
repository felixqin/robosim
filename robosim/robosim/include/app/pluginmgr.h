#ifndef MANAGER_H__
#define MANAGER_H__

#include <vector>

class IPlugin;
class ISimulator;
class IResourceManager;
class SystemParameters;

class PluginManager
{
public:
    PluginManager(const char* plugin_dir);
    virtual ~PluginManager();

    void install(const SystemParameters* sp, ISimulator* sim, IResourceManager* rm);
    void uninstall();

    void timeslice();

private:
    std::vector<IPlugin*> plugins_;
};

#endif
