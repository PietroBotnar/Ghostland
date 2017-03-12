#include "LightingSystem.h"
#include "..\include\glew.h"
#include "..\ECS\World.h"
#include "..\Events\TorchToggleEvent.h"
#include "..\GameWindow.h"

void LightingSystem::InitialiseSystem()
{
	//init lighthouse direction
	lighthouseDirection = CVector3f(0.0f, 0.0f, 1.0f);

	//save player transform for torch light
	auto player = _world->EntityManager->GetTaggedEntity(PlayerTag);
	playerTransform = player->GetTransform();

	//create text to display torch stauts
	_torchOn = false;
	_torchTxt = _world->CreateEntity()->AddText("TORCH", GameWindow::SCREEN_WIDTH - 100, GameWindow::SCREEN_HEIGHT - 20, 0, 0, 0);
	UpdateTorchDisplay();
}

void LightingSystem::Update(float dt)
{
	if (!_playing) return; 

	PlayerTorch();

	DefaultLight();

	LightHouse();
	lighthouseDirection = lighthouseDirection.RotateAboutAxis(20 * dt, CVector3f(0, 1, 0));
}

void LightingSystem::OnEvent(IEvent* _event)
{
	switch (_event->Type)
	{
		case Torch: 
		{
			if (!_torchOn) {
				_torchOn = true;
			}
			else {
				glDisable(GL_LIGHT2);
				_torchOn = false;
			}
			UpdateTorchDisplay();
		}
		break;
	}
}

void LightingSystem::OnEnterGameState(GameState state)
{
	if (state != Play) 
	{
		glDisable(GL_LIGHTING);
		_playing = false;
	}
	else
	{
		glEnable(GL_LIGHTING);
		_playing = true;
	}
}

void LightingSystem::PlayerTorch()
{
	if(!_torchOn)
		return;

	GLfloat ambientLight0[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularLight0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat positionLight0[] = { playerTransform->Position.x, playerTransform->Position.y, playerTransform->Position.z, 1.0f };
	GLfloat directionLight[] = { playerTransform->Direction.x, playerTransform->Direction.y, playerTransform->Direction.z, 0.0f };

	glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight0);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight0);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight0);
	glLightfv(GL_LIGHT2, GL_POSITION, positionLight0);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, directionLight);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 20.0);// set cutoff angle
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 1); // set focusing strength 
	
	glEnable(GL_LIGHT2);
	glEnable(GL_NORMALIZE);
}

void LightingSystem::DefaultLight()
{
	glEnable(GL_LIGHTING);

	GLfloat ambientLight0[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat specularLight0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat positionLight0[] = { 0.0f, 1.0f, 1.0f, 0.0f };
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambientLight0);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuseLight0);
	glLightfv(GL_LIGHT3, GL_SPECULAR, specularLight0);
	glLightfv(GL_LIGHT3, GL_POSITION, positionLight0);

	glEnable(GL_LIGHT3);
	glEnable(GL_NORMALIZE);
}

void LightingSystem::LightHouse()
{
	GLfloat ambientLight1[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat diffuseLight1[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat specularLight1[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat positionLight1[] = { 0, 20, 0, 1.0f };
	GLfloat directionLight1[] = { lighthouseDirection.x, lighthouseDirection.y, lighthouseDirection.z, 0.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight1);
	glLightfv(GL_LIGHT0, GL_POSITION, positionLight1);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, directionLight1);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 35.0);// set cutoff angle
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1); // set focusing strength 

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	GLfloat ambientLight2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuseLight2[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	GLfloat specularLight2[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	GLfloat positionLight2[] = { 0, 20, 0, 1.0f };
	GLfloat directionLight2[] = { -lighthouseDirection.x, lighthouseDirection.y, -lighthouseDirection.z, 0.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight2);
	glLightfv(GL_LIGHT1, GL_POSITION, positionLight2);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, directionLight2);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 35.0);// set cutoff angle
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1); // set focusing strength 

	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
}

void LightingSystem::UpdateTorchDisplay()
{
	auto text = _torchTxt->GetText();
	//text->text = _torchOn ? "TORCH: ON" : "TORCH: OFF";
	text->r = _torchOn ? 0 : 1;
	text->g = _torchOn ? 1 : 0;
}
