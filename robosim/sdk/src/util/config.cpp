#include <stdexcept>
#include "robosim/util/log.h"
#include "robosim/util/config.h"
#include "impl/config_impl.h"

using namespace config;

ConfigReader CreateConfigReader(const char* fname)
{
    try
    {
        IOImpl* io = new IOImpl(fname);
        io->read();
        return io;
    }
    catch (...)
    {
        LogData("failed to create Config reader!");
    }
    return 0;
}
void DestroyConfigReader(ConfigReader cfg)
{
    delete cfg;
}
ConfigSection GetConfigSection(ConfigReader cfg, const char* section)
{
    IOImpl* io = cfg;
    if ( io && !io->getConfig().empty() )
    {
        const SectionTable st = io->getConfig().getSections();
        SectionTable::const_iterator it;
        for ( it = st.begin(); it != st.end(); ++it )
        {
            if ( (*it)->name() == section ) return (*it).get();
        }
    }
    return 0;
}
const char* GetConfigValue(ConfigSection sec, const char* var)
{
    SectionImpl* impl_ = sec;
    if ( impl_ && !impl_->empty())
    {
        const VarTable vt = impl_->getVars();
        VarTable::const_iterator it;
        for ( it = vt.begin(); it != vt.end(); ++it )
        {
            if ( (*it)->name() == var ) return (*it).get()->value().c_str();
        }
    }
    return 0;
}

