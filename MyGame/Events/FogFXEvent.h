#pragma once
#include "..\ECS\IEvent.h"
#include "..\Data\Vector3f.h"
class FogFXEvent : public IEvent
{
public:
	explicit FogFXEvent(bool active, float density, CVector3f rgb) : IEvent(FogFX)
	{
		Active	= active;
		Density = density;
		Color	= rgb;
	};

	explicit FogFXEvent(bool active) : IEvent(FogFX)
	{
		Active	= active;
		Density = 0;
		Color	= CVector3f();
	};

	~FogFXEvent(){};

	bool		Active;
	float		Density;
	CVector3f	Color;
};

