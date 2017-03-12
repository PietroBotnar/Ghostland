#pragma once
#include "..\ECS\IEvent.h"
#include <wtypes.h>

enum KeyState
{
	Up,
	Down,
};

//Keyboard input event received from the window instance
class KeyboardInputEvent :	public IEvent
{
public:
	KeyboardInputEvent(KeyState state, WPARAM wparam, LPARAM lparam) : IEvent(KeyboardInput)
	{
		STATE = state;
		w_param = wparam;
		l_param = lparam;
	}

	~KeyboardInputEvent(){};

	KeyState STATE;

	WPARAM w_param; 
	LPARAM l_param;
};

