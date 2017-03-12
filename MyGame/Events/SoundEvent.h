#pragma once
#include "..\ECS\IEvent.h"
#include "..\Data\Enums.h"
#include "..\ECS\EventTypes.h"
#include "..\Data\Vector3f.h"

//Defines a sound event identified by a Sound ID
class SoundEvent : public IEvent
{

public:
	explicit SoundEvent(SoundID sound) : IEvent(EventType::Sound)
	{
		Sound = sound;
		is3D = false;
	};

	SoundEvent(SoundID sound, CVector3f position) : IEvent(EventType::Sound)
	{
		Sound = sound;
		SourcePosition = position;
		is3D = true;
	};

	~SoundEvent(){};

	SoundID		Sound;

	bool		is3D;
	CVector3f	SourcePosition;
};

