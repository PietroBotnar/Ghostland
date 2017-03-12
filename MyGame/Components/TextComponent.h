#pragma once
#include "..\ECS\IComponent.h"
#include <string>

//Defines a text rendered on the screen
class TextComponent : public IComponent
{
public:
	explicit TextComponent(std::string text, float x, float y, float r, float g, float b)
	{
		this->text = text;
		this->x = x;
		this->y = y;
		this->r = r;
		this->g = g;
		this->b = b;
	}

	~TextComponent(){};

	std::string text;

	float x, y, r, g, b;
};

