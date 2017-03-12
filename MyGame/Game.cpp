/* 
OpenGL Template for IN3026
City University London, School of Informatics
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Christy Quinn, Sam Kellett, and others

 For educational use by School of Informatics, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, mesh loader, sipmle physics, texturing, audio

 Potential ways to modify the code:  Add new geometry types, change the terrain, load new meshes, set up lighting, 
 add in additional music / sound FX, modify camera control, put in a startup screen, collision detection, etc.
 
 Template version 3.0a 25/08/2014
 Dr. Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 
*/


#include "Game.h"
#include "Systems/WorldRenderSystem.h"
#include "Systems/UIRenderSystem.h"
#include "Systems/CameraSystem.h"
#include "Systems/AudioSystem.h"
#include "Events/SoundEvent.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/PlayerControllerSystem.h"
#include "Systems/FXRenderSystem.h"
#include "Events/FogFXEvent.h"
#include "Events/AnimationFXEvent.h"
#include "Events/CrossfadeFXEvent.h"
#include "Events/AlphaSpereFXEvent.h"
#include "Systems/LightingSystem.h"
#include "Events/TorchToggleEvent.h"
#include "Systems/CollisionDetectionSystem.h"
#include "Systems/GameplayReactionSystem.h"
#include "Systems/AISystem.h"
#include "Events/KeyboardInputEvent.h"
#include "Systems/ScreenControlSystem.h"
#include "ECS/GameFSM.h"

// Constructor.  
Game::Game()  
{
	m_dt = 0.0f;
}

// Destructor.
Game::~Game() 
{
	delete _world;
}

// Initialise the game by loading assets and setting up the scene.  Note, it's best to make OpenGL calls here since we know the rendering context is available.
void Game::Initialise() 
{
	_world		= new World();
	_factory	= new Factory(_world);

	_world->Factory = _factory;

	//register systems here
	_world->RegisterSystem(new CameraSystem());
	_world->RegisterSystem(new LightingSystem());
	_world->RegisterSystem(new WorldRenderSystem());

	auto cd = new CollisionDetectionSystem();
	_world->RegisterSystem(cd);

	_world->RegisterSystem(new PhysicsSystem());
	_world->RegisterSystem(new GameplayReactionSystem());

	auto ai = new AISystem();
	ai->CollsionDetectionSys = cd;

	_world->RegisterSystem(ai);
	_world->RegisterSystem(new PlayerControllerSystem());
	_world->RegisterSystem(new FXRenderSystem());
	_world->RegisterSystem(new AudioSystem());
	_world->RegisterSystem(new ScreenControlSystem());

	auto uiRender = new UIRenderSystem();
	uiRender->Create(m_GameWindow.GetHdc(), "Arial", 18);

	_world->RegisterSystem(uiRender);
	
	_factory->CreatePlayer(CVector3f(0.0f, 2.0f, -100.0f));

	_factory->CreateSceneLevel();

	_world->Initialise();

	//start the game at the main menu screen
	_world->SetState(MainMenu);
}

// Update world and game window
void Game::Update() 
{
	_factory->Update(m_dt);
	_world->Update(m_dt);

	m_GameWindow.UpdateFrameRate(m_dt);
	SwapBuffers(m_GameWindow.GetHdc());
}

void Game::GameLoop()
{
	// This code implements a variable timer
	m_highResolutionTimer.Start();
	Update();
	
	m_dt = m_highResolutionTimer.Elapsed();
}

WPARAM Game::Execute() 
{
	m_GameWindow.Init(m_hinstance);

	if(!m_GameWindow.GetHdc()) {
		return 1;
	}

	Initialise();

	m_highResolutionTimer.Start();

	double frame_duration = 1.0 / (double) 30.0;  // 30 FPS

	MSG msg;

	while(1) {													
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
			if(msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		}else{
			GameLoop();
		} 
	}

	m_GameWindow.Deinit();

	return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
	LRESULT result = 0;

	switch (message) {
	case WM_SIZE:
		if (!m_GameWindow.GetFullScreen()) {
			m_GameWindow.SizeOpenGLScreen(LOWORD(l_param), HIWORD(l_param));

			RECT dimensions;
			GetClientRect(window, &dimensions);
			m_GameWindow.SetDimensions(dimensions);
		}
		break;

	case WM_KEYDOWN:
		_world->SendEvent(new KeyboardInputEvent(Down, w_param, l_param));
		break;
	case WM_KEYUP:
		_world->SendEvent(new KeyboardInputEvent(Up, w_param, l_param));
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;
		
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

Game& Game::GetInstance() 
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance) 
{
	m_hinstance = hinstance;
}


LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{
	Game &game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return game.Execute();
}
