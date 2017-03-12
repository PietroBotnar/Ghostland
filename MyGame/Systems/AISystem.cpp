#include "AISystem.h"
#include "../ECS/World.h"
#include "../Factory.h"
#include <math.h>
#include <stdlib.h>
#include "../Events/SoundEvent.h"
#include "../Events/EnemySpawnEvent.h"
#include "../Events/AnimationFXEvent.h"

void AISystem::InitialiseSystem()
{
	_playerPosition = &_world->EntityManager->GetTaggedEntity(PlayerTag)->GetTransform()->Position;

	_spawnTimer		= Timer();
	_screamTimer	= Timer(3);

	_screamTimer.Activate();
}

void AISystem::OnEnterGameState(GameState state)
{
	_playing = _world->GetState() == Play;
}

void AISystem::OnExitGameState(GameState state)
{
	if(state != GameOver) return;

	//on exit from game over, reset the system
	for (auto it = _ghosts.begin(); it != _ghosts.end(); ++it)
	{
		(*it)->Destroy();
	}

	_ghosts.clear();

	_enemiesToSpawn = 0;
}

void AISystem::OnEvent(IEvent* _event)
{
	if(_event->Type != EnemySpawn) return;

	auto spawnEvent = static_cast<EnemySpawnEvent*>(_event);

	//clear old ghosts
	_ghosts.clear();

	//set spawn rate
	_spawnTimer.Set(spawnEvent->SpawnRate);

	//set enemies to spawn
	_enemiesToSpawn = spawnEvent->Quantity;

	//set enemy max velocity
	MAX_VELOCITY = spawnEvent->MaxVelocity;

	_spawnTimer.Activate();
}

void AISystem::Update(float dt)
{
	if(!_playing) return;

	if(_spawnTimer.UpdateTimer(dt) && _enemiesToSpawn > 0)
		SpawnEnemy();

	for (auto& ghost : _ghosts)
	{
		if(!ghost->IsAlive()) 
			continue;

		Behaviour(ghost, dt);
	}
}

void AISystem::ScreamSound(CVector3f & position)
{
	auto distance = position.XZDistance(*_playerPosition);

	//only scream if within 200 distance units and 50% chance
	if (distance < 100 && ((rand() % 100) <= 40)) {
		_world->SendEvent(new SoundEvent(SoundID::GhostScream));
	}
}

void AISystem::SpawnEnemy()
{
	auto position = CVector3f();
	auto direction = CVector3f();

	if (rand() % 100 < 50) {
		position = CVector3f(225, 6, 225);
		direction = CVector3f(0, 0, -1);
	}
	else {
		position = CVector3f(-225, 6, -225);
		direction = CVector3f(0, 0, 1);
	}

	_world->SendEvent(new AnimationFXEvent(Portal, position, 30, 30));
	_world->SendEvent(new SoundEvent(SoundID::Spawn));
	
	auto ghostColor = (MeshID)(rand() % 6);

	auto ghost = _world->Factory->CreateGhost(position, direction, ghostColor);
	_ghosts.push_back(ghost);
	_enemiesToSpawn--;

	//activate scream timer
	ghost->GetTimer()->Activate();

	_spawnTimer.Activate();
}

void AISystem::Behaviour(Entity* agent, float dt)
{
	auto position = agent->GetTransform()->Position;
	auto velocity = agent->GetMovement();

	//trigger scream sounds
	auto screamTimer = agent->GetTimer();
	if (screamTimer->Update(dt)) {
		ScreamSound(position);
		screamTimer->Activate();
	}

	auto futureVelocity = CVector3f();

	//apply flocking forces
	auto alignment	= Alignment(agent, position)	* _alignmentWeight;
	auto cohesion	= Cohesion(agent, position)		* _cohesionWeight;
	auto separation = Separation(agent, position)	* _separationWeight;
	
	futureVelocity = alignment + cohesion + separation;

	//apply player seeking force
	auto seekPlayer = Seek(position, futureVelocity,*_playerPosition) * _seekingWeight;

	futureVelocity += seekPlayer;
	futureVelocity.Normalise();

	//apply obstacle avoidance force
	auto avoidance = ObstacleAvoidance(agent, position, futureVelocity) * _avoidanceWeight;

	futureVelocity += avoidance;

	futureVelocity *= MAX_VELOCITY;
	futureVelocity.Truncate(MAX_VELOCITY);

	//make sure it doesn't move vertically
	futureVelocity.y = 0;

	futureVelocity = (velocity->Velocity + futureVelocity) / 2;

	CheckMapBounds(position, futureVelocity);

	//set final velocity force
	velocity->Velocity					= futureVelocity;
	agent->GetTransform()->Direction	= futureVelocity;
}

CVector3f AISystem::Alignment(Entity* myAgent, CVector3f& position)
{
	auto neighbourCount = 0;
	auto alignment = CVector3f();

	for (auto &agent : _ghosts)
	{
		if (agent->Equals(myAgent) || !agent->IsAlive()) continue;

		auto agentTransform = agent->GetTransform();
		float distance = position.XZDistance(agentTransform->Position);
		if (distance < NEIGHBOUR_RADIUS)
		{
			auto movement = agent->GetMovement();

			alignment += movement->Velocity;
			neighbourCount++;
		}
	}

	if (neighbourCount == 0)
		return alignment;

	//compute alignment
	alignment /= neighbourCount;

	alignment.Normalise();
	return alignment;
}

CVector3f AISystem::Cohesion(Entity * myAgent, CVector3f& position)
{
	auto neighbourCount = 0;
	auto cohesion = CVector3f();

	for (auto &agent : _ghosts)
	{
		if (agent->Equals(myAgent) || !agent->IsAlive()) continue;

		auto agentTransform = agent->GetTransform();
		float distance = position.XZDistance(agentTransform->Position);
		if (distance < NEIGHBOUR_RADIUS)
		{
			cohesion += agentTransform->Position;
			neighbourCount++;
		}
	}

	if (neighbourCount == 0)
		return cohesion;

	//compute cohesion
	cohesion /= neighbourCount;
	cohesion -= position;
	cohesion.Normalise();

	return cohesion;
}

CVector3f AISystem::Separation(Entity* myAgent, CVector3f& position)
{
	auto neighbourCount = 0;
	auto separation = CVector3f();

	for (auto &agent : _ghosts)
	{
		if (agent->Equals(myAgent) || !agent->IsAlive()) continue;

		auto agentTransform = agent->GetTransform();
		float distance = position.XZDistance(agentTransform->Position);
		if (distance <= NEIGHBOUR_RADIUS)
		{
			auto movement = agent->GetMovement();

			auto diff = position - agentTransform->Position;
			diff.Normalise();
			diff /= distance;

			separation += diff;

			neighbourCount++;
		}
	}

	if (neighbourCount == 0)
		return separation;

	//compute separation
	separation /= neighbourCount;
	separation.Normalise();

	return separation;
}

CVector3f AISystem::ObstacleAvoidance(Entity* agent, CVector3f& position, CVector3f& velocity)
{
	auto avoidance = CVector3f();
	//check	if agent can see obstacles ahead using AABB check

	auto velocityCopy = velocity;

	auto ahead = position + velocity * OBJECT_AWARENESS;

	float distance; CVector3f normal, center;
	if (CollsionDetectionSys->CollidesStatics(position, ahead/2, ahead, agent->GetBoxCollider(), center)) {
		avoidance = ahead - center;
		avoidance.Normalise();
	}
	return avoidance;
}

CVector3f AISystem::Seek(CVector3f & position, CVector3f & velocity, CVector3f & target)
{
	auto seek = CVector3f();

	auto distance = position.XZDistance(target);
//	if (distance < PLAYER_AWARENESS_DISTANCE) {

		seek = target - position;
		seek.Normalise();
		seek.y = 0;
//	}
	return seek;
}

void AISystem::CheckMapBounds(CVector3f & position, CVector3f & velocity)
{
	auto next = position;
	if (abs(next.x) >= 245) 
		velocity.x *= -1;

	if( abs(next.z) >= 240) 
		velocity.z *= -1;
}

