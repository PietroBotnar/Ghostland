#pragma once
#include "..\ECS\IComponent.h"
#include "..\Data\Vector3f.h"

//Defines the size of the bounding box centered at the object position
class BoxColliderComponent : public IComponent
{
public:
	BoxColliderComponent(CVector3f size)
	{
		Size = size;
	};

	~BoxColliderComponent(){};

	CVector3f Size;

	CVector3f GetMinPoint(CVector3f position)
	{
		return CVector3f(position.x - (Size.x / 2), position.y - (Size.y / 2), position.z - (Size.z / 2));
	};

	CVector3f GetMaxPoint(CVector3f position)
	{
		return CVector3f(position.x + (Size.x / 2), position.y + (Size.y / 2), position.z + (Size.z / 2));
	};
};

