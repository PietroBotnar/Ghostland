#include "World.h"
#include "Entity.h"
#include "GameFSM.h"

World::World()
{
	EntityManager		= new ::EntityManager(this);
	_gameFSM			= new ::GameFSM();
}

World::~World()
{
	delete EntityManager;
}

void World::Initialise()
{
	_gameFSM->Activate(true);

	for (auto& system : _systems)
	{
		system->InitialiseSystem();
	}
}

void World::Update(float dt)
{
	for (auto& system : _systems)
	{
		system->Update(dt);
	}

	EntityManager->CleanUp();
}

ISystem* World::RegisterSystem(ISystem* system)
{
	system->Set(this, _nextSystemId);
	_systems.push_back(system);
	_nextSystemId++;

	return system;
}

void World::SendEvent(IEvent* _event)
{
	for (auto& system : _systems)
	{
		system->OnEvent(_event);
	}

	delete _event;
}

Entity* World::CreateEntity()
{
	return EntityManager->CreateEntity();
}

void World::TagEntity(int entityId, ComponentType tag)
{
	EntityManager->AddTaggedEntity(entityId, tag);
	EntityManager->GetEntity(entityId)->AddComposition(tag);
}

void World::RemoveTag(int entityId, ComponentType tag)
{
	EntityManager->RemoveTag(tag);
	EntityManager->GetEntity(entityId)->RemoveTag(tag);
}

GameState World::GetState()
{
	return _gameFSM->GetState();
}

void World::SetState(GameState state)
{
	for (auto& system : _systems)
	{
		system->OnExitGameState(_gameFSM->GetState());
	}
	
	_gameFSM->SetState(state);

	for (auto& system : _systems)
	{
		system->OnEnterGameState(state);
	}
}

template<typename T>
T* World::GetSystem()
{
	for (auto& system : _systems) {
		auto cast = static_cast<T*>(system);
		if(cast != nullptr)
			return system;
	}
	return nullptr;
}


