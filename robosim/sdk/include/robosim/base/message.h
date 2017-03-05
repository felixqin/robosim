#ifndef MESSAGE_H__
#define MESSAGE_H__

#include <stddef.h>
#include "../precomp.h"
#include "../types.h"

enum MessageType
{
    MSG_NULL = 0,
    MSG_SET_DRIBBLER = 0x0200,  //2xx 表示使用整数
    MSG_SET_VELOCITY = 0x0300,  //3xx 表示使用real
    MSG_USER = 0x1000,
};

struct Message
{
    int     type;
    size_t  data_size;
    union
    {
        char    data[1];    //数据指针
        char    string[1];
        int     integer[1];
        Real    real[1];
    };
};

enum { MESSAGE_HEADER_SIZE = sizeof(int)+sizeof(size_t) };


#endif // MESSAGE_H__
