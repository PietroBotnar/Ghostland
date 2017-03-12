#pragma once
#include "..\ECS\ISystem.h"
#include "..\Data\Vector3f.h"
#include "..\Components\TransformComponent.h"
#include "..\ECS\Entity.h"
#include "..\ECS\GameFSM.h"

//Controls lights in the game
class LightingSystem :	public ISystem
{
public:
	explicit LightingSystem(){};
	~LightingSystem(){};

	virtual void InitialiseSystem() override;
	virtual void Update(float dt) override;
	virtual void OnEnterGameState(GameState state) override;
	virtual void OnEvent(IEvent* _event) override;

private:
	void LightHouse();
	void PlayerTorch();

	void DefaultLight();

	void UpdateTorchDisplay();

	CVector3f lighthouseDirection;

	bool _torchOn = false;
	bool _playing = false;

	TransformComponent* playerTransform;
	Entity* _torchTxt;
};

