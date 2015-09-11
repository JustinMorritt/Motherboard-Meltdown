#include "Sound.h"

Sound::Sound() : system(0), sound1(0), sound2(0), sound3(0), sound(0), MainSong(0)
{

}
Sound::~Sound()
{

}

void Sound::Initialize()
{
	//FMOD SYSTEM INIT**********************************************************
	result = FMOD::System_Create(&system);	ERRCHECK(result);
	result = system->getVersion(&version);	ERRCHECK(result);
	if (version < FMOD_VERSION){OutputDebugString(L"Version FMOD OutDated");}
	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);	ERRCHECK(result);
	//FMOD SYSTEM INIT**********************************************************
	


	//LOAD MUSIC SOUNDS
	result = system->createStream("Sound/main.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);	ERRCHECK(result);
	result = sound->getNumSubSounds(&numsubsounds);	ERRCHECK(result);
	if (numsubsounds)
	{
		sound->getSubSound(0, &MainSong); ERRCHECK(result);
	}
	else
	{
		MainSong = sound;
	}

	result = system->createStream("Sound/maingame.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);	ERRCHECK(result);
	result = sound->getNumSubSounds(&numsubsounds);	ERRCHECK(result);
	if (numsubsounds)
	{
		sound->getSubSound(0, &GameSong); ERRCHECK(result);
	}
	else
	{
		GameSong = sound;
	}

	result = system->createStream("Sound/bossmusic.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);	ERRCHECK(result);
	result = sound->getNumSubSounds(&numsubsounds);	ERRCHECK(result);
	if (numsubsounds)
	{
		sound->getSubSound(0, &BossSong); ERRCHECK(result);
	}
	else
	{
		BossSong = sound;
	}


	result = system->createSound("Sound/hover.mp3", FMOD_DEFAULT, 0, &sound1);
	ERRCHECK(result);

	result = system->createSound("Sound/evilboss.mp3", FMOD_DEFAULT, 0, &sound2);
	ERRCHECK(result);

	result = system->createSound("Sound/losegame.mp3", FMOD_DEFAULT, 0, &sound3);
	ERRCHECK(result);

	result = system->createSound("Sound/wingame.mp3", FMOD_DEFAULT, 0, &sound4);
	ERRCHECK(result);

	result = system->createSound("Sound/grow.mp3", FMOD_DEFAULT, 0, &sound5);
	ERRCHECK(result);
}

void Sound::Shutdown()
{
	result = sound->release();  
	ERRCHECK(result);
	result = sound1->release(); 
	ERRCHECK(result);
	result = sound2->release(); 
	ERRCHECK(result);
	result = sound3->release(); 
	ERRCHECK(result);
	result = MainSong->release();
	ERRCHECK(result);
	result = system->close();
	ERRCHECK(result);
	result = system->release();
	ERRCHECK(result);
}

bool Sound::ERRCHECK(FMOD_RESULT result)
{
	return (result == FMOD_OK);
}

void Sound::StreamMusic(int num)
{
	switch (num)
	{
	case 1: result = system->playSound(MainSong, 0, false, &channel);  ERRCHECK(result); break;
	case 2: result = system->playSound(GameSong, 0, false, &channel2); ERRCHECK(result); break;
	case 3: result = system->playSound(BossSong, 0, false, &channel3); ERRCHECK(result); break;
	}
}

void Sound::PlaySound(int num)
{
	switch (num)
	{
	case 1: result = system->playSound(sound1, 0, false, 0);	ERRCHECK(result); break; // HOVER
	case 2: result = system->playSound(sound2, 0, false, 0);	ERRCHECK(result); break; // EVILBOSS
	case 3: result = system->playSound(sound3, 0, false, 0);	ERRCHECK(result); break; // LOSEGAME
	case 4: result = system->playSound(sound4, 0, false, 0);	ERRCHECK(result); break; // WINGAME
	case 5: result = system->playSound(sound5, 0, false, 0);	ERRCHECK(result); break; // WINGAME
	}
}

void Sound::PauseMusic(bool pause, int num)
{
	switch (num)
	{
	case 1:	channel->setPaused(pause); break;
	case 2: channel2->setPaused(pause); break;
	case 3: channel3->setPaused(pause); break;
	}
}

bool Sound::PlayingMusic(int num)
{
	bool playing;
	switch (num)
	{
	case 1: channel->isPlaying(&playing);  break;
	case 2: channel2->isPlaying(&playing); break;
	case 3: channel3->isPlaying(&playing); break;
	}
	return  playing;
}

bool Sound::MusicPaused(int num)
{
	bool paused;
	switch (num)
	{
	case 1: channel->getPaused(&paused); break;
	case 2: channel2->getPaused(&paused); break;
	case 3: channel3->getPaused(&paused); break;
	}
	return  paused;
}
