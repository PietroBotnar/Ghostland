#pragma once
#include "..\ECS\IFiniteStateMachine.h"

enum GameState 
{
	MainMenu,
	Play,
	Pause,
	GameOver
};

//FSM of the game state
class GameFSM : public IFiniteStateMachine<GameState>
{
public:
	GameFSM(){};
	~GameFSM(){};

	virtual void EnterState(GameState state) override{};


	virtual void UpdateState(GameState state) override{};


	virtual void ExitState(GameState state) override{};

};

