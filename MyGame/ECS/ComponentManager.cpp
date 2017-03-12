#include "ComponentManager.h"

ComponentManager::ComponentManager()
{
}

ComponentManager::~ComponentManager()
{
}

IComponent* ComponentManager::GetComponent(int type, int id)
{
	return _components[type].at(id);
}

unsigned int ComponentManager::AddComponent(IComponent* component, int type)
{
	if (!_pool[type].empty())
	{
		unsigned int poolId = _pool[type].back();
		_components[type].at(poolId) = component;
		_pool[type].pop_back();
		return poolId;
	}

	unsigned int id = 0;
	if (_next.find(type) != _next.end())
	{
		id = _next[type];
		_next[type] ++;
	}
	else
	{
		_next[type] = 1;
	}

	_components[type].push_back(component);

	return id;
}

void ComponentManager::RemoveComponent(int type, int id)
{
	_pool[type].push_back(id);
	delete _components[type][id];
}

ComponentList& ComponentManager::GetComponents(int type)
{
	return _components[type];
}
