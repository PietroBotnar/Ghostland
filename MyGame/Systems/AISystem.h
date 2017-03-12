#pragma once
#include "CollisionDetectionSystem.h"
#include "..\ECS\Entity.h"
#include "..\Objects\Timer.h"
#include "..\ECS\GameFSM.h"

#define _USE_MATH_DEFINES	

class AISystem : public ISystem
{
public:
	explicit AISystem(){};
	~AISystem(){};

	virtual void InitialiseSystem() override;
	virtual void Update(float dt) override;
	virtual void OnEvent(IEvent* _event) override;
	virtual void OnEnterGameState(GameState state) override;
	virtual void OnExitGameState(GameState state) override;

	CollisionDetectionSystem* CollsionDetectionSys;

private:
	void Behaviour(Entity* agent, float dt);

	//STEERING BEHAVIOURS
	CVector3f Alignment(Entity* agent, CVector3f& position);
	CVector3f Cohesion(Entity* agent, CVector3f& position);
	CVector3f Separation(Entity* agent, CVector3f& position);
	CVector3f ObstacleAvoidance(Entity* agent, CVector3f& position, CVector3f& velocity);
	CVector3f Seek(CVector3f & position, CVector3f & velocity, CVector3f & target);

	void CheckMapBounds(CVector3f & position, CVector3f & velocity);

	void ScreamSound(CVector3f & position);

	void SpawnEnemy();

	Timer _spawnTimer;
	Timer _screamTimer;

	int _enemiesToSpawn;

	//contains all the ghost in the game
	std::vector<Entity*> _ghosts;

	const float NEIGHBOUR_RADIUS			= 100.0f;
	const float CLOSE_DISTANCE				= 30.0f;
	const float PLAYER_AWARENESS_DISTANCE	= 100.0f;
	const float OBJECT_AWARENESS			= 5.0f;

	float MAX_VELOCITY	= 0.0f;

	//flocking weight factors
	const float _separationWeight	= 1.1f;
	const float _cohesionWeight		= 0.9f;
	const float _alignmentWeight	= 1.0f;
	const float _avoidanceWeight	= 1.5f;
	const float _seekingWeight		= 1.3f;

	bool _playing = false;

	CVector3f* _playerPosition;
};

