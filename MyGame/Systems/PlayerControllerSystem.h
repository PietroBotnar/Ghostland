#pragma once
#include "..\ECS\ISystem.h"
#include "..\ECS\Entity.h"
#include <windows.h>
#include "..\Components\TransformComponent.h"
#include "..\ECS\GameFSM.h"

class PlayerControllerSystem : 	public ISystem
{
public:
	explicit PlayerControllerSystem(){};
	~PlayerControllerSystem(){};

	virtual void InitialiseSystem() override;
	virtual void Update(float dt) override;
	virtual void OnEvent(IEvent* _event) override;
	virtual void OnEnterGameState(GameState state) override;

	

private:
	void Shoot();
	void ThrowGrenade();

	void KeyDownListeners(double dt, TransformComponent* movement);

	void DisplayThrowingForce();

	Entity* Player	= NULL;

	float _throwingForce = 0;
	float _movementSpeed = 0;
	
	bool _shot		= false;
	bool _grenade	= false;
	bool _torch     = false;
	bool _running	= false;
	bool _playing   = false;

	bool _haveGrenades = false;

	const float WALKING_SPEED = 100;
	const float RUNNING_SPEED = 150;

	const int SHOOTING_ANGLE = 35;

	const int THROW_FORCE_MAX = 100;
	const int THROW_FORCE_MIN = 30;
};

