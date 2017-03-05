#ifndef ICOMMUNICATOR_H__
#define ICOMMUNICATOR_H__

#include "../precomp.h"
#include "imsghandler.h"

class ICommunicator
{
public:
    virtual ~ICommunicator() {}

    virtual void post(const IMessageHandler* to, MessageID) = 0;
    virtual void send(const IMessageHandler* to, MessageID) = 0;
    virtual void dispatch() = 0;

	//virtual void post(std::tr1::shared_ptr<IMessage> msg);
};


#endif // ICOMMUNICATOR_H__
