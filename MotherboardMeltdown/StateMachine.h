#ifndef StateMachine_h__
#define StateMachine_h__

enum class GameState
{
	MAINMENU,
	PAUSED,
	WIN,
	LOSE,
	GAMEON
};

enum class SoundState
{
	SOUNDON,
	SOUNDOFF
};

enum class MusicState
{
	MUSICON,
	MUSICOFF
};



struct StateMachine
{
	static GameState*	pGameState;
	static SoundState*	pSoundState;
	static MusicState*	pMusicState;
};


#endif // StateMachine_h__