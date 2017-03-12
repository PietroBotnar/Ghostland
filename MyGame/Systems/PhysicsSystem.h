#pragma once
#include "..\ECS\ISystem.h"
#include <vector>
#include "..\Components\BoxColliderComponent.h"
#include "..\Components\RigidBodyComponent.h"
#include "..\ECS\Entity.h"
#include "..\Data\Vector3f.h"
#include "..\Data\CollisionInfo.h"
#include "..\ECS\GameFSM.h"

struct ColliderBE
{
	unsigned int id;
	float Begin;
	float End;
};


class PhysicsSystem : public ISystem
{
public:
	explicit PhysicsSystem(){};
	~PhysicsSystem(){};

	virtual void InitialiseSystem() override;

	virtual void Update(float dt) override;

	virtual void OnEnterGameState(GameState state) override;

	bool IsCollisionOnPlane(float ySize, CVector3f position, CVector3f velocity);

	void UpdateRigigBody(TransformComponent* transform, RigidBodyComponent* rigidBody, float dt);

	void RespondToPlaneCollision(CVector3f normal, CVector3f& position, RigidBodyComponent* rigidBody);

	void OnCollision(CollisionInfo collision);
	
	void StaticObjectCollisionResponse(Entity* wall, Entity *colliding, CVector3f normal, float distance);

	void ForceFieldCollision(Entity* entity);

	virtual void OnEvent(IEvent* _event) override;

private:
	std::vector<ColliderBE> _colliderBEs;

	std::vector<unsigned int> _checked;

	const float GRAVITY = 9.8f;

	bool _playing = false;
};

