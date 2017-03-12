#pragma once
#include <vector>
#include <map>

#include "Entity.h"

class World;
class ComponentManager;

//Vector of Entity(s)
typedef std::vector<Entity*> EntityList;

//Manages all Entities present in the World;
//Handles creation, retrieval and deletion of Entities
class EntityManager
{

public:
	explicit EntityManager(World* world) {
		World				= world; 
	}

	~EntityManager();

	Entity* CreateEntity();
	Entity* GetEntity(unsigned int id);

	void	AddTaggedEntity(int entityId, ComponentType tag);
	Entity* GetTaggedEntity(ComponentType tag);
	void	RemoveTag(ComponentType tag);

	Entity* Matcher(int composition);

	//Add dead entity ids to the pool, so they can be reused
	void CleanUp();

	World*				World;

	EntityList	_entities;

private:
	unsigned int _next  = 0;
	unsigned int _count = 0;
	std::vector<unsigned int> _pool;

	std::map<ComponentType, int> _tags;
};