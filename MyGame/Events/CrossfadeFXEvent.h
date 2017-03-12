#pragma once
#include "..\ECS\IEvent.h"
#include "..\Data\Vector3f.h"

class CrossfadeFXEvent : public IEvent
{
public:
	explicit CrossfadeFXEvent(CVector3f rgb) : IEvent(CrossfadeFX)
	{
		color = rgb;
	};

	~CrossfadeFXEvent(){};

	CVector3f color;
};

