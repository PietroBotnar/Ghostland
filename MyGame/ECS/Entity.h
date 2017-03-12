#pragma once
#include "ComponentType.h"
#include "IComponent.h"
#include "../Components/MeshAssetComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/TextComponent.h"
#include "../Components/PrefabComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Data/Vector3f.h"
#include <string>
#include <map>
#include "../Components/RangeComponents.h"
#include "../Components/TimerComponent.h"


//Represents an object that exists in the World identified by an ID
class Entity
{
public:
	explicit Entity(unsigned int id);;
	~Entity();

	int				ID()		const { return _id; }
	bool			IsAlive()	const { return _alive; }

	void			SetAlive(bool alive) { _alive = alive; }			
	bool			HasComponent(ComponentType type) const;
	void			RemoveComponent(ComponentType type);
	bool			Matches(Composition composition) const;
	bool			Equals(Entity* other);
	void			Destroy();

	Entity*			AddComposition(ComponentType tag);
	void			RemoveTag(ComponentType tag);

	//add/get components

	Entity*				AddTransform(CVector3f position, CVector3f direction, CVector3f scale);
	TransformComponent*	GetTransform();

	Entity*				AddMovement(CVector3f velocity);
	MovementComponent*	GetMovement();

	Entity*				AddText(std::string text, float x, float y, float r, float g, float b);
	TextComponent*		GetText();

	Entity*				AddPrefab(PrefabType type);
	PrefabComponent*	GetPrefab();

	Entity*				AddRigidBody(float radius, float mass, float rotationInertia, float restitution, float contactTime);
	RigidBodyComponent* GetRigidBody();

	Entity*				AddMeshAsset(MeshID id);
	MeshAssetComponent* GetMeshAsset();

	Entity*				AddBoxCollider(CVector3f size);
	BoxColliderComponent* GetBoxCollider();

	Entity*				AddHitPoints(int min, int value, int max);
	HitPointsComponent* GetHitPoints();

	Entity*				AddGuns(int min, int max);
	GunComponent*		GetGuns();

	Entity*				AddScore();
	ScoreComponent*		GetScore();

	Entity*				AddTimer(int trigger);
	TimerComponent*		GetTimer();

	Entity*				AddGrenades();
	GrenadesComponent*	GetGrenades();

	Entity*				AddWaveCount();
	WavesCountComponent*GetWaveCount();

	//template methods for new components
	//Entity* AddName(values);
	//Component* GetName();

private:
	unsigned int	_id;
	bool			_alive = false;
	Composition     _composition = 0;

	std::map<ComponentType, IComponent*> components;

	void AddComponent(IComponent* component, ComponentType type);

	template<typename T>
	T* GetComponent(ComponentType type);
};
