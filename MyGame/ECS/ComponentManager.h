#pragma once

#include <map>
#include <vector>

class IComponent;

typedef std::vector<IComponent*>	ComponentList;

//Manages creation and manipulation of components
class ComponentManager
{
public:
	ComponentManager();
	~ComponentManager();

	unsigned int AddComponent(IComponent* component, int type);

	IComponent* GetComponent(int type, int id);

	void RemoveComponent(int type, int id);

	ComponentList& GetComponents(int type);

private:
	std::map<int, ComponentList> _components;
	std::map<int, unsigned int> _next;
	std::map<int, std::vector<unsigned int>> _pool;
};
