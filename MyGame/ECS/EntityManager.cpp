#include "EntityManager.h"

EntityManager::~EntityManager()
{
	for(auto& entity : _entities)
	{
		delete(entity);
	}
	_next = _count = 0;
}

Entity* EntityManager::CreateEntity()
{
	unsigned int _id;

	//check pool for available ids
	if (_pool.size() > 0) {
		_id = _pool.back();
		_entities.at(_id) = new Entity(_id);
		_pool.pop_back();
	}
	else {
		_id = _next;
		_entities.push_back(new Entity(_id));
		_next++;
	}
	_count++;

	return _entities[_id];
}

Entity* EntityManager::GetEntity(unsigned int id)
{
	try
	{
		return _entities.at(id);
	}
	catch (std::out_of_range o)
	{
		return nullptr;
	}
}

void EntityManager::AddTaggedEntity(int entityId, ComponentType tag)
{
	_tags[tag] = entityId;
}

Entity* EntityManager::GetTaggedEntity(ComponentType tag)
{
	return GetEntity(_tags.at(tag));
}

void EntityManager::RemoveTag(ComponentType tag)
{
	auto it = _tags.find(tag);
	_tags.erase(it);
}

Entity* EntityManager::Matcher(int composition)
{
	return 0;
}

void EntityManager::CleanUp()
{
	for (auto iter = _entities.begin(); iter != _entities.end(); ++iter)
	{
		if ((*iter)->IsAlive()) continue;

		bool alreadyPooled = (std::find(_pool.begin(), _pool.end(), (*iter)->ID()) != _pool.end());
		if(!alreadyPooled)
		{
			_pool.push_back((*iter)->ID());
			_count--;
		}
	}

	//if pool reaches size 100, delete dead entities and reclaim memory
	if (_pool.size() == 100) {
		for (auto iter = _entities.begin(); iter != _entities.end();)
		{
			if(!(*iter)->IsAlive())
				iter = _entities.erase(iter);
			else
				++iter;
		}
	}
}
