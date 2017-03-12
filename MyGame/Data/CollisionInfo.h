#pragma once
#include "..\ECS\Entity.h"

class CollisionInfo
{
public:
	CollisionInfo(){};

	CollisionInfo(Entity* ent1, Entity* ent2, CVector3f norm, float distance)
	{
		Entity_1	= ent1;
		Entity_2	= ent2;
		Normal		= norm;
		Distance	= distance;
	};

	~CollisionInfo(){};

	CVector3f GetOppositeDistance()
	{
		return -1*(Normal * Distance);
	}

	Entity* Entity_1;
	Entity* Entity_2;

	CVector3f	Normal;
	float		Distance;


};

