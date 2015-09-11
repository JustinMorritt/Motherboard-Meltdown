#include "Sound.h"

Sound::Sound() : system(0), sound1(0), sound2(0), sound3(0), sound(0), MainSong(0)
{

}
Sound::~Sound()
{

}

void Sound::Initialize()
{
	result = FMOD::System_Create(&system);
	ERRCHECK(result);

	result = system->getVersion(&version);
	ERRCHECK(result);

	if (version < FMOD_VERSION)
	{
		OutputDebugString(L"Version FMOD OutDated");
	}

	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
	ERRCHECK(result);

	result = system->createStream("Sound/main.wav", FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
	ERRCHECK(result);

	result = sound->getNumSubSounds(&numsubsounds);
	ERRCHECK(result);

	if (numsubsounds)
	{
		sound->getSubSound(0, &MainSong);
		ERRCHECK(result);
	}
	else
	{
		MainSong = sound;
	}

	/*
	Play the sound.
	*/
	result = system->playSound(MainSong, 0, false, &channel);
	ERRCHECK(result);

}

void Sound::Shutdown()
{
	result = sound->release();  /* Release the parent, not the sound that was retrieved with getSubSound. */
	ERRCHECK(result);
	result = sound1->release();  /* Release the parent, not the sound that was retrieved with getSubSound. */
	ERRCHECK(result);
	result = sound2->release();  /* Release the parent, not the sound that was retrieved with getSubSound. */
	ERRCHECK(result);
	result = sound3->release();  /* Release the parent, not the sound that was retrieved with getSubSound. */
	ERRCHECK(result);
	result = MainSong->release();  /* Release the parent, not the sound that was retrieved with getSubSound. */
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
