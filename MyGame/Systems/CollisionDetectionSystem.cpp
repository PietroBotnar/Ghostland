#include "CollisionDetectionSystem.h"
#include "..\ECS\World.h"
#include "..\ECS\ComponentManager.h"
#include "..\ECS\ComponentType.h"
#include "..\Events\CollisionEvent.h"
#include "..\Data\CollisionInfo.h"
#include "..\Factory.h"

void CollisionDetectionSystem::InitialiseSystem()
{
}

void CollisionDetectionSystem::Update(float dt)
{
	if(!_playing) return;

	collisions.clear();

	int i = 0;
	auto& entities = _world->EntityManager->_entities;
	//find all occuring collisions
	for (auto& entity : entities)
	{
		i++;
		if(!entity->IsAlive() || !entity->Matches(BoxCollider | Transform)) 
			continue;

		auto colliderComponent	= entity->GetBoxCollider();
		auto transformComponent	= entity->GetTransform();
		
		auto _rend = entities.rend();
		std::advance(_rend, -i);

		//iterate backwards excluding last elements to avoid double checking
		for (auto it2 = entities.rbegin(); it2 != _rend; ++it2)
		{
			if (!(*it2)->IsAlive() || !(*it2)->Matches(BoxCollider | Transform)) 
				continue;

			auto _colliderComponent = (*it2)->GetBoxCollider();
			auto _transform			= (*it2)->GetTransform();

			auto normal		= CVector3f();
			float distance	= 0.0f;

			if(AABBCollision(transformComponent->Position, colliderComponent, _transform->Position, _colliderComponent, distance, normal))
				collisions.push_back(CollisionInfo(entity, *it2, normal, distance));
		}
	}

	//send collision events
	for (auto& collision : collisions)
	{
		_world->SendEvent(new CollisionEvent(collision, dt));
	}
}

void CollisionDetectionSystem::OnEvent(IEvent* _event)
{
}

void CollisionDetectionSystem::OnEnterGameState(GameState state)
{
	_playing = state == Play;
}

bool CollisionDetectionSystem::AABBCollision(CVector3f& postion1, BoxColliderComponent* box1, CVector3f& postion2, BoxColliderComponent* box2, float& collisionDistance, CVector3f& normal)
{
	CVector3f min1 = box1->GetMinPoint(postion1);
	CVector3f max1 = box1->GetMaxPoint(postion1);
	CVector3f min2 = box2->GetMinPoint(postion2);
	CVector3f max2 = box2->GetMaxPoint(postion2);

	if (max1.x > min2.x &&
		min1.x < max2.x &&
		max1.y > min2.y &&
		min1.y < max2.y &&
		max1.z > min2.z &&
		min1.z < max2.z)
	{
		// Collision detected

		// distance of collided box to the normal
		float distances[6] =
		{
			(max2.x - min1.x),
			(max1.x - min2.x),
			(max2.y - min1.y),
			(max1.y - min2.y),
			(max2.z - min1.z),
			(max1.z - min2.z),
		};

		//find colliding normal
		for (int i = 0; i < 6; i++)
		{
			if ((i == 0) || (distances[i] < collisionDistance))
			{
				normal				= BoxFaces[i];
				collisionDistance	= distances[i];
			}
		}
		
		return true;
	}

	// No collision detected
	return false;
}

bool CollisionDetectionSystem::CollidesStatics(CVector3f& position1, CVector3f& position2, CVector3f& position3, BoxColliderComponent* box, CVector3f& center)
{
	for (auto& entity : _world->Factory->StaticObjects)
	{
		auto otherBox = entity->GetBoxCollider();
		float d; CVector3f n;

		center = entity->GetTransform()->Position;
		if (AABBCollision(position1, box, center, otherBox, d, n))
			return true;
		if (AABBCollision(position2, box, center, otherBox, d, n))
			return true;
		if (AABBCollision(position3, box, center, otherBox, d, n))
			return true;
	}
	return false;
}
