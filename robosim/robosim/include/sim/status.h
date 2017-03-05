#ifndef SIMSTATUS_H__
#define SIMSTATUS_H__

#include <unordered_map>
#include <boost/thread.hpp>
#include <robosim/base/istatus.h>


typedef std::pair<Tag,int> EntityIdPair;
typedef std::tr1::unordered_map<EntityIdPair,EntityStatus> EntityStatusMap;


class IEntity;
class ISimulator;

class SimStatus : public ISimulatorStatus
{
    public:
        void update(const ISimulator* sim);

        size_t getGroupMember(Tag group, int* out_buf, size_t buf_size) const;
        const IMessageHandler* getMsgHandler(Tag group, int number) const;
        bool getEntityStatus(Tag group,int number, EntityStatus& status) const;

    protected:
        void fetch_entity_status(const IEntity*);

    private:
        EntityStatusMap entity_map_;
        mutable boost::shared_mutex mutex_;
};

#endif // SIMSTATUS_H__
