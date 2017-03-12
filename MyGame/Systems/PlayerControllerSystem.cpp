#include "PlayerControllerSystem.h"
#include "..\Factory.h"
#include "..\ECS\World.h"
#include <winuser.rh>
#include <winuser.h>
#include "..\Events\DisplayTextEvent.h"
#include "..\Events\SoundEvent.h"
#include "..\Data\Enums.h"
#include "..\Events\TorchToggleEvent.h"
#include "..\Events\KeyboardInputEvent.h"

void PlayerControllerSystem::InitialiseSystem()
{
	Player = _world->EntityManager->GetTaggedEntity(PlayerTag);

	_movementSpeed = WALKING_SPEED;
	_throwingForce = THROW_FORCE_MIN;
}

void PlayerControllerSystem::Update(float dt)
{
	if(!_playing) return;

	KeyDownListeners(dt, Player->GetTransform());

	DisplayThrowingForce();
}

void PlayerControllerSystem::OnEvent(IEvent* _event)
{
	if(!_playing) return;

	if (_event->Type == KeyboardInput) {
		auto input = static_cast<KeyboardInputEvent*>(_event);
		switch (input->w_param)
		{
			case VK_SPACE:
				if (input->STATE == Down)
					Shoot();
				else
					_shot = false;
				break;

			case 'F':
				if (input->STATE == Up && !_grenade && !Player->GetGrenades()->AtMinimum()) {
					ThrowGrenade();
					_grenade = false;
					break;
				}

				if(input->STATE == Down)
					_haveGrenades = !Player->GetGrenades()->AtMinimum();

				break;

			case 'E':
				if(input->STATE == Up) _world->SendEvent(new TorchToggleEvent());
				break;

			case VK_SHIFT:
				_movementSpeed = input->STATE == Down ? RUNNING_SPEED : WALKING_SPEED;
				break;
		}
	}
}

void PlayerControllerSystem::Shoot()
{
	if(_shot) return;

	auto playerT = Player->GetTransform();

	auto position	= playerT->Position;
	auto direction	= playerT->Direction;

	int _guns = Player->GetGuns()->Value();
	
	float spacing = (SHOOTING_ANGLE/_guns)/2;

	//side guns
	for (int i = 1; i <= (_guns/2); i++)
	{
		float mod = i % 2 == 0 ? 1 : -1;
		auto dir = direction.RotateAboutAxis(spacing *i, CVector3f(0,1,0));
		auto dir2 = direction.RotateAboutAxis(spacing *i * -1, CVector3f(0,1,0));

		auto bullet = _world->Factory->CreatePlayerBullet(position + dir * 5);
		auto bullet2 = _world->Factory->CreatePlayerBullet(position + dir2 * 5);

		bullet->AddMovement(dir * 800);
		bullet2->AddMovement(dir2 * 800);
	}

	//mid gun
	if (_guns % 2 != 0) {
		auto bullet3 = _world->Factory->CreatePlayerBullet(position + direction * 5);
		bullet3->AddMovement(direction * 800);
	}

	_world->SendEvent(new SoundEvent(SoundID::PlayerShot));

	_shot = true;
}

void PlayerControllerSystem::ThrowGrenade()
{
	if(_grenade) return;

	auto playerT = Player->GetTransform();

	auto position = playerT->Position + CVector3f(0, 0, 0);
	auto direction = playerT->Direction ;

	auto grenade = _world->Factory->CreateGrenade(position + direction * 2);
	auto grenadeDirection = direction + CVector3f(0.0f, 0.5f, 0.0f);

	auto rigidBody = grenade->GetRigidBody();

	CVector3f force = (_throwingForce) * grenadeDirection;
	CVector3f torque = 200 * CVector3f(1, 0, 0);
	
	rigidBody->AddForce(force);
	rigidBody->AddTorque(torque);

	_throwingForce = THROW_FORCE_MIN;

	grenade->GetTimer()->Activate();

	Player->GetGrenades()->Delta(-1);

	_grenade = true;
}

void PlayerControllerSystem::KeyDownListeners(double dt, TransformComponent* transform)
{
	if(!_playing) return;
	
	// move Forward
	if ((GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80)) {
		transform->Position += (transform->Direction * _movementSpeed * dt);	
	}

	// move Backward
	if ((GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80)) {
		transform->Position -= (transform->Direction * _movementSpeed * dt);
	}

	// turn Left
	if ((GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80)) {
		transform->Rotate(170 * dt, CVector3f(0, 1, 0));
	}

	// turn Right
	if ((GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80)) {
		transform->Rotate(-170 * dt, CVector3f(0, 1, 0));
	}

	// turn Right
	if ((GetKeyState('F') & 0x80) && _throwingForce < THROW_FORCE_MAX && !_grenade && _haveGrenades) {
		_throwingForce += 100 * dt;
	}
}

void PlayerControllerSystem::DisplayThrowingForce()
{
	if(_throwingForce <= THROW_FORCE_MIN)
		return;

	char force[128];
	sprintf_s(force, "%d\n", (int)_throwingForce);
	_world->SendEvent(new DisplayTextEvent(force, 400, 400, CVector3f(0, 0, 1)));
}

void PlayerControllerSystem::OnEnterGameState(GameState state)
{
	_playing = state == Play;
}
