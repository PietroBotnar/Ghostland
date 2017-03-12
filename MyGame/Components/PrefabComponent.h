#pragma once
#include "..\ECS\IComponent.h"
#include "..\Data\Enums.h"

//Defines how to render an object
class PrefabComponent : public IComponent
{
public:
	PrefabComponent(PrefabType type)
	{
		Type = type;
	};

	~PrefabComponent(){};
	
	PrefabType Type;
};

