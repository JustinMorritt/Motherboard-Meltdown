#ifndef Engine_h__
#define Engine_h__

#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "LightHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include "RenderStates.h"
#include "Camera.h"
#include "Sky.h"
#include "Terrain.h"
#include "ParticleSystem.h"
#include "StateMachine.h"
#include "Button.h"
#include "xnacollision.h"
#include "MathHelper.h"
#include "TextureMgr.h"
#include "BasicModel.h"
#include "Sound.h"
#include <vector>


class Engine : public D3DApp
{
public:
	Engine(HINSTANCE hInstance);
	~Engine();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnKeyUP(WPARAM btnState);
	void KeyboardHandler(float dt);
	void CameraHandler();
	void ClearScene();
	void RestoreStates();
	void ResetCamMainMenu();
	void ResetCamInGame();
	void IncProgress(float dt);
	void IncBugs(float dt);
	void DecBugs(float dt);
	void DecHP(float dt);
	void ClearVectors();

	//SCENE INITS
	void InitMainMenu();
	void InitPaused();
	void InitGameOn();
	void InitWin();
	void InitLose();

	//GAME DRAWS
	void DrawMainMenu();
	void DrawAbout();
	void DrawPaused();
	void DrawGameOn();
	void DrawWin();
	void DrawLose();
	void DrawBossWin();
	void DrawBossLose();

	//UPDATES
	void UpdateMainMenu(float dt);
	void UpdateGame(float dt);
	void UpdateBugs(float dt);
	void UpdatePickups(float dt);
	void UpdateProjectiles(float dt);
	void UpdateGhosts(float dt);
	void UpdateBoss(float dt);
	void UpdateBossWin(float dt);
	void UpdateBossLose(float dt);

	//SPAWNERS
	void SpawnBug();
	void SpawnMushroom();
	void SpawnProjectile();
	void SpawnGhost();


	//BUTTON HANDLERS
	void BtnsMainMenu(float x, float y, bool clicked);
	void BtnsAbout(float x, float y, bool clicked);
	void BtnsPaused(float x, float y, bool clicked);
	void BtnsGameOn(float x, float y, bool clicked);
	void BtnsWin(float x, float y, bool clicked);
	void BtnsLose(float x, float y, bool clicked);
	void BtnsBossLose(float x, float y, bool clicked);
	void BtnsBossWin(float x, float y, bool clicked);
	bool InButton3D(float sx, float sy, Button* button);
	bool InButton2D(float sx, float sy, Button* button);

	//COLLISION
	bool CamOnPickUp(Button* pickup);
	bool ProjectileBounds(Button* proj);
	bool ProjectileHitBug(Button* proj);
	bool ProjectileHitBoss(Button* proj);
	bool ProjectileHitGhost(Button* proj);
	bool GhostHitCam(Button* ghost);
	bool BossHitCam(Button* ghost);

private:
	Sky* mSky;
	TextureMgr mTexMgr;
	Camera mCam;
	Sound mSound;

	ID3D11Buffer* mShapesVB;
	ID3D11Buffer* mShapesIB;

	//Buttons
	Button* mPlayButt;
	Button* mTitleButt;
	Button* mAboutButt;
	Button* mMusicButt;
	Button* mSoundButt;
	Button* mSOnButt;
	Button* mSOffButt;
	Button* mMOnButt;
	Button* mMOffButt;
	Button* mCompiledButt;
	Button* mBymeButt;
	Button* mBugsButt;
	Button* mQuitButt;
	Button* mRestartButt;
	Button* mPausedButt;
	Button* mBackButt;
	Button* mAboutMsgButt;
	Button* mYouWinButt;
	Button* mYouLoseButt;
	Button* mRetryButt;
	Button* mModeButt;
	Button* mEasyButt;
	Button* mMedButt;
	Button* mHardButt;
	Button* mInsaneButt;
	Button* mInvader;
	Button* mHPButt;
	Button* mBossBattButt;
	Button* mBSOD;


	Button* mInvader2;
	Button* mInvader3;
	Button* mInvader4;
	Button* mMushroom;
	Button* mProjectile;

	Button* mBoss;
	Button* mGhost1;
	Button* mGhost2;
	Button* mGhost3;
	Button* mGhost4;
	Button* mGhost5;

	std::vector<Button*> mInvaders;
	std::vector<Button*> mMushrooms;
	std::vector<Button*> mProjectiles;
	std::vector<Button*> mGhosts;

	//Models
	std::vector<BasicModelInstance> mModelInstances;
	BasicModel* testModel;

	//Binary Floors 
	Button* mNorthF;
	Button* mSouthF;
	Button* mWestF;
	Button* mEastF;

	//Binary Walls
	Button* mNorthW; 
	Button* mSouthW; 
	Button* mWestW;  
	Button* mEastW;	 

	//Progress Bars 
	Button* mCompBar;
	Button* mCompBarOL;
	Button* mBugBar;
	Button* mBugBarOL;
	Button* mHPOL;
	Button* mHPBAR;

	//Spawn Timers
	float spawnTimer; // Set this per second ..
	float bossTimer;
	float randBossTime;
	bool toRandSpot;
	bool toCam;
	float bugsWorth;
	float dmgAmount;
	float difficultyTimer;
	int spawnBugTime;
	int spawnMushTime;
	int speedBonusTime;
	int waitToClickTime;
	int mMoveSpeed;

	ID3D11ShaderResourceView* mFlareTexSRV;
	ID3D11ShaderResourceView* mRainTexSRV;
	ID3D11ShaderResourceView* mRandomTexSRV;

	ParticleSystem mFire;
	ParticleSystem mRain;

	DirectionalLight mDirLights[3];
	DirectionalLight mDirLights2[3];

	ID3D11ShaderResourceView* mFloorTexSRV;

	Material mGridMat;
	Material mShadowMat;

	XMFLOAT4X4 mGridWorld;
	int mGridVertexOffset;
	UINT mGridIndexOffset;
	UINT mGridIndexCount;

	bool mWalkCamMode;
	bool mWireMode;
	bool mBFCull;
	bool fullyLoaded;
	bool exitable;
	bool inButt;

	POINT mLastMousePos;

	XMMATRIX mOrthoWorld;
};


#endif // Engine_h__
