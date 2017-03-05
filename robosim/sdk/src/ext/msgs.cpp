#include <algorithm>
#include "robosim/ext/messages.h"

inline
MessageID to_message_id(Message* msg)
{
    return reinterpret_cast<MessageID>(msg);
}

inline
Message* to_message(MessageID id)
{
    return reinterpret_cast<Message*>(id);
}

inline
Message* create_message(size_t data_size)
{
    Message* msg = (Message*)operator new(data_size+MESSAGE_HEADER_SIZE);
    msg->type = MSG_NULL;
    msg->data_size = data_size;
    return msg;
}

inline
void destroy_message(Message* msg)
{
    operator delete(msg);
}

/******************************************************************************/

Message* ToMessage(MessageID id)
{
    return to_message(id);
}

void DestroyMessage(MessageID id)
{
    destroy_message(to_message(id));
}

MessageID CreateMsg(int type,char const* data,size_t data_size)
{
    Message* msg = create_message(data_size);
    msg->type = type;
    std::copy(&data[0],&data[data_size],&msg->data[0]);
    return to_message_id(msg);
}
MessageID CreateVelMsg(int type,Real x, Real y, Real theta)
{
    Message* msg = create_message(sizeof(Real)*3);
    msg->type = type;
    msg->real[0]=x;
    msg->real[1]=y;
    msg->real[2]=theta;
    return to_message_id(msg);
}
