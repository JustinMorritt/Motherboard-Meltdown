#ifndef StateMachine_h__
#define StateMachine_h__

enum class GameState
{
	MAINMENU,
	ABOUT,
	PAUSED,
	WIN,
	LOSE,
	GAMEON
};

enum class GameMode
{
	EASY,
	MED,
	HARD,
	INSANE
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
	static GameMode*	pGameMode;
};


#endif // StateMachine_h__