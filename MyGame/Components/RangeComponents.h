#pragma once
#include "..\ECS\IComponent.h"
#include "..\Data\Range.h"
#include <limits.h>

//Hit points range
class HitPointsComponent : public IComponent, public Range
{
public:
	explicit HitPointsComponent(int min, int value, int max) : Range(min, value, max) {}
	~HitPointsComponent() {}
};

//Guns range
class GunComponent : public IComponent, public Range
{
public:
	explicit GunComponent(int min, int max) : Range(min, min, max) {}
	~GunComponent() {}
};

//Score range
class ScoreComponent : public IComponent, public Range
{
public:
	explicit ScoreComponent() : Range(0, 0, INT_MAX) {}
	~ScoreComponent(){}
};

//Grenades range
class GrenadesComponent : public IComponent, public Range
{
public:
	explicit GrenadesComponent() : Range(0, 0, 5) {}
	~GrenadesComponent() {}
};

//Waves survived range
class WavesCountComponent : public IComponent, public Range
{
public:
	explicit WavesCountComponent() : Range(0, 0, INT_MAX) {}
	~WavesCountComponent() {}
};