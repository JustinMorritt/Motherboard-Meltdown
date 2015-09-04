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

	//UPDATES
	void UpdateMainMenu(float dt);
	void UpdateGame(float dt);
	void UpdateBugs(float dt);
	void UpdatePickups(float dt);
	void UpdateProjectiles(float dt);

	//SPAWNERS
	void SpawnBug();
	void SpawnMushroom();
	void SpawnProjectile();


	//BUTTON HANDLERS
	void BtnsMainMenu(float x, float y, bool clicked);
	void BtnsAbout(float x, float y, bool clicked);
	void BtnsPaused(float x, float y, bool clicked);
	void BtnsGameOn(float x, float y, bool clicked);
	void BtnsWin(float x, float y, bool clicked);
	void BtnsLose(float x, float y, bool clicked);
	bool InButton3D(float sx, float sy, Button* button);
	bool InButton2D(float sx, float sy, Button* button);
	bool CamOnPickUp(Button* pickup);
	bool ProjectileBounds(Button* proj);

private:
	Sky* mSky;
	Terrain mTerrain;
	TextureMgr mTexMgr;

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
	Button* mInvader;

	Button* mInvader2;
	Button* mInvader3;
	Button* mInvader4;
	Button* mMushroom;
	Button* mProjectile;

	std::vector<Button*> mInvaders;
	std::vector<Button*> mMushrooms;
	std::vector<Button*> mProjectiles;

	//Models
	std::vector<BasicModelInstance> mModelInstances;
	BasicModel* testModel;


	//Binary Floors 
	Button* mNorthF;
	Button* mSouthF;
	Button* mWestF;
	Button* mEastF;

	//Progress Bars 
	Button* mCompBar;
	Button* mCompBarOL;
	Button* mBugBar;
	Button* mBugBarOL;

	//Spawn Timers
	float spawnTimer; // Set this per second ..
	int spawnBugTime;
	int spawnMushTime;
	int speedBonusTime;

	ID3D11ShaderResourceView* mFlareTexSRV;
	ID3D11ShaderResourceView* mRainTexSRV;
	ID3D11ShaderResourceView* mRandomTexSRV;

	ParticleSystem mFire;
	ParticleSystem mRain;

	DirectionalLight mDirLights[3];

	Camera mCam;


	//MAIN MENU STUFF
	ID3D11ShaderResourceView* mFloorTexSRV;
	ID3D11ShaderResourceView* mBinaryFloorSRV; //TODO: Maybe add a different material for it 

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

	POINT mLastMousePos;

	XMMATRIX mOrthoWorld;
	
	int mMoveSpeed;
};




#endif // Engine_h__
