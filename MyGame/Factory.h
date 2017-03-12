#pragma once
#include "ECS\World.h"
#include "Data\Vector3f.h"
#include <vector>

//Builds predefined game objects
class Factory
{
public:

	Factory(World* world)
	{
		World = world;
	}

	void Update(float dt);

	~Factory(){};

	World* World;

	Entity* CreatePlayer(CVector3f position);
	Entity* CreatePlayerBullet(CVector3f position);
	Entity* CreateGhost(CVector3f position, CVector3f direction, MeshID mesh);
	Entity* CreateHpBonusPickup(CVector3f position);
	Entity* CreateGunComboPickup(CVector3f position);
	Entity* CreateGrenadePickup(CVector3f position);
	Entity* CreateGrenade(CVector3f position);
	Entity* CreateForceField(CVector3f position);

	void CreateSceneLevel();

	Entity* LightHouseTop;
	Entity* LightHouseLight;

	std::vector<Entity*> StaticObjects;

};

