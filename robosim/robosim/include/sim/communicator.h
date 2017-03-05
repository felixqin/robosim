#ifndef COMMUNICATOR_H__
#define COMMUNICATOR_H__

#include <queue>
#include <utility>
#include <boost/thread/mutex.hpp>
#include <robosim/base/icomm.h>


class Communicator : public ICommunicator
{
	Communicator(const Communicator&);
	Communicator& operator=(const Communicator&);
public:
    Communicator();
    virtual ~Communicator();

    void post(const IMessageHandler* handler,MessageID);
    void send(const IMessageHandler* handler,MessageID);
    void dispatch();

private:
    typedef std::pair<const IMessageHandler*,MessageID> MessagePair;
    typedef std::queue<MessagePair> MessageQueue;

    MessageQueue msgs_;
    boost::mutex mutex_;
};


#endif // COMMUNICATOR_H__
