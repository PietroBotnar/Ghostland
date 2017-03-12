#pragma once

//Base class for all components that can be attached to an Entity
class IComponent
{
protected:
	IComponent() = default;

public:
	unsigned int Id;
	unsigned int EntityId;
};