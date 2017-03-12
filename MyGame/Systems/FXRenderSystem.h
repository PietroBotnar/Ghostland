#pragma once
#include "..\ECS\ISystem.h"
#include "..\Objects\CrossFade.h"
#include "..\Objects\SpriteAnimation.h"

#include <map>
#include "..\Data\Enums.h"
#include "..\Objects\AlphaSphere.h"
#include "..\Data\Vector3f.h"
#include <vector>

//Controls and renders special effects in game
class FXRenderSystem : public ISystem
{
public:
	explicit FXRenderSystem(){};
	~FXRenderSystem(){};

	virtual void InitialiseSystem() override;
	virtual void Update(float dt) override;
	virtual void OnEvent(IEvent* _event) override;
	virtual void OnEnterGameState(GameState state) override;

	void ActivateFog(float density, CVector3f rgb);

private:
	//removes all non-active animations and alpha spheres
	void cleanUpFXs();

	CCrossFade			_crossFade;
	CAlphaSphere		_alphaSphere;

	std::map<SpriteAnimationID, SpriteAnimation> _2dAnimations;
	std::vector<SpriteAnimation> _activeAnimations;
	std::vector<CAlphaSphere>	_activeAlphaSpheres;

	bool _playing = false;
};

