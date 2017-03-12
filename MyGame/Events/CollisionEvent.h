#pragma once
#include "..\ECS\IEvent.h"
#include "..\Data\CollisionInfo.h"

class Entity;

class CollisionEvent :	public IEvent
{
public:
	explicit CollisionEvent(CollisionInfo collision, float _dt) : IEvent(Collision)
	{
		collisionInfo = collision;
		dt = _dt;
	};

	~CollisionEvent(){};

	float dt;
	CollisionInfo collisionInfo;
};
