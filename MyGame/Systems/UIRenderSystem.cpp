#include "UIRenderSystem.h"
#include "..\ECS\ComponentType.h"
#include "..\ECS\ISystem.h"
#include "..\ECS\World.h"
#include "..\ECS\IFiniteStateMachine.h"
#include "..\GameWindow.h"
#include "..\Events\DisplayTextEvent.h"
#include "..\Objects\Texture.h"
#include "..\ECS\GameFSM.h"

void UIRenderSystem::InitialiseSystem()
{
	//load main menu texture
	CTexture menu;
	menu.Load("Resources\\Textures\\menu.bmp", false);
	_mainMenuTexture = menu.m_textureID;

	_textFlash = Timer(0.5f);
	_textFlash.Activate();
}

void UIRenderSystem::OnEnterGameState(GameState state)
{
}

void UIRenderSystem::OnExitGameState(GameState state)
{

}

void UIRenderSystem::Update(float dt)
{
	switch (_world->GetState())
	{
	case MainMenu:
		RenderMainMenu(dt);
		break;
	case Play:
		RenderHeadsUpScreen();
		break;
	case Pause:
		RenderPauseScreen();
		break;
	case GameOver:
		RenderGameOver();
		break;
	}
}

void UIRenderSystem::RenderMainMenu(float dt)
{
	RenderMainMenuBackground();

	if (_textFlash.UpdateTimer(dt)) {
		_flash = !_flash;
		_textFlash.Activate();
	}

	if(!_flash) Render("PRESS ENTER TO PLAY", 500, 250, 1, 0.2, 0);

	DisplayControls();
}

void UIRenderSystem::RenderPauseScreen()
{
	Render("GAME PAUSED", GameWindow::SCREEN_WIDTH / 2 - 100, GameWindow::SCREEN_HEIGHT / 2, 1, 1, 0);
	Render("(press ENTER to continue)", GameWindow::SCREEN_WIDTH / 2 - 100, GameWindow::SCREEN_HEIGHT / 2 + 25, 1, 1, 0);
	Render("(press ESC to exit game)", GameWindow::SCREEN_WIDTH / 2 - 100, GameWindow::SCREEN_HEIGHT / 2 + 50 , 1, 1, 0);

	DisplayControls();
}

void UIRenderSystem::RenderGameOver()
{
	auto player = _world->EntityManager->GetTaggedEntity(PlayerTag);
	char result[128];
	sprintf_s(result, "WAVES SURVIVED: %d \nKILLS: %d\n", player->GetWaveCount()->Value(), player->GetScore()->Value());

	Render("GAME OVER",								GameWindow::SCREEN_WIDTH / 2 - 200, GameWindow::SCREEN_HEIGHT / 2, 1, 0, 0);
	Render(result,									GameWindow::SCREEN_WIDTH / 2 - 200, GameWindow::SCREEN_HEIGHT / 2 + 25, 1, 0, 0);
	Render("(press ENTER to go to main screen)",	GameWindow::SCREEN_WIDTH / 2 - 200, GameWindow::SCREEN_HEIGHT / 2 + 50, 1, 0, 0);
	Render("(press ESC to exit game)",				GameWindow::SCREEN_WIDTH / 2 - 200, GameWindow::SCREEN_HEIGHT / 2 + 75, 1, 0, 0);
}

void UIRenderSystem::RenderHeadsUpScreen()
{
	for (auto& entity : _world->EntityManager->_entities)
	{
		if (entity->Matches(Text))
		{
			auto text = entity->GetText();
			DisplayText(text);
		}
	}
}

void UIRenderSystem::RenderMainMenuBackground()
{
	// Set the OpenGL state and go to 2D mode
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, _mainMenuTexture);
	SetOrthographicProjection();
	glLoadIdentity();

	glColor4f(1,1,1, 0);

	// Render full screen quad
	int w = GameWindow::SCREEN_WIDTH;
	int h = GameWindow::SCREEN_HEIGHT;
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex3f(0, 0, 0);

	glTexCoord2f(0, -1);
	glVertex3f(0, h, 0);

	glTexCoord2f(1, -1);
	glVertex3f(w, h, 0);

	glTexCoord2f(1, 0);
	glVertex3f(w, 0, 0);

	glEnd();

	// Go back to 3D mode (perspective projection)
	ResetPerspectiveProjection();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
}

void UIRenderSystem::DisplayControls()
{
	Render("Controls:", 800, 500 + 25, 1, 0, 0);
	Render("Move:........W A S D or ARROWS", 800, 500 + 50, 0, 1, 0);
	Render("Shoot:.......SPACE BAR", 800, 500 + 75, 0, 1, 0);
	Render("Grenade:...F (hold to charge)", 800, 500 + 100, 0, 1, 0);
	Render("Torch:.......E (toggle)", 800, 500 + 125, 0, 1, 0);
	Render("Sprint:.......SHIFT (hold)", 800, 500 + 150, 0, 1, 0);
	Render("Pause:......ESC", 800, 500 + 175, 0, 1, 0);
}

void UIRenderSystem::Render(std::string text, float x, float y, float r, float g, float b)
{
	glColor3f(r, g, b);
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	SetOrthographicProjection();
	glLoadIdentity();
	RenderBitmapString(x, y, strdup(text.c_str()));
	ResetPerspectiveProjection();
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

void UIRenderSystem::Create(HDC hdc, char *fontName, int size)
{
	// Create a Windows font
	HFONT font = ::CreateFont(-size, 0, 0, 0, size, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, fontName);

	// Select into device context and call wglUseFontBitmaps
	HFONT fontOld = (HFONT)SelectObject(hdc, font);
	wglUseFontBitmaps(hdc, 0, 255, 1000);
}

void UIRenderSystem::OnEvent(IEvent* _event)
{
	//events here
	
	if (_event->Type == TextUI) 
	{
		auto textEvent = static_cast<DisplayTextEvent*>(_event);
		Render(textEvent->Text, textEvent->x, textEvent->y, textEvent->color.x, textEvent->color.y, textEvent->color.z);
	}
}

void UIRenderSystem::SetOrthographicProjection()
{
	int w = GameWindow::SCREEN_WIDTH;
	int h = GameWindow::SCREEN_HEIGHT;

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, w, 0, h);
	// invert the y axis, down is positive
	glScalef(1.0f, -1.0f, 1.0f);
	// move the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -h, 0);
	glMatrixMode(GL_MODELVIEW);
}

void UIRenderSystem::ResetPerspectiveProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void UIRenderSystem::RenderBitmapString(float x, float y, char *string)
{
	glRasterPos2f(x, y);
	glListBase(1000);
	glCallLists(strlen(string), GL_UNSIGNED_BYTE, string);
}

void UIRenderSystem::DisplayText(TextComponent* text)
{
	RenderText(text);
}

void UIRenderSystem::RenderText(TextComponent* text)
{
	Render(text->text, text->x, text->y, text->r, text->g, text->b);
}
