#include <algorithm>
#include <vector>
#include <utility>
#include <robosim/base/ientity.h>
#include "sim/status.h"
#include "sim/simulator.h"
#include "sim/world.h"


/******************************************************************************/
/*
void ergod_entities(void* data, const IEntity* entity)
{
    SimStatus* ss = static_cast<SimStatus*>(data);
    assert(ss);
    ss->ergod(entity);
}
*/
/******************************************************************************/

void SimStatus::fetch_entity_status(const IEntity* entity)
{
    EntityStatus status;
    status.invalid = true;  //TODO: 请根据实际情况赋值
    entity->getPosition(status.px,status.py,status.pz);
    entity->getQuaternion(status.qw,status.qx,status.qy,status.qz);
    status.msgHandler = entity->getMsgHandler();

    EntityIdPair id = std::make_pair(entity->group(),entity->number());
    entity_map_[id] = status;
}

void SimStatus::update(const ISimulator* sim)
{
    assert(sim);
    boost::unique_lock<boost::shared_mutex> lock(mutex_);
    entity_map_.clear();

    const World* world= dynamic_cast<World*>(((ISimulator*)sim)->getModel());
    assert(world);
    EntityTable const& et = world->getEntityTable();

    using namespace std::tr1;
    std::for_each(et.begin(),et.end(),bind(&SimStatus::fetch_entity_status,this,placeholders::_1));
}

const IMessageHandler* SimStatus::getMsgHandler(Tag group, int number) const
{
    EntityIdPair id(group,number);
    boost::shared_lock<boost::shared_mutex> lock(mutex_);
    EntityStatusMap::const_iterator it = entity_map_.find(id);
    if ( it != entity_map_.end() )
    {
        return it->second.msgHandler;
    }
    return 0;
}

size_t SimStatus::getGroupMember(Tag group, int* out_buf, size_t buf_size) const
{
    std::vector<int> list;
    boost::shared_lock<boost::shared_mutex> lock(mutex_);
    EntityStatusMap::const_iterator it = entity_map_.begin();
    for ( ; it != entity_map_.end(); ++it )
    {
        if (it->first.first==group) list.push_back(it->first.second);
    }
    size_t count = std::min(list.size(),buf_size);
    std::copy(&list[0],&list[count],out_buf);
    return list.size();
}

bool SimStatus::getEntityStatus(Tag group,int number, EntityStatus& status) const
{
    EntityIdPair id = std::make_pair(group,number);
    boost::shared_lock<boost::shared_mutex> lock(mutex_);
    EntityStatusMap::const_iterator it = entity_map_.find(id);
    if (it!=entity_map_.end())
    {
        status = it->second;
        return true;
    }
    return false;
}
