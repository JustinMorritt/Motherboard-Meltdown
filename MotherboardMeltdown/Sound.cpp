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
	result = system->createStream("Sound/main.wav", FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);	ERRCHECK(result);
	result = sound->getNumSubSounds(&numsubsounds);	ERRCHECK(result);
	if (numsubsounds)
	{
		sound->getSubSound(0, &MainSong); ERRCHECK(result);
	}
	else
	{
		MainSong = sound;
	}

// 	result = system->createSound(Common_MediaPath("drumloop.wav"), FMOD_DEFAULT, 0, &sound1);
// 	ERRCHECK(result);

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
	case 1: result = system->playSound(MainSong, 0, false, &channel); ERRCHECK(result); break;
	case 2: result = system->playSound(MainSong, 0, false, &channel); ERRCHECK(result); break;
	}
}

void Sound::PlaySound(int num)
{
	switch (num)
	{
	case 1: result = system->playSound(sound1, 0, false, &channel);	ERRCHECK(result); break;
	case 2: result = system->playSound(sound2, 0, false, &channel);	ERRCHECK(result); break;
	case 3: result = system->playSound(sound3, 0, false, &channel);	ERRCHECK(result); break;
	}
}

void Sound::PauseMusic(bool pause)
{
	channel->setPaused(pause);
}
