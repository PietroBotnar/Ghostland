#pragma once
#include "..\ECS\ISystem.h"
#include "..\ECS\Entity.h"
#include "..\Components\TextComponent.h"
#include "..\Objects\Timer.h"

//Handles gameplay reactions and event coordinations
class GameplayReactionSystem :	public ISystem
{
public:
	GameplayReactionSystem(){};
	~GameplayReactionSystem(){};

	virtual void InitialiseSystem() override;
	virtual void Update(float dt) override;
	virtual void OnEvent(IEvent* _event) override;
	virtual void OnEnterGameState(GameState state) override;
	virtual void OnExitGameState(GameState state) override;

private:
	void BulletHit(Entity* bullet, Entity* hit);
	void GhostHit(Entity* ghost);
	void PlayerHit(Entity* hit, float dt);
	void GrenadeExplosion(Entity *grenade);

	void NextWave();
	void GameOver();
	void BonusSpawn(CVector3f position);

	void UpdateKillScoreText();
	void UpdateHpText();
	void UpdateGrenadesText();
	void UpdateWavesText();

	int Random();

	bool _playing;

	Entity* _killsTxt;
	Entity* _healthTxt;
	Entity* _greandesTxt;
	Entity* _wavesTxt;

	Entity* Player;

	HitPointsComponent* _playerHp;
	ScoreComponent*		_killScore;
	GrenadesComponent*	_grenadesCount;

	Timer _nextWaveTimer;

	float elapsedFromHit;

	int _wave		= 0;
	int _enemies	= 0;

	const int HEALTH_BONUS = 25;

	const int HEALTH_BONUS_SPAWN_CHANCE		= 15;
	const int GUN_BONUS_SPAWN_CHANCE		= 5;
	const int GRENADE_BONUS_SPAWN_CHANCE	= 20;

	const int ENEMIES_PER_WAVE = 10; 
	const float NEXT_WAVE_DELAY = 5.0f; //seconds before next wave trigger
};