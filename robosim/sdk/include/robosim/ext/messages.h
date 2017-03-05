#pragma once

#include "../precomp.h"
#include "../base/imsghandler.h"
#include "../base/message.h"

extern "C"
{

SDK_API Message*    ToMessage(MessageID);
SDK_API void        DestroyMessage(MessageID);

SDK_API MessageID CreateMsg(int type, char const* data,size_t data_size);
SDK_API MessageID CreateVelMsg(int type, float x, float y, float theta);
};
