#pragma once
#include <vector>
#include "IEvent.h"
#include "GameFSM.h"

class World;

//Base class for System
class ISystem
{
public:
	explicit ISystem(){};
	virtual ~ISystem(){};

	virtual void InitialiseSystem() = 0;
	virtual void Update(float dt)	= 0;

	virtual void OnEvent(IEvent* _event){};

	virtual void OnEnterGameState(GameState state){};
	virtual void OnExitGameState(GameState state){};

	std::vector<int> FamilyListeners;

	void Set(World* world, unsigned int id) 
	{
		_world	= world;
		_id		= id;
	}

protected:
	World*			_world;
	unsigned int	_id;
};
