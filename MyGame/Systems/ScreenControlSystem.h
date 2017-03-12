#pragma once
#include "..\ECS\ISystem.h"

//Controls the switch between game states
class ScreenControlSystem :	public ISystem
{
public:
	ScreenControlSystem(){};
	~ScreenControlSystem(){};

	virtual void InitialiseSystem() override;
	virtual void Update(float dt) override;
	virtual void OnEvent(IEvent* _event) override;
};

