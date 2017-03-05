#ifndef SIMULATOR_H__
#define SIMULATOR_H__

#include <vector>

#include <robosim/base/param.h>
#include <robosim/base/itimer.h>
#include <robosim/base/icomm.h>
#include <robosim/base/isim.h>
#include <robosim/base/iworld.h>
#include "sim/status.h"


class Simulator : public ISimulator
{
public:
    Simulator(const ModelParameters* mp, ITimer* timer);
    virtual ~Simulator();

    void timeslice();

    IWorld* getModel();
    ICommunicator* getCommunicator();
    const ISimulatorStatus* getStatus() const;
    double getRealTime() const;     //现实时间
    double getModelTime() const;    //模型时间

private:
    ITimer* timer_;

    IWorld* model_;
    ICommunicator* communicator_;
    SimStatus status_;
};


/******************************************************************************/

inline
IWorld* Simulator::getModel()
{
    return model_;
}
inline
ICommunicator* Simulator::getCommunicator()
{
    return communicator_;
}
inline
double Simulator::getRealTime() const
{
    return timer_->elapsed();
}
inline
double Simulator::getModelTime() const
{
    return model_->getTime();
}
inline
const ISimulatorStatus* Simulator::getStatus() const
{
    return &status_;
}

#endif //SIMULATOR_H__
