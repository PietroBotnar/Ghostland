#pragma once
#include "../ECS/IComponent.h"
#include "../Data/Vector3f.h"

class TransformComponent : public IComponent
{
public:
	explicit TransformComponent()
	{
		Direction	= CVector3f(0, 0, 1);
		Position	= CVector3f(0, 0, 0);
		Scale		= CVector3f(1, 1, 1);
		Rotation	= CVector3f();
	}

	TransformComponent(CVector3f position)
	{
		Position	= position;
		Direction	= CVector3f(0, 0, 1);
		Scale		= CVector3f(1, 1, 1);
		Rotation	= CVector3f();
	};

	TransformComponent(CVector3f position, CVector3f direction)
	{
		Position	= position;
		Direction	= direction;
		Scale		= CVector3f(1, 1, 1);
		Rotation	= CVector3f();
	};

	TransformComponent(CVector3f position, CVector3f direction, CVector3f scale)
	{
		Position	= position;
		Direction	= direction;
		Scale		= scale;
		Rotation	= CVector3f();
	};

	void Rotate(float angle, CVector3f axis)
	{
		Direction = Direction.RotateAboutAxis(angle, axis);
	}

	~TransformComponent(){};

	CVector3f Direction;

	CVector3f Position;
	CVector3f Scale;
	CVector3f Rotation;
};
