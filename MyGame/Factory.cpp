#include "Factory.h"

void Factory::Update(float dt)
{
	LightHouseLight->GetTransform()->Rotate(20 * dt, CVector3f(0, 1, 0));
}

Entity* Factory::CreatePlayer(CVector3f position)
{
	auto player =  World->CreateEntity()
					->AddMeshAsset(Player)
					->AddTransform(position, CVector3f(0, 0, 1), CVector3f(5, 5, 5))
					->AddBoxCollider(CVector3f(4.5f,4.5f,4.5f))
					->AddHitPoints(0, 100, 100)
					->AddGuns(1, 5)
					->AddScore()
					->AddGrenades()
					->AddWaveCount();

	World->TagEntity(player->ID(), PlayerTag);
	return player;
}

Entity* Factory::CreatePlayerBullet(CVector3f position)
{
	auto bullet = World->CreateEntity()
					->AddTransform(position, CVector3f(), CVector3f())
					->AddPrefab(PlayerBullet)
					->AddBoxCollider(CVector3f(2,2,2))
					->AddComposition(BulletsGroup);

	return bullet;
}

Entity* Factory::CreateGhost(CVector3f position, CVector3f direction, MeshID mesh)
{
	//fix height
	position.y = 6;

	auto ghost = World->CreateEntity()
					->AddMeshAsset(mesh)
					->AddTransform(position, direction, CVector3f(1, 1, 1))
					->AddMovement(CVector3f(0, 0, 1))
					->AddBoxCollider(CVector3f(6, 10, 6))
					->AddComposition(GhostsGroup)
					->AddTimer(5);

	return ghost;
}

Entity* Factory::CreateHpBonusPickup(CVector3f position)
{
	//fix height
	position.y = 0;

	auto bonus = World->CreateEntity()
		->AddTransform(position, CVector3f(0, 0, 1), CVector3f(5, 5, 5))
		->AddPrefab(Prism)
		->AddBoxCollider(CVector3f(10, 10, 10))
		->AddComposition(Pickup)
		->AddComposition(HealthBonus);

	return bonus;
}

Entity* Factory::CreateGunComboPickup(CVector3f position)
{
	//fix height
	position.y = 5;

	auto bonus = World->CreateEntity()
		->AddTransform(position, CVector3f(0, 0, 1), CVector3f(3, 3, 3))
		->AddPrefab(Octahedron)
		->AddBoxCollider(CVector3f(5, 5, 5))
		->AddComposition(Pickup)
		->AddComposition(GunComboBonus);

	return bonus;
}

Entity* Factory::CreateGrenadePickup(CVector3f position)
{
	//fix height
	position.y = 0;

	auto bonus = World->CreateEntity()
		->AddTransform(position, CVector3f(0, 0, 1), CVector3f(3, 5, 3))
		->AddPrefab(Pyramid)
		->AddBoxCollider(CVector3f(5, 10, 5))
		->AddComposition(Pickup)
		->AddComposition(GrenadePickup);

	return bonus;
}

Entity* Factory::CreateGrenade(CVector3f position)
{
	auto grenade = World->CreateEntity()
		->AddTransform(position, CVector3f(), CVector3f(0.02f, 0.02f, 0.02f))
		->AddMeshAsset(Grenade)
		->AddBoxCollider(CVector3f(2, 2, 2))
		->AddRigidBody(1, 0.1f, (2.0f / 3.0f), 0.5f, 0.01f)
		->AddComposition(GrenadesGroup)
		->AddTimer(3);

	return grenade;
}

Entity* Factory::CreateForceField(CVector3f position)
{
	auto field = World->CreateEntity()
	->AddTransform(position, CVector3f(), CVector3f())
	->AddBoxCollider(CVector3f(40, 40, 40))
	->AddTimer(10)
	->AddComposition(ForceField);

	return field;
}

void Factory::CreateSceneLevel()
{
	//surrounding walls
	//north
	StaticObjects.push_back(World->CreateEntity()
		->AddTransform(CVector3f(0, 10, 250), CVector3f(0, 0, 0), CVector3f(500, 20, 5))
		->AddPrefab(Wall)
		->AddBoxCollider(CVector3f(500, 20, 5))
		->AddComposition(StaticObjectsGroup));

	//east
	StaticObjects.push_back(World->CreateEntity()
		->AddTransform(CVector3f(245, 10, 0), CVector3f(0, 0, 1), CVector3f(500, 20, 5))
		->AddPrefab(Wall)
		->AddBoxCollider(CVector3f(5, 20, 495))
		->AddComposition(StaticObjectsGroup));

	//south
	StaticObjects.push_back(World->CreateEntity()
		->AddTransform(CVector3f(0, 10, -250), CVector3f(0, 0, 0), CVector3f(500, 20, 5))
		->AddPrefab(Wall)
		->AddBoxCollider(CVector3f(500, 20, 5))
		->AddComposition(StaticObjectsGroup));

	//west
	StaticObjects.push_back(World->CreateEntity()
		->AddTransform(CVector3f(-245, 10, 0), CVector3f(0, 0, 1), CVector3f(500, 20, 5))
		->AddPrefab(Wall)
		->AddBoxCollider(CVector3f(5, 20, 495))
		->AddComposition(StaticObjectsGroup));

	//inner walls 1

	StaticObjects.push_back(World->CreateEntity()
		->AddTransform(CVector3f(0, 10, 200), CVector3f(0, 0, 0), CVector3f(200, 20, 5))
		->AddPrefab(Wall)
		->AddBoxCollider(CVector3f(200, 20, 5))
		->AddComposition(StaticObjectsGroup));

	StaticObjects.push_back(World->CreateEntity()
		->AddTransform(CVector3f(0, 10, -200), CVector3f(0, 0, 0), CVector3f(200, 20, 5))
		->AddPrefab(Wall)
		->AddBoxCollider(CVector3f(200, 20, 5))
		->AddComposition(StaticObjectsGroup));

	StaticObjects.push_back(World->CreateEntity()
		->AddTransform(CVector3f(-195, 10, 0), CVector3f(0, 0, 1), CVector3f(200, 20, 5))
		->AddPrefab(Wall)
		->AddBoxCollider(CVector3f(5, 20, 195))
		->AddComposition(StaticObjectsGroup));

	StaticObjects.push_back(World->CreateEntity()
		->AddTransform(CVector3f(195, 10, 0), CVector3f(0, 0, 1), CVector3f(200, 20, 5))
		->AddPrefab(Wall)
		->AddBoxCollider(CVector3f(5, 20, 195))
		->AddComposition(StaticObjectsGroup));

	//inner walls 2

	StaticObjects.push_back(World->CreateEntity()
		->AddTransform(CVector3f(-100.0f, 7.5f, 150.0f), CVector3f(0, 0, 0), CVector3f(80, 15, 5))
		->AddPrefab(Wall)
		->AddBoxCollider(CVector3f(80, 10, 5))
		->AddComposition(StaticObjectsGroup));

	StaticObjects.push_back(World->CreateEntity()
		->AddTransform(CVector3f(80.0f, 7.5f, 150.0f), CVector3f(0, 0, 0), CVector3f(80, 15, 5))
		->AddPrefab(Wall)
		->AddBoxCollider(CVector3f(80, 10, 5))
		->AddComposition(StaticObjectsGroup));

	StaticObjects.push_back(World->CreateEntity()
		->AddTransform(CVector3f(-100.0f, 7.5f, -150.0f), CVector3f(0, 0, 0), CVector3f(80, 15, 5))
		->AddPrefab(Wall)
		->AddBoxCollider(CVector3f(80, 10, 5))
		->AddComposition(StaticObjectsGroup));

	StaticObjects.push_back(World->CreateEntity()
		->AddTransform(CVector3f(80.0f, 7.5f, -150.0f), CVector3f(0, 0, 0), CVector3f(80, 15, 5))
		->AddPrefab(Wall)
		->AddBoxCollider(CVector3f(80, 10, 5))
		->AddComposition(StaticObjectsGroup));

	StaticObjects.push_back(World->CreateEntity()
		->AddTransform(CVector3f(-147.5f, 7.5f, -100.0f), CVector3f(0, 0, 1), CVector3f(80, 15, 5))
		->AddPrefab(Wall)
		->AddBoxCollider(CVector3f(5, 10, 80))
		->AddComposition(StaticObjectsGroup));

	StaticObjects.push_back(World->CreateEntity()
		->AddTransform(CVector3f(-147.5f, 7.5f, 80.0f), CVector3f(0, 0, 1), CVector3f(80, 15, 5))
		->AddPrefab(Wall)
		->AddBoxCollider(CVector3f(5, 10, 80))
		->AddComposition(StaticObjectsGroup));

	StaticObjects.push_back(World->CreateEntity()
		->AddTransform(CVector3f(147.5f, 7.5f, -80.0f), CVector3f(0, 0, 1), CVector3f(80, 15, 5))
		->AddPrefab(Wall)
		->AddBoxCollider(CVector3f(5, 10, 80))
		->AddComposition(StaticObjectsGroup));

	StaticObjects.push_back(World->CreateEntity()
		->AddTransform(CVector3f(147.5f, 7.5f, 80.0f), CVector3f(0, 0, 1), CVector3f(80, 15, 5))
		->AddPrefab(Wall)
		->AddBoxCollider(CVector3f(5, 10, 80))
		->AddComposition(StaticObjectsGroup));

	//lighthouse
	StaticObjects.push_back(World->CreateEntity()->AddTransform(CVector3f(), CVector3f(0, 0, 1), CVector3f(15, 5, 15))
		->AddMeshAsset(Lighthouse)
		->AddBoxCollider(CVector3f(5, 10, 5))
		->AddComposition(StaticObjectsGroup));

	LightHouseLight = World->CreateEntity()
			->AddTransform(CVector3f(0, 0, 0), CVector3f(0, 0, 1), CVector3f(15, 5, 15))
			->AddMeshAsset(LighhouseLights);
}

