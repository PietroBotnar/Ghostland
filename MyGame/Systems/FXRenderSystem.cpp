#include "FXRenderSystem.h"
#include "..\ECS\World.h"
#include "..\Events\AnimationFXEvent.h"
#include "..\Events\CrossfadeFXEvent.h"
#include "..\Events\AlphaSpereFXEvent.h"
#include "..\Events\FogFXEvent.h"

void FXRenderSystem::InitialiseSystem()
{
	//load sprite animations
	_2dAnimations[Explosion]	= SpriteAnimation("Resources\\Textures\\explosion.tga", 4, 5, 16);
	_2dAnimations[Portal]		= SpriteAnimation("Resources\\Textures\\portal.png", 5, 5, 25);
	
	_crossFade.Disable();
}

void FXRenderSystem::Update(float dt)
{
	if (!_playing)	return;

	for (auto& animation : _activeAnimations)
	{
		animation.Update(dt);
		animation.Render();
	}

	for (auto& alphaSphere : _activeAlphaSpheres) {
		alphaSphere.Update(dt);
		alphaSphere.Render();
	}

	_crossFade.Update(dt);
	_crossFade.Render();

	cleanUpFXs();
}

void FXRenderSystem::OnEvent(IEvent* _event)
{
	switch (_event->Type)
	{
		case AnimationFX:
		{
			auto animationEvent = static_cast<AnimationFXEvent*>(_event);

			//a copy is made to allow the same animation be played multiple times at the same time
			auto animation = _2dAnimations[animationEvent->AnimationID];
			animation.Activate(animationEvent->position, _world->GetCameraViewPoint() - _world->GetCameraPosition(), animationEvent->width, animationEvent->height);
			_activeAnimations.push_back(animation);
			break;
		}
		case CrossfadeFX:
		{
			auto crossfadeEvent = static_cast<CrossfadeFXEvent*>(_event);
			_crossFade.Activate(crossfadeEvent->color);
			break;
		}
		case AlphaSpereFX:
		{
			auto asEvent = static_cast<AlphaSpereFXEvent*>(_event);

			//make a copy of alpha sphere to allow multiple sphere rendering
			auto alphaSphereCopy = _alphaSphere;
			alphaSphereCopy.Activate(asEvent->position, asEvent->radius, asEvent->startColour, asEvent->endColour, asEvent->zap, asEvent->zapLevel);

			_activeAlphaSpheres.push_back(alphaSphereCopy);
			break;
		}
		case FogFX:
		{
			auto fogEvent = static_cast<FogFXEvent*>(_event);
			if (fogEvent->Active) 
			{
				ActivateFog(fogEvent->Density, fogEvent->Color);
			}
			else 
			{
				glDisable(GL_FOG);
			}
			break;
		}
	}
}

void FXRenderSystem::ActivateFog(float density, CVector3f rgb)
{
	GLfloat fogColor[4] = { rgb.x, rgb.y, rgb.z, 1.0 };

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, density);
	glHint(GL_FOG_HINT, GL_NICEST);
}

void FXRenderSystem::cleanUpFXs()
{
	//erase non active animations
	for (auto it = _activeAnimations.begin(); it != _activeAnimations.end();) {
		if(!(*it).IsActive())
			it = _activeAnimations.erase(it);
		else
			++it;
	}

	//erase non active alpha spheres
	for (auto it = _activeAlphaSpheres.begin(); it != _activeAlphaSpheres.end();) {
		if (!(*it).IsActive())
			it = _activeAlphaSpheres.erase(it);
		else
			++it;
	}
}

void FXRenderSystem::OnEnterGameState(GameState state)
{
	_playing = state == Play;

	if(state == GameOver){

		glDisable(GL_FOG);
		_crossFade.Disable();
		_activeAnimations.clear();

		for (auto& fx : _activeAlphaSpheres)
		{
			fx.Disable();
		}

		_activeAlphaSpheres.clear();
	 }
}
