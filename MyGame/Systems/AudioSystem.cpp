#include "AudioSystem.h"
#include <windows.h>									// Header File For The Windows Library
#include <stdio.h>										// Header File For The Standard Input/Output Functions
#include <stdlib.h>	

#include "..\ECS\World.h"
#include "..\include\FMOD_Studio\fmod_errors.h"
#include "..\ECS\EventTypes.h"
#include <winuser.h>

#pragma comment(lib, "../lib/FMOD_Studio/fmod_vc.lib")

void AudioSystem::InitialiseSystem()
{
	// Create an FMOD system
	result = FMOD::System_Create(&m_FmodSystem);
	FmodErrorCheck(result, "FMOD");
		
	// Initialise the system
	result = m_FmodSystem->init(32, FMOD_INIT_NORMAL, 0);
	FmodErrorCheck(result, "FMOD");

	//load audio files here
	LoadAudioFile(SoundID::PlayerShot,		"Resources\\Audio\\shooting_0.wav");
	LoadAudioFile(SoundID::GhostDeath,		"Resources\\Audio\\ghost_death.mp3");
	LoadAudioFile(SoundID::SmallExplosion,	"Resources\\Audio\\small_explosion.wav", true);
	LoadAudioFile(SoundID::PlayerHit,		"Resources\\Audio\\player_hit.wav");
	LoadAudioFile(SoundID::GameOverSound,	"Resources\\Audio\\game_over.mp3");
	LoadAudioFile(SoundID::HealthPickup,	"Resources\\Audio\\health_pickup.wav");
	LoadAudioFile(SoundID::GhostScream,		"Resources\\Audio\\creepy-ghost-scream.mp3");
	LoadAudioFile(SoundID::ComboPickup,		"Resources\\Audio\\combo_pickup.wav");
	LoadAudioFile(SoundID::GrenadePickup,	"Resources\\Audio\\grenade_pickup.wav");
	LoadAudioFile(SoundID::GrenadeExplosion,"Resources\\Audio\\grenade_explosion.mp3", true);
	LoadAudioFile(SoundID::NextWave,		"Resources\\Audio\\next_wave.wav");
	LoadAudioFile(SoundID::Spawn,			"Resources\\Audio\\spawn.mp3");

	//load music
	LoadMusicStream(SoundID::BackgrondMusic, "Resources\\Audio\\music.wav");
	LoadMusicStream(SoundID::MainMenu,		"Resources\\Audio\\menu.mp3");
}

void AudioSystem::Update(float dt)
{
	UpdateListenerPosition();
	m_FmodSystem->update();
}

void AudioSystem::OnEvent(IEvent* _event)
{
	if (_event->Type == Sound) 
	{
		auto soundEvent = static_cast<SoundEvent*>(_event);

		if (soundEvent->is3D)
			Play3dAudio(soundEvent);
		else
			PlayAudio(soundEvent->Sound);
	}
}

void AudioSystem::PlayAudio(SoundID id, FMOD::Channel** channel)
{
	auto audio = _audioFiles[id];
	result = m_FmodSystem->playSound(audio, NULL, false, channel);
}

void AudioSystem::Play3dAudio(SoundEvent* soundEvent, FMOD::Channel** channeli)
{
	auto& position = soundEvent->SourcePosition;

	// Set position and velocity of listener based on the camera
	FMOD_VECTOR pos, vel;
	pos.x = position.x;
	pos.y = position.y;
	pos.z = position.z;
	vel.x = 0.0f;
	vel.y = 0.0f;
	vel.z = 0.0f;

	// Play the sound
	FMOD::Channel* channel = NULL;
	float volume = 1.0f;
	PlayAudio(soundEvent->Sound, &channel);
	result = channel->setVolume(1.0);
	channel->set3DMinMaxDistance(2.5, 500);
	channel->set3DAttributes(&pos, &vel);	 // The the 3D position of the sound

	channeli = &channel;
}

void AudioSystem::LoadAudioFile(SoundID id, char *filename, bool is3D)
{
	FMOD::Sound* sound;
	result = m_FmodSystem->createSound(filename, is3D ? FMOD_3D : NULL, 0, &sound);
	FmodErrorCheck(result, filename);

	if (result != FMOD_OK)
	{
		delete sound;
		return;
	}
	_audioFiles[id] = sound;
}

void AudioSystem::LoadMusicStream(SoundID id, char *filename)
{
	FMOD::Sound* music;
	result = m_FmodSystem->createStream(filename, NULL | FMOD_LOOP_NORMAL, 0, &music);
	FmodErrorCheck(result, filename);

	if (result != FMOD_OK)
	{
		delete music;
		return;
	}
	_audioFiles[id] = music;
}

void AudioSystem::UpdateListenerPosition()
{
	FMOD_VECTOR listenerPosition, listenerVelocity, upVector, viewVector;

	CVector3f cameraPosition	= _world->GetCameraPosition();
	CVector3f dir				= cameraPosition - _world->GetCameraViewPoint();  // note: viewVector pointing backwards due to right-handed coordinate system
	CVector3f up				= CVector3f(0, 1, 0);

	dir.Normalise();
	listenerPosition.x = cameraPosition.x;
	listenerPosition.y = cameraPosition.y;
	listenerPosition.z = cameraPosition.z;
	listenerVelocity.x = 0;
	listenerVelocity.y = 0;
	listenerVelocity.z = 0;
	upVector.x = up.x;
	upVector.y = up.y;
	upVector.z = up.z;
	viewVector.x = dir.x;
	viewVector.y = dir.y;
	viewVector.z = dir.z;

	// Update the listener position, velocity, and orientation based on the camera
	m_FmodSystem->set3DListenerAttributes(0, &listenerPosition, &listenerVelocity, &viewVector, &upVector);
}

void AudioSystem::OnEnterGameState(GameState state)
{
	switch (state)
	{
	case MainMenu:
		PlayAudio(SoundID::MainMenu, &menuMusicChannel);
		break;
	case Play:
		menuMusicChannel->stop();

		if (!_musicLoaded) {
			PlayAudio(SoundID::BackgrondMusic, &gameMusicChannel);
			_musicLoaded = true;
		}
		else
			gameMusicChannel->setPaused(false);

		break;
	case Pause:
			gameMusicChannel->setPaused(true);
		break;
	case GameOver:
			gameMusicChannel->stop();
		break;
	}
}

void AudioSystem::FmodErrorCheck(FMOD_RESULT result, std::string filename)
{
	if (result == FMOD_OK) return;

	char message[1024];
	sprintf_s(message, "Cannot load audio\n%s\n", filename.c_str());
	MessageBox(NULL, message, "Error", MB_ICONERROR);
}
