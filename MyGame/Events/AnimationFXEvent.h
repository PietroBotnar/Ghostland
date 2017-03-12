#pragma once
#include "..\ECS\IEvent.h"
#include "..\Data\Enums.h"

class AnimationFXEvent : public IEvent
{
public:
	explicit AnimationFXEvent(SpriteAnimationID id, CVector3f p, float w, float h) : IEvent(AnimationFX)
	{
		AnimationID	= id;
		position	= p;
		width		= w;
		height		= h;
	};

	~AnimationFXEvent(){};

	SpriteAnimationID	AnimationID;
	CVector3f			position;
	float				width;
	float				height;
};

