#include "Entity.h"
#include "IComponent.h"
#include "ComponentManager.h"
#include "ComponentType.h"
#include "..\Data\Enums.h"

Entity::Entity(unsigned int id)
{
	_id					= id;
	_alive				= true;
}

Entity::~Entity()
{
	_composition = 0;
}

void Entity::AddComponent(IComponent* component, ComponentType type)
{
	_composition	|= type;
	components[type] = component;
}

template<typename T>
T * Entity::GetComponent(ComponentType type)
{
	if(!HasComponent(type))
		return nullptr;

	return static_cast<T*>(components[type]);
}

bool Entity::HasComponent(ComponentType type) const
{
	return ComponentTypes::Matches(_composition, type);
}

void Entity::RemoveComponent(ComponentType type)
{
	_composition ^= type;
	components.erase(type);
}

bool Entity::Matches(Composition composition) const
{
	if(this == NULL) return false;

	return ComponentTypes::Matches(_composition, composition);
}

bool Entity::Equals(Entity* other)
{
	return _id == other->_id && _composition == other->_composition;
}

void Entity::Destroy()
{
	_alive = false;
}

Entity* Entity::AddComposition(ComponentType tag)
{
	_composition |= tag;

	return this;
}

void Entity::RemoveTag(ComponentType tag)
{
	_composition ^= tag;
}

Entity* Entity::AddTransform(CVector3f position, CVector3f direction, CVector3f scale)
{
	//update value
	if (HasComponent(Transform)) {
		GetTransform()->Position = position;
		return this;
		}

	AddComponent(new TransformComponent(position, direction, scale), Transform);
	return this;
}

TransformComponent* Entity::GetTransform()
{
	return GetComponent<TransformComponent>(Transform);
}

Entity* Entity::AddMovement(CVector3f velocity)
{
	//update value
	if (HasComponent(Movement)) {
		GetMovement()->Set(velocity);
		return this;
	}

	AddComponent(new MovementComponent(velocity), Movement);
	return this;
}

MovementComponent * Entity::GetMovement()
{
	return GetComponent<MovementComponent>(Movement);
}

Entity* Entity::AddText(std::string text, float x, float y, float r, float g, float b)
{
	if (HasComponent(Text)) {
		//update value
		return this;
	};

	AddComponent(new TextComponent(text, x, y, r, g, b), Text);

	return this;
}

TextComponent * Entity::GetText()
{
	return GetComponent<TextComponent>(Text);
}

Entity* Entity::AddPrefab(PrefabType type)
{
	AddComponent(new PrefabComponent(type), Prefab);
	return this;
}

PrefabComponent* Entity::GetPrefab()
{
	return GetComponent<PrefabComponent>(Prefab);
}

Entity* Entity::AddRigidBody(float radius, float mass, float rotationInertia, float restitution, float contactTime)
{
	AddComponent(new RigidBodyComponent(radius, mass, rotationInertia, restitution, contactTime), RigidBody);
	return this;
}

RigidBodyComponent* Entity::GetRigidBody()
{
	return GetComponent<RigidBodyComponent>(RigidBody);
}

Entity* Entity::AddMeshAsset(MeshID id)
{
	AddComponent(new MeshAssetComponent(id), MeshAsset);
	return this;
}

MeshAssetComponent* Entity::GetMeshAsset()
{
	return GetComponent<MeshAssetComponent>(MeshAsset);
}

Entity* Entity::AddBoxCollider(CVector3f size)
{
	AddComponent(new BoxColliderComponent(size), BoxCollider);
	return this;
}

BoxColliderComponent* Entity::GetBoxCollider()
{
	return GetComponent<BoxColliderComponent>(BoxCollider);
}

Entity* Entity::AddHitPoints(int min, int value, int max)
{
	AddComponent(new HitPointsComponent(min, value, max), HitPoints);
	return this;
}

HitPointsComponent* Entity::GetHitPoints()
{
	return GetComponent<HitPointsComponent>(HitPoints);
}

Entity* Entity::AddGuns(int min, int max)
{
	AddComponent(new GunComponent(min, max), Guns);
	return this;
}

GunComponent* Entity::GetGuns()
{
	return GetComponent<GunComponent>(Guns);
}

Entity* Entity::AddScore()
{
	AddComponent(new ScoreComponent(), Score);
	return this;
}

ScoreComponent* Entity::GetScore()
{
	return GetComponent<ScoreComponent>(Score);
}

Entity* Entity::AddTimer(int trigger)
{
	AddComponent(new TimerComponent(trigger), TimerC);
	return this;
}

TimerComponent* Entity::GetTimer()
{
	return GetComponent<TimerComponent>(TimerC);
}

Entity* Entity::AddGrenades()
{
	AddComponent(new GrenadesComponent(), Grenades);
	return this;
}

GrenadesComponent* Entity::GetGrenades()
{
	return GetComponent<GrenadesComponent>(Grenades);
}

Entity* Entity::AddWaveCount()
{
	AddComponent(new WavesCountComponent(), WaveCount);
	return this;
}

WavesCountComponent* Entity::GetWaveCount()
{
	return GetComponent<WavesCountComponent>(WaveCount);
}

