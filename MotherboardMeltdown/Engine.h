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

	void UpdateMainMenu(float dt);
	void UpdateGame(float dt);


	//BUTTON HANDLERS
	void BtnsMainMenu(float x, float y, bool clicked);
	void BtnsAbout(float x, float y, bool clicked);
	void BtnsPaused(float x, float y, bool clicked);
	void BtnsGameOn(float x, float y, bool clicked);
	void BtnsWin(float x, float y, bool clicked);
	void BtnsLose(float x, float y, bool clicked);
	bool InButton(float sx, float sy, Button* button);
	bool InButton2D(float sx, float sy, Button* button);

private:
	Sky* mSky;
	Terrain mTerrain;

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
	

};




#endif // Engine_h__
