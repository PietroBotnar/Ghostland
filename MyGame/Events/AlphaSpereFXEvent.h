#pragma once
#include "..\ECS\IEvent.h"
#include "..\Data\Vector3f.h"

class AlphaSpereFXEvent : public IEvent
{
public:
	explicit AlphaSpereFXEvent(CVector3f _position, float _radius, CVector3f _startColour, CVector3f _endColour, bool _zap, float _zapLevel) : IEvent(AlphaSpereFX)
	{
		position	= _position;
		radius		= _radius;
		startColour = _startColour;
		endColour	= _endColour;
		zap			= _zap;
		zapLevel	= _zapLevel;
	};

	~AlphaSpereFXEvent(){};

	CVector3f	position;
	float		radius;
	CVector3f	startColour;
	CVector3f	endColour;
	bool		zap;
	float		zapLevel;
};

