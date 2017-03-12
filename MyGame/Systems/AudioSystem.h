#pragma once
#include "..\ECS\IEvent.h"
#include "..\include\FMOD_Studio\fmod_common.h"
#include "..\include\FMOD_Studio\fmod.hpp"
#include "..\ECS\ISystem.h"
#include "..\Events\SoundEvent.h"

#include <map>
#include "..\Data\Enums.h"
#include <string>
#include "..\ECS\GameFSM.h"

//Controls all audio and soundfx played in the world using FMOD
class AudioSystem : public ISystem
{
public:
	explicit AudioSystem(){};
	~AudioSystem(){};

	virtual void InitialiseSystem() override;


	virtual void Update(float dt) override;
	virtual void OnEvent(IEvent* _event) override;

	void PlayAudio(SoundID id, FMOD::Channel** channel = NULL);
	void Play3dAudio(SoundEvent* soundEvent, FMOD::Channel** channel = NULL);
	void LoadAudioFile(SoundID id, char *filename, bool is3D = false);
	void LoadMusicStream(SoundID id, char *filename);

	void UpdateListenerPosition();

	virtual void OnEnterGameState(GameState state) override;

private:
	void FmodErrorCheck(FMOD_RESULT result, std::string filename);

	FMOD_RESULT result;
	FMOD::System *m_FmodSystem;	// the global variable for talking to FMOD

	FMOD::Channel* gameMusicChannel;
	FMOD::Channel* menuMusicChannel;

	bool _musicLoaded = false;

	std::map<SoundID, FMOD::Sound*> _audioFiles;
};

