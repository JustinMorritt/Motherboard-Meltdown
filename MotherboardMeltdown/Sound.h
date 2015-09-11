#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_

#include "fmod.hpp"
#include "fmod_errors.h"
#include "windows.h"

//		VC++ Include = \MotherboardMeltdown\Common\FMOD\inc
//		VC++ Library = \MotherboardMeltdown\Common\FMOD\lib
// Configuration Properties / Debugging /    *FOR .DLL*
//  --> PATH=C:\Users\jm\Desktop\Motherboard-Meltdown\MotherboardMeltdown\Common\FMOD\lib

class Sound
{
public:
	Sound();
	~Sound();

	void Initialize();
	void Shutdown();
	bool ERRCHECK(FMOD_RESULT result);
	
	void StreamMusic(int num);
	void PlaySound(int num);
	void PauseMusic(bool pause, int num);
	bool PlayingMusic(int num);
	bool MusicPaused(int num);

private:
	FMOD::System     *system;
	FMOD::Sound      *sound1, *sound2, *sound3, *sound4, *sound5, *sound, *MainSong, *GameSong, *BossSong;
	FMOD::Channel    *channel = 0;
	FMOD::Channel    *channel2 = 0;
	FMOD::Channel    *channel3 = 0;
	FMOD_RESULT       result;
	unsigned int      version;
	void             *extradriverdata = 0;
	int               numsubsounds;
};

#endif