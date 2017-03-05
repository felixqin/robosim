#include <cassert>
#include <robosim/ext/messages.h>
#include "sim/communicator.h"

using namespace std;

Communicator::Communicator()
{
}
Communicator::~Communicator()
{
}
void Communicator::post(const IMessageHandler* handler,MessageID msg)
{
    assert(handler);
    boost::mutex::scoped_lock lock(mutex_);
    msgs_.push(MessagePair(handler,msg));
}
void Communicator::send(const IMessageHandler* handler,MessageID msg)
{
    assert(handler);
    IMessageHandler* h = const_cast<IMessageHandler*>(handler);
	h->handleMessage(msg);
	DestroyMessage(msg);
}

void Communicator::dispatch()
{
    boost::mutex::scoped_lock lock(mutex_);
    while (!msgs_.empty())
    {
        MessagePair mp = msgs_.front();
        send(mp.first,mp.second);
        msgs_.pop();
    }
}

