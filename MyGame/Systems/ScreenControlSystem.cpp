#include "ScreenControlSystem.h"
#include "..\Events\KeyboardInputEvent.h"
#include <winuser.h>
#include "..\ECS\World.h"

void ScreenControlSystem::InitialiseSystem()
{
}

void ScreenControlSystem::Update(float dt)
{
}

void ScreenControlSystem::OnEvent(IEvent* _event)
{
	if (_event->Type != KeyboardInput) return;

	auto input = static_cast<KeyboardInputEvent*>(_event);
	if (input->STATE == Down) return;

	switch (input->w_param)
	{
		case VK_ESCAPE: //on escape key down
			switch (_world->GetState()){
			case Play: 
				_world->SetState(Pause);
				break;

			case Pause:
			case MainMenu:
			case GameOver:
				PostQuitMessage(0);
				break;
			}
			break;

		case VK_RETURN: //on enter key down
			switch (_world->GetState()){
			case Pause:
				_world->SetState(Play);
				break;

			case MainMenu:
				_world->SetState(Play);
				break;

			case GameOver:
				_world->SetState(MainMenu);
				break;
			}
			break;
	}
}
