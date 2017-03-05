#ifndef IMSG_HANDLER_H__
#define IMSG_HANDLER_H__

#include "../precomp.h"

typedef int* MessageID;

class IMessageHandler
{
public:
	virtual ~IMessageHandler() {}
	virtual void handleMessage(MessageID msg) = 0;
};

#endif
