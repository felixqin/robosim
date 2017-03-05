#ifndef ITIMER_H__
#define ITIMER_H__

#include "../precomp.h"

class ITimer
{
public:
    virtual void reset() = 0;
    virtual double elapsed() const = 0;
    virtual void pause() = 0;
    virtual void start() = 0;
};


#endif //ITIMER_H__
