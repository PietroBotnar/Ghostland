#pragma once
#include "..\ECS\IEvent.h"
#include "..\ECS\EventTypes.h"

class DisplayTextEvent : public IEvent
{
public:
	explicit DisplayTextEvent(char* text) : IEvent(TextUI)
	{
		Text	= text;
		x		= 15;
		y		= 25;
		color	= CVector3f();
	};

	DisplayTextEvent(char* text, int xpos, int ypos, CVector3f rgb) : IEvent(TextUI)
	{
		Text = text;
		x = xpos;
		y = ypos;
		color = rgb;
	};

	~DisplayTextEvent(){};

	char* Text;

	int x;
	int y;
	CVector3f color;
};

