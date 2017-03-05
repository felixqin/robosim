#ifndef IPLUGIN_H__
#define IPLUGIN_H__

#include "../precomp.h"

class ISimulator;
class IResourceManager;
class SystemParameters;

class IPlugin
{
public:
    virtual ~IPlugin() {}

    virtual bool install(const SystemParameters*, ISimulator*, IResourceManager*) = 0;
    virtual void uninstall() = 0;

    virtual void timeslice() = 0;
};

#endif //IPLUGIN_H__
