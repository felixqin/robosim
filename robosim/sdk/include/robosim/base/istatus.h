#pragma once

#include <cstddef>
#include "../precomp.h"
#include "../types.h"

class IMessageHandler;


struct EntityStatus
{
    bool    invalid;
    float   px,py,pz;
    float   qw,qx,qy,qz;
    const IMessageHandler *msgHandler;
};

class ISimulatorStatus
{
    public:
        virtual ~ISimulatorStatus() {};

        virtual size_t getGroupMember(Tag group, int* out_buf, size_t buf_size) const = 0;
        virtual const IMessageHandler* getMsgHandler(Tag group, int number) const = 0;
        virtual bool getEntityStatus(Tag group,int number, EntityStatus& status) const = 0;
};
