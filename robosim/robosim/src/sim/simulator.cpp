#include <cassert>
#include "sim/simulator.h"
#include "sim/world.h"
#include "sim/communicator.h"

Simulator::Simulator(const ModelParameters* mp, ITimer* timer)
    :
    timer_(timer)
{
    model_ = new World(mp);
    communicator_ = new Communicator();

    //timer_->restart();
}

Simulator::~Simulator()
{
    delete communicator_;
    delete model_;
}

/******************************************************************/

void Simulator::timeslice()
{
    communicator_->dispatch();
    model_->step(getRealTime()-model_->getTime());    //向现实时间演化
    status_.update(this);
}
