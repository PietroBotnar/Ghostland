#pragma once
#include "EntityManager.h"
#include "ISystem.h"
#include <vector>
#include "..\Data\Vector3f.h"
#include "GameFSM.h"
#include "ComponentManager.h"

class Factory;

//Manages entities and systems
class World
{
public:
	World();
	~World();

	EntityManager*		EntityManager;
	
	Factory*			Factory;

	void Initialise();

	void Update(float dt);

	ISystem* RegisterSystem(ISystem* system);

	void SendEvent(IEvent* _event);

	template<typename T>
	T* GetSystem();

	Entity* CreateEntity();

	void TagEntity(int entityId, ComponentType tag);
	void RemoveTag(int entityId, ComponentType tag);

	// Return the camera position (in world coordinates)
	CVector3f& GetCameraPosition() const { return *_cameraPosition; };

	// Return the point where the camera is looking (in world coordinates)
	CVector3f& GetCameraViewPoint() const {return *_cameraViewPoint;} ;

	void SetCameraPostion(CVector3f* position)
	{
		_cameraPosition = position;
	}

	void SetCameraViewpoint(CVector3f* point) 
	{
		_cameraViewPoint = point;
	}

	//Returns current FSM state
	GameState	GetState();

	//Updates fsm state and notifies all systems
	void		SetState(GameState state);

private:
	unsigned int _nextSystemId = 0;
	std::vector<ISystem*> _systems;

	CVector3f* _cameraPosition;
	CVector3f* _cameraViewPoint;

	GameFSM*   _gameFSM;
};