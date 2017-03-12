#pragma once
#include "EventTypes.h"

//Interface for events sent in the world
class IEvent
{
protected:
	IEvent(EventType type)
	{
		Type = type;
	};

public:
	EventType Type;
};

