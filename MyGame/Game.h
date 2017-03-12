#pragma once

#define _USE_MATH_DEFINES								// Will allow certain constants (like M_PI) to be availabe from <math.h>

#include <windows.h>									// Header File For the Windows Library
#include <stdio.h>										// Header File For the Standard Input/Output Functions
#include <stdlib.h>										// Header File For the Standard Library
#include <math.h>										// Header File For the Math header file
#include ".\include\glew.h"								// Header File For the GLEW Library
#include ".\include\gl.h"								// Header File For the OpenGL32 Library
#include ".\include\glu.h"								// Header File For the GLu32 Library

// Includes for game objects
#include "Gamewindow.h"
#include "Objects/HighResolutionTimer.h"

#include "ECS\World.h"
#include "Factory.h"


class Game {
public:

private:
	void Initialise();		// Initialise the game world and register all systems
	void Update();			// Update the world
	void GameLoop();		// Main game loop

	World*		_world;
	Factory*	_factory;

	CHighResolutionTimer m_highResolutionTimer;		// A timer for measuring the time between frames

	double m_dt;									// A variable to measure the amount of time elasped between frames

public:
	~Game();
	static Game& GetInstance();
	WPARAM Execute();
	void SetHinstance(HINSTANCE hinstance);
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);
	

private:
	Game::Game();
	Game::Game(const Game&);
	void Game::operator=(const Game&);

	GameWindow m_GameWindow;
	HINSTANCE m_hinstance;
};