#pragma once
#include "..\ECS\IEvent.h"

class TorchToggleEvent :	public IEvent
{
public:
	explicit TorchToggleEvent() : IEvent(Torch) 
	{
	}

	~TorchToggleEvent(){};
};