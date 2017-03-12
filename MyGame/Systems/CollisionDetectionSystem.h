#pragma once
#include "..\ECS\ISystem.h"
#include "..\Data\Vector3f.h"
#include <vector>
#include <map>
#include "..\Components\BoxColliderComponent.h"
#include "..\ECS\Entity.h"
#include "..\Data\CollisionInfo.h"

//Checks colliding objects and notifies the world on collision
class CollisionDetectionSystem : public ISystem
{
public:
	explicit CollisionDetectionSystem(){};
	~CollisionDetectionSystem(){};

	virtual void InitialiseSystem() override;


	virtual void Update(float dt) override;


	virtual void OnEvent(IEvent* _event) override;

	virtual void OnEnterGameState(GameState state) override;

	bool AABBCollision(CVector3f& postion1, BoxColliderComponent* box1, CVector3f& postion2, BoxColliderComponent* box2, float& collisionDistance, CVector3f& normal);

	bool CollidesStatics(CVector3f& position1, CVector3f& position2, CVector3f& position3, BoxColliderComponent* box, CVector3f& center);

private:
	bool _playing;

	std::map<Entity*, std::vector<Entity*>> _collisions;

	std::vector<CollisionInfo> collisions;

		CVector3f BoxFaces[6] =
	{
		CVector3f(-1,  0,  0),
		CVector3f(1,  0,  0), 
		CVector3f(0, -1,  0), 
		CVector3f(0,  1,  0), 
		CVector3f(0,  0, -1), 
		CVector3f(0,  0,  1), 
	};
};