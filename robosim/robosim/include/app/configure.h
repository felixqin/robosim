#ifndef CONFIGURE_H__
#define CONFIGURE_H__

#include <robosim/base/param.h>

class Configure
{
public:
    static Configure* instance();

    Configure();
    virtual ~Configure();

    const SystemParameters& getSysParam() const
    {
        return sys_param_;
    }
    const ModelParameters& getSimParam() const
    {
        return mod_param_;
    }
protected:
    void load_config();
private:
    SystemParameters    sys_param_;
    ModelParameters     mod_param_;
};

inline
Configure* Configure::instance()
{
    static Configure config;
    return &config;
}


#endif
