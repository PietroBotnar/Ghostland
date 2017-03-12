#pragma once
#include "..\ECS\IComponent.h"
#include "..\Objects\Timer.h"

class TimerComponent :	public IComponent
{
public:
	TimerComponent(int trigger) {
		timer = Timer(trigger);
	};

	~TimerComponent(){};

	bool Update(float dt) {return timer.UpdateTimer(dt);}
	void Activate(bool active = true) { timer.Activate(active);}

	Timer timer;
};

