#include "PhysicsSystem.h"
#include "..\ECS\World.h"
#include "..\Components\RigidBodyComponent.h"
#include "..\Events\SoundEvent.h"
#include "..\Data\Enums.h"
#include <xutility>
#include "..\Events\CollisionEvent.h"

void PhysicsSystem::InitialiseSystem()
{
}

void PhysicsSystem::Update(float dt)
{
	if(!_playing)	return;
	
	for (auto &entity : _world->EntityManager->_entities)
	{
		if(!entity->IsAlive() || !entity->Matches(Transform)) continue;

			auto transform = entity->GetTransform();

			if (entity->Matches(RigidBody))
				UpdateRigigBody(transform, entity->GetRigidBody(), dt);

			if (entity->Matches(Movement)) {
				auto movement = entity->GetMovement();
				float y = transform->Position.y;
				transform->Position += (movement->Velocity * dt);
				transform->Position.y = y;
			}
	}
}

bool PhysicsSystem::IsCollisionOnPlane(float ySize, CVector3f position, CVector3f velocity)
{
	// Check for collision with the ground by looking at the y value of the ball's position
	if (position.y - ySize < 0 && velocity.y < 0) {
		
		return true;
	}
	return false;
}


void PhysicsSystem::UpdateRigigBody(TransformComponent* transform, RigidBodyComponent* rigidBody, float dt)
{
	// Update physical quantities

	//apply gravity
	rigidBody->m_acceleration = CVector3f(0.0f, -GRAVITY, 0.0f);

	//update position from velocity
	transform->Position += rigidBody->m_velocity * dt;

	//update rotation angle
	transform->Rotation += rigidBody->m_angularVelocity * dt;

	//update velocity from acceleration
	rigidBody->m_velocity += (rigidBody->m_acceleration + rigidBody->m_instantaneousAcceleration) * dt;

	//update rotation velocity
	rigidBody->m_angularVelocity += (rigidBody->m_angularAcceleration + rigidBody->m_instantaneousAngularAcceleration) * dt;

	// Turn off instantaneous forces if contact time is surpassed
	if (rigidBody->m_instantaneousAcceleration.Length() > 0 && rigidBody->m_contactTime > 0.05) {
		rigidBody->m_instantaneousAcceleration = CVector3f(0, 0, 0);
		rigidBody->m_instantaneousAngularAcceleration = CVector3f(0, 0, 0);
		rigidBody->m_contactTime = 0;
	}
	rigidBody->m_contactTime += dt;

	//chech and respond with planes collision (terrain/walls)
	if (IsCollisionOnPlane(rigidBody->m_radius, transform->Position, rigidBody->m_velocity))
	{
		RespondToPlaneCollision(CVector3f(0, 1, 0), transform->Position, rigidBody);
	}
}

void PhysicsSystem::RespondToPlaneCollision(CVector3f normal, CVector3f& position, RigidBodyComponent* rigidBody)
{
	float convergenceThreshold = 1.0f;
	if (rigidBody->m_velocity.Length() > convergenceThreshold) {

		// The body has bounced, implement a bounce by flipping the velocity on the colliding normal
		rigidBody->m_velocity = rigidBody->m_coefficientOfRestitution*CVector3f(normal.x != 0 ? -1 * rigidBody->m_velocity.x : rigidBody->m_velocity.x, normal.y != 0 ? -1 * rigidBody->m_velocity.y : rigidBody->m_velocity.y, normal.z != 0 ? -1 * rigidBody->m_velocity.z : rigidBody->m_velocity.z);
		rigidBody->m_angularVelocity = rigidBody->m_coefficientOfRestitution*rigidBody->m_angularVelocity;
	}
	else {
		// Velocity of the body is below a threshold, stop it 
		rigidBody->m_velocity = CVector3f(0.0f, 0.0f, 0.0f);
		rigidBody->m_acceleration = CVector3f(0.0f, 0.0f, 0.0f);
		position = CVector3f(position.x, rigidBody->m_radius, position.z);
		rigidBody->m_angularVelocity = CVector3f(0.0f, 0.0f, 0.0f);
	}
}

void PhysicsSystem::OnCollision(CollisionInfo collision)
{
	if (collision.Entity_1->Matches(StaticObjectsGroup))
		StaticObjectCollisionResponse(collision.Entity_1, collision.Entity_2, collision.Normal, collision.Distance);

	if (collision.Entity_2->Matches(StaticObjectsGroup))
		StaticObjectCollisionResponse(collision.Entity_2, collision.Entity_1, collision.Normal, collision.Distance);

	if(collision.Entity_1->Matches(ForceField))
		ForceFieldCollision(collision.Entity_2);

	if (collision.Entity_2->Matches(ForceField))
		ForceFieldCollision(collision.Entity_1);
}

void PhysicsSystem::StaticObjectCollisionResponse(Entity* wall, Entity *colliding, CVector3f normal, float distance)
{
	if(colliding->Matches(GhostsGroup))
		return;

	auto transform = colliding->GetTransform();

	if (colliding->Matches(RigidBody)) {
		RespondToPlaneCollision(normal, transform->Position, colliding->GetRigidBody());
		return;
	}

	float y = transform->Position.y;
	transform->Position += -1 * ((normal * distance));
	transform->Position.y = y;
}

void PhysicsSystem::ForceFieldCollision(Entity* entity)
{
	if(!entity->Matches(GhostsGroup)) return;

	entity->GetMovement()->Velocity *= 0.2f;
}

void PhysicsSystem::OnEvent(IEvent* _event)
{
	if (_event->Type == Collision) {
		auto collisionEvent = static_cast<CollisionEvent*>(_event);
		
		OnCollision(collisionEvent->collisionInfo);
	}
}

void PhysicsSystem::OnEnterGameState(GameState state)
{
	_playing = state == Play;
}
