#ifndef ISIMULATOR_H__
#define ISIMULATOR_H__

#include "../precomp.h"


class IPlugin;
class IWorld;
class ICommunicator;
class ISimulatorStatus;

class ISimulator
{
public:
    virtual ~ISimulator() {}

    virtual void timeslice() = 0;

    virtual IWorld* getModel() = 0;
    virtual ICommunicator* getCommunicator() = 0;
    virtual const ISimulatorStatus* getStatus() const = 0;
    virtual double getRealTime() const = 0;     //��ʵʱ��
    virtual double getModelTime() const = 0;    //ģ��ʱ��
};

#endif //ISIMULATOR_H__
