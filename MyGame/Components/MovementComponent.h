#pragma once
#include "../ECS/IComponent.h"
#include "../Data/Vector3f.h"

class MovementComponent : public IComponent
{
public:
	explicit MovementComponent()
	{
		Velocity = CVector3f(0, 0, 0);
	}

	MovementComponent(CVector3f velocity)
	{
		Velocity = velocity;
	}

	void Set(CVector3f velocity) {
		Velocity = velocity;
	}

	void Rotate(float angle) 
	{
		Velocity = Velocity.RotateAboutAxis(angle, CVector3f(0, 1, 0));
	}

	~MovementComponent(){};

	CVector3f Velocity;
};

