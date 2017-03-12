#pragma once
#include "..\ECS\IEvent.h"

class EnemySpawnEvent :	public IEvent
{
public:
	explicit EnemySpawnEvent(int quantity, float rate, float enemyVelocity) : IEvent(EnemySpawn) {
		Quantity = quantity;
		SpawnRate = rate;
		MaxVelocity = enemyVelocity;
	}

	~EnemySpawnEvent(){};

	float	SpawnRate;
	int		Quantity;
	float	MaxVelocity;
};

