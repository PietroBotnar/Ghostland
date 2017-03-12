#pragma once
#include "..\ECS\ISystem.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "..\include\glut.h"	
#include <Windows.h>
#include "..\Components\TextComponent.h"
#include "..\Objects\Timer.h"
#include "..\ECS\GameFSM.h"

//Renders UI elements inside the game window
class UIRenderSystem : public ISystem
{
public:
	explicit UIRenderSystem(){};
	~UIRenderSystem(){};

	virtual void InitialiseSystem() override;
	virtual void Update(float dt) override;

	void Render(std::string text, float x, float y, float r, float g, float b);
	void RenderText(TextComponent* text);
	void Create(HDC hdc, char *fontName, int size);

	virtual void OnEvent(IEvent* _event) override;
	virtual void OnEnterGameState(GameState state) override;


	virtual void OnExitGameState(GameState state) override;

private:
	void RenderMainMenu(float dt);
	void RenderPauseScreen();
	void RenderGameOver();
	void RenderHeadsUpScreen();

	UINT _mainMenuTexture;
	void RenderMainMenuBackground();

	void DisplayControls();

	void SetOrthographicProjection();
	void ResetPerspectiveProjection();
	void RenderBitmapString(float x, float y, char *string);

	void DisplayText(TextComponent* text);

	Timer _textFlash;
	bool _flash = false;
};