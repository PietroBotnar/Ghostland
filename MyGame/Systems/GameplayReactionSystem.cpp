#include "GameplayReactionSystem.h"
#include "..\Events\CollisionEvent.h"
#include "..\Events\AnimationFXEvent.h"
#include "..\ECS\World.h"
#include "..\Events\SoundEvent.h"
#include <stdio.h>
#include <string>
#include "..\Events\CrossfadeFXEvent.h"
#include "..\Components\TimerComponent.h"
#include <stdlib.h>
#include "..\Factory.h"
#include "..\Events\AlphaSpereFXEvent.h"
#include "..\Events\EnemySpawnEvent.h"
#include "..\Events\FogFXEvent.h"
#include "..\GameWindow.h"

void GameplayReactionSystem::InitialiseSystem()
{
	Player		= _world->EntityManager->GetTaggedEntity(PlayerTag);
	_playerHp	= Player->GetHitPoints();
	_killScore  = Player->GetScore();
	_grenadesCount = Player->GetGrenades();

	_killsTxt = _world->CreateEntity()->AddText("", 20, 20, 0, 1, 0);
	UpdateKillScoreText();

	_healthTxt = _world->CreateEntity()->AddText("", GameWindow::SCREEN_WIDTH - 100, 20, 1, 0, 0);
	UpdateHpText();

	_greandesTxt = _world->CreateEntity()->AddText("", GameWindow::SCREEN_WIDTH / 2 - 50, 20, 0, 0, 1);

	_wave = 0;
	_wavesTxt = _world->CreateEntity()->AddText("", 20, GameWindow::SCREEN_HEIGHT - 20, 1, 1, 1);
	UpdateWavesText();
	
	_nextWaveTimer = Timer(NEXT_WAVE_DELAY);
	_nextWaveTimer.Activate();
}

void GameplayReactionSystem::Update(float dt)
{
	if(!_playing) return;

	auto& entities = _world->EntityManager->_entities;

	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		auto& entity = *it;

		if(!entity->IsAlive()) continue;

		//rotate pickups
		if (entity->Matches(Pickup)) {
			entity->GetTransform()->Rotate(300 * dt, CVector3f(0, 1, 0));
		}

		//update timers
		if (entity->Matches(TimerC)) {
			if (entity->GetTimer()->Update(dt)) {
				
				//respond to timer
				if (entity->Matches(GrenadesGroup)) {
					GrenadeExplosion(entity);
				}
				else if (entity->Matches(ForceField)) {
					entity->Destroy();
				}
			}
		}
	}

	UpdateGrenadesText();

	//trigger next wave
	if(_nextWaveTimer.UpdateTimer(dt))
		NextWave();

	//if player dies, trigger game over
	if(_playerHp->AtMinimum()) GameOver();

}

void GameplayReactionSystem::OnEvent(IEvent* _event)
{
	switch (_event->Type)
	{
	case Collision:
		auto collision = static_cast<CollisionEvent*>(_event);

		auto info = collision->collisionInfo;

		//ignore bullet vs bullet collisions
		if(info.Entity_1->Matches(BulletsGroup) && info.Entity_2->Matches(BulletsGroup))
			return;

		if(info.Entity_1->Matches(BulletsGroup))
			BulletHit(info.Entity_1, info.Entity_2);

		else if (info.Entity_2->Matches(BulletsGroup))
			BulletHit(info.Entity_2, info.Entity_1);

		else if (info.Entity_1->Matches(PlayerTag))
			PlayerHit(info.Entity_2, collision->dt);

		else if (info.Entity_2->Matches(PlayerTag))
			PlayerHit(info.Entity_1, collision->dt);

		break;
	}
}

void GameplayReactionSystem::BulletHit(Entity* bullet, Entity* hit)
{
	//ignore bullet vs pickup hit
	if(hit->Matches(Pickup) || hit->Matches(ForceField)) return;

	auto position = bullet->GetTransform()->Position;
	auto direction = bullet->GetMovement()->Velocity;

	direction.Normalise();

	bullet->Destroy();

	//set animation postion closer to camera
	auto explosionPosition = position + (direction* -1 * 3);

	// explosion fx
	_world->SendEvent(new AnimationFXEvent(SpriteAnimationID::Explosion, explosionPosition, 10, 10));
	_world->SendEvent(new SoundEvent(SoundID::SmallExplosion, position));

	if(hit->Matches(GhostsGroup))
		GhostHit(hit);
}

void GameplayReactionSystem::GhostHit(Entity* ghost)
{
	if(!ghost->IsAlive())
		return;

	auto ghostPosition = ghost->GetTransform()->Position;
	ghost->Destroy();

	_enemies--;

	//trigger sound
	_world->SendEvent(new SoundEvent(SoundID::GhostDeath));

	//check bonus spawn
	BonusSpawn(ghostPosition);

	//update score
	_killScore->Delta(1);
	UpdateKillScoreText();

	//check wave status
	if(_enemies <= 0)
		_nextWaveTimer.Activate();
}

void GameplayReactionSystem::PlayerHit(Entity* hit, float dt)
{
	if(hit->Matches(StaticObjectsGroup))
		return;

	//player hit by the ghost
	if (hit->Matches(GhostsGroup)) {

		//trigger crossfade
		_world->SendEvent(new CrossfadeFXEvent(CVector3f(1, 0, 0)));
		//trigger hit sound
		_world->SendEvent(new SoundEvent(SoundID::PlayerHit));

		elapsedFromHit += dt;

		//reduced hp frame rate independently
		if (elapsedFromHit >= 0.01f) {

			//reduce hp
			_playerHp->Delta(-1);
			elapsedFromHit = 0;
		}

		//reset gun combo
		Player->GetGuns()->ToMinimum();

		UpdateHpText();
	}
	//pickup health bonus
	else if (hit->Matches(HealthBonus)) {
		
		hit->Destroy();

		_world->SendEvent(new CrossfadeFXEvent(CVector3f(0, 1, 0)));
		_world->SendEvent(new SoundEvent(SoundID::HealthPickup));

		//increase HP
		_playerHp->Delta(HEALTH_BONUS);

		UpdateHpText();
	}
	//pickup gun combo bonus
	else if (hit->Matches(GunComboBonus)) {

		hit->Destroy();

		_world->SendEvent(new CrossfadeFXEvent(CVector3f(1, 1, 0)));
		_world->SendEvent(new SoundEvent(SoundID::ComboPickup));

		Player->GetGuns()->Delta(2);
	}
	//pickup grenade
	else if (hit->Matches(GrenadePickup)) {
		hit->Destroy();

		_world->SendEvent(new CrossfadeFXEvent(CVector3f(0, 0, 1)));
		_world->SendEvent(new SoundEvent(SoundID::GrenadePickup));

		Player->GetGrenades()->Delta(1);
	}
}

void GameplayReactionSystem::GrenadeExplosion(Entity *grenade)
{
	CVector3f explosionPosition = grenade->GetTransform()->Position;
	
	auto forceField = _world->Factory->CreateForceField(explosionPosition);
	forceField->GetTimer()->Activate();

	_world->SendEvent(new AlphaSpereFXEvent(explosionPosition, 30, CVector3f(0, 0, 1), CVector3f(1, 0, 0), true, 5));
	_world->SendEvent(new SoundEvent(SoundID::GrenadeExplosion, explosionPosition));

	grenade->Destroy();
}

void GameplayReactionSystem::NextWave()
{
	_wave++;
	UpdateWavesText();

	_enemies = _wave * ENEMIES_PER_WAVE;
	
	_world->SendEvent(new SoundEvent(SoundID::NextWave));
	_world->SendEvent(new EnemySpawnEvent(_enemies, 2.0f, _wave * 2 + 50));
	_world->SendEvent(new FogFXEvent(true, (float)_wave/1000.0f, CVector3f(1.0f, 0.8f, 0.8f)));

	Player->GetWaveCount()->Delta(1);
}

void GameplayReactionSystem::GameOver()
{
	_world->SendEvent(new SoundEvent(SoundID::GameOverSound));
	_world->SetState(GameState::GameOver);
}

void GameplayReactionSystem::BonusSpawn(CVector3f position)
{
	//hp bonus
	if (Random() <= HEALTH_BONUS_SPAWN_CHANCE) {
		_world->Factory->CreateHpBonusPickup(position);
		return;
	}

	//gun combo pickup
	if (Random() <= GUN_BONUS_SPAWN_CHANCE) {
		_world->Factory->CreateGunComboPickup(position);
		return;
	}

	//grenade pickup
	if (Random() <= GRENADE_BONUS_SPAWN_CHANCE) {
		_world->Factory->CreateGrenadePickup(position);
		return;
	}
}

void GameplayReactionSystem::UpdateKillScoreText()
{
	char kills[128];
	sprintf_s(kills, "KILLS: %d\n", _killScore->Value());
	_killsTxt->GetText()->text = kills;
}

void GameplayReactionSystem::UpdateHpText()
{
	char health[128];
	sprintf_s(health, "HP: %d\n", _playerHp->Value());
	_healthTxt->GetText()->text = health;
}

void GameplayReactionSystem::UpdateGrenadesText()
{
	char grenades[128];
	sprintf_s(grenades, "Grenades: %d\n", _grenadesCount->Value());
	_greandesTxt->GetText()->text = grenades;
}

void GameplayReactionSystem::UpdateWavesText()
{
	char wave[128];
	sprintf_s(wave, "Wave: %d\n", _wave);
	_wavesTxt->GetText()->text = wave;
}

int GameplayReactionSystem::Random()
{
	return rand() % 100;
}

void GameplayReactionSystem::OnEnterGameState(GameState state)
{
	_playing = state == Play;
}

void GameplayReactionSystem::OnExitGameState(GameState state)
{
	//on exit from game over, reset all
	if(state != GameState::GameOver) return;

	//reset scores
	_playerHp->ToMaximum();
	_killScore->ToMinimum();
	_grenadesCount->ToMinimum();
	Player->GetWaveCount()->ToMinimum();
	_wave = 0;

	UpdateGrenadesText();
	UpdateHpText();
	UpdateWavesText();
	UpdateKillScoreText();

	_world->SendEvent(new FogFXEvent(false));

	//reset position
	Player->GetTransform()->Position = CVector3f( 0.0f, 2.0f, -100.0f);
	Player->GetGuns()->ToMinimum();

	_enemies = 0;

	//remove all active pickups and force fields
	for (auto& entity : _world->EntityManager->_entities)
	{
		if(entity->Matches(Pickup) || entity->Matches(ForceField))
			entity->Destroy();
	}

	_nextWaveTimer.Set(NEXT_WAVE_DELAY);
	_nextWaveTimer.Activate();
}
