#pragma once

//[flags] Defines the enum type of the component
enum ComponentType
{
	Empty		= 1 << 0,

	//components
	Movement	= 1 << 1,
	Transform	= 1 << 2,
	Text		= 1 << 3,
	Prefab  = 1 << 4,
	BoxCollider	= 1 << 5,
	RigidBody   = 1 << 6,
	MeshAsset	= 1 << 7,
	Guns		= 1 << 8,	
	HitPoints	= 1 << 9,	

	//groups
	StaticObjectsGroup = 1 << 10,
	GhostsGroup		= 1 << 11,
	BulletsGroup	= 1 << 12,
	GrenadesGroup	= 1 << 13,

	//tags
	PlayerTag		= 1 << 14,
	
	//pickups
	Pickup			= 1 << 15,
	HealthBonus		= 1 << 16,
	GunComboBonus	= 1 << 17,
	GrenadePickup	= 1 << 18,

	Score			= 1 << 19,
	TimerC			= 1 << 20,
	ForceField		= 1 << 21,
	Grenades		= 1 << 22,
	WaveCount		= 1 << 23,
};

typedef int Composition;

class ComponentTypes
{
public:

	static bool Matches(ComponentType value, ComponentType key)
	{
		return (value & key) == key;
	};

	static bool Matches(Composition value, Composition key)
	{
		return (value & key) == key;
	};

	static bool Matches(Composition value, ComponentType key)
	{
		return (value & key) == key;
	};
};
