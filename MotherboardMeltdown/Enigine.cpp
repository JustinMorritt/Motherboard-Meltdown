#include "Engine.h"
//TODO:..Make A Button Able To Have a Start And End Position and it Goes to it ..





Engine::Engine(HINSTANCE hInstance)
	: D3DApp(hInstance), mSky(0), mRandomTexSRV(0), mFlareTexSRV(0), mRainTexSRV(0), mFloorTexSRV(0), mWalkCamMode(false), mWireMode(false), mBFCull(false),
	mPlayButt(0),
	mPausedButt(0),
	mTitleButt(0),
	mAboutButt(0),
	mBymeButt(0),
	mCompiledButt(0),
	mMusicButt(0),
	mSoundButt(0),
	mSOnButt(0),
	mMOffButt(0),
	mMOnButt(0),
	mSOffButt(0),
	mBugsButt(0),
	mQuitButt(0),
	mRestartButt(0),
	mBackButt(0),
	mAboutMsgButt(0),
	mNorthF(0),
	mWestF(0),
	mEastF(0),
	mSouthF(0),
	mCompBar(0),
	mCompBarOL(0),
	mBugBar(0),
	mBugBarOL(0),
	mInvader(0),
	testModel(0),
	spawnBugTime(0),
	mInvader2(0),
	mInvader3(0),
	mInvader4(0),
	mMushroom(0),
	spawnMushTime(0),
	speedBonusTime(0),
	mProjectile(0),
	mMoveSpeed(500),
	spawnTimer(0.0f),
	fullyLoaded(false)
{
	mMainWndCaption = L"Motherboard Meltdown";
	mEnable4xMsaa = false;

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	mCam.SetPosition(0.0f, 150.0f, -400.0f);
	mCam.Pitch(XM_PI / 6.5);


	mDirLights[0].Ambient  = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mDirLights[0].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.8f, 0.8f, 0.7f, 1.0f);
	mDirLights[0].Direction = XMFLOAT3(0.707f, -0.707f, 0.0f);

	mDirLights[1].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[1].Diffuse  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[1].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	mDirLights[2].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Diffuse  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Direction = XMFLOAT3(-0.57735f, -0.57735f, -0.57735f);

	mShadowMat.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mShadowMat.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	mShadowMat.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 16.0f);


	mOrthoWorld = XMMatrixOrthographicLH(mClientWidth, mClientHeight, -1000.0f, 1000.0f);
}

Engine::~Engine()
{
	if (md3dImmediateContext){ md3dImmediateContext->ClearState(); }
	
	ReleaseCOM(mRandomTexSRV);
	ReleaseCOM(mFlareTexSRV);
	ReleaseCOM(mRainTexSRV);

	SafeDelete(testModel);
	SafeDelete(mSky);

	Effects::DestroyAll();
	InputLayouts::DestroyAll();
	RenderStates::DestroyAll();
}

bool Engine::Init()
{
	if(!D3DApp::Init())	return false;

	// Must init Effects first since InputLayouts depend on shader signatures.
	Effects::InitAll(md3dDevice);
	mTexMgr.Init(md3dDevice);
	InputLayouts::InitAll(md3dDevice);
	RenderStates::InitAll(md3dDevice);


	//TODO:: CLEAN UP THE MODEL CLASS WITH FUNCTIONALITY

// 	//Load Models
// 	testModel = new BasicModel(md3dDevice, mTexMgr, "Models\\Motherboard.obj", L"Textures\\");
// 	BasicModelInstance testInstance;
// 	testInstance.Model = testModel;
// 
// 	XMMATRIX modelScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
// 	XMMATRIX modelRot = XMMatrixRotationY(0.0f);
// 	XMMATRIX modelOffset = XMMatrixTranslation(0.0f, 10.0f, 0.0f);
// 
// 	XMStoreFloat4x4(&testInstance.World, modelScale*modelRot*modelOffset);
// 
// 	mModelInstances.push_back(testInstance);






	InitMainMenu();

	mSky = new Sky(md3dDevice, L"Textures/MarriottMadisonWest.dds", 5000.0f);


//FIRE EMITTER
//mRandomTexSRV = d3dHelper::CreateRandomTexture1DSRV(md3dDevice);
// 	std::vector<std::wstring> flares;
// 	flares.push_back(L"Textures\\flare0.dds");
// 	mFlareTexSRV = d3dHelper::CreateTexture2DArraySRV(md3dDevice, md3dImmediateContext, flares);
// 	mFire.Init(md3dDevice, Effects::FireFX, mFlareTexSRV, mRandomTexSRV, 500); 
// 	mFire.SetEmitPos(XMFLOAT3(0.0f, 1.0f, 120.0f));

	*StateMachine::pGameState = GameState::MAINMENU;
	*StateMachine::pSoundState = SoundState::SOUNDON;
	*StateMachine::pMusicState = MusicState::MUSICON;



	return true;
}

void Engine::OnResize()
{
	D3DApp::OnResize();
	mOrthoWorld = XMMatrixOrthographicLH(mClientWidth, mClientHeight, -1000.0f, 1000.0f);
	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 3000.0f);
}

void Engine::UpdateScene(float dt)
{
	KeyboardHandler(dt);
	CameraHandler();

	//SWITCH ON THE MODE THE GAME IS IN 
	switch (*StateMachine::pGameState)
	{
	case GameState::ABOUT:		
	case GameState::MAINMENU:	UpdateMainMenu(dt); break;
	case GameState::GAMEON:		
	case GameState::PAUSED:
	case GameState::WIN:
	case GameState::LOSE:		UpdateGame(dt);		break;
	}

	//mButton1->RotateY(mButton1->rotation);


	//UPDATE PARTICLE SYSTEMS
	mFire.Update(dt, mTimer.TotalTime());
	mRain.Update(dt, mTimer.TotalTime());

	
}

void Engine::UpdateMainMenu(float dt)
{
	mPlayButt->Update(mCam, dt);
	mSoundButt->Update(mCam, dt);
	mMusicButt->Update(mCam, dt);
	mSOnButt->Update(mCam, dt);
	mSOffButt->Update(mCam, dt);
	mMOnButt->Update(mCam, dt);
	mMOffButt->Update(mCam, dt);
	mTitleButt->Update(mCam, dt);
	mAboutButt->Update(mCam, dt);
	mBackButt->Update(mCam, dt);
	mAboutMsgButt->Update(mCam, dt);
	mBymeButt->Update(mCam, dt);


	mNorthF->Update(mCam, dt);
	mWestF->Update(mCam, dt);
	mEastF->Update(mCam, dt);
	mSouthF->Update(mCam, dt);
}
void Engine::UpdateGame(float dt)
{
	(*StateMachine::pGameState == GameState::GAMEON) ? mCursorOn = false : mCursorOn = true;

	mCompiledButt->Update(mCam, dt);
	mBugsButt->Update(mCam, dt);
	mQuitButt->Update(mCam, dt);
	mRestartButt->Update(mCam, dt);
	mPausedButt->Update(mCam, dt);
	mBackButt->Update(mCam, dt);
	mCompBar->Update(mCam, dt); IncProgress(dt);
	mCompBarOL->Update(mCam, dt);
	mBugBar->Update(mCam, dt);
	mBugBarOL->Update(mCam, dt);
	mYouWinButt->Update(mCam, dt);
	mYouLoseButt->Update(mCam, dt);
	mRetryButt->Update(mCam, dt);
	
	
	//TIMER STUFF / SPAWN RATES   *Spawn Before Update Or Youll Get a Flicker Later On Of it Not Translated Yet*
	spawnTimer += dt;
	if (spawnTimer >= 1.0)
	{
		spawnBugTime++;
		if (spawnBugTime == 1){ SpawnBug();	spawnBugTime = 0; }

		spawnMushTime++;
		if (spawnMushTime == 10){ SpawnMushroom();		spawnMushTime = 0; }
		
		if (speedBonusTime > 0){ speedBonusTime--;	mMoveSpeed = 1000;	if (speedBonusTime == 0){ mMoveSpeed = 500; } }

		spawnTimer = 0.0f;
	}

	UpdateBugs(dt);
	UpdatePickups(dt);
	UpdateProjectiles(dt);

// 	if (GetAsyncKeyState('T') & 0x8000)
// 		mInvader->Walk(100.0f*dt);
// 	if (GetAsyncKeyState('H') & 0x8000)
// 		mInvader->Yaw(dt);
// 	if (GetAsyncKeyState('F') & 0x8000)
// 		mInvader->Yaw(-dt);
// 	if (GetAsyncKeyState('J') & 0x8000)
// 		mInvader->Walk(-100.0f*dt);

}
void Engine::UpdateBugs(float dt)
{
	for (int i = 0; i < mInvaders.size(); i++)
	{
		if (mInvaders[i]->goToPos){ mInvaders[i]->Walk(100 * dt); }
		else{ mInvaders[i]->mDead = true; }

		mInvaders[i]->Update(mCam, dt);
		if (mInvaders[i]->mDead)
		{
			
			delete mInvaders[i];
			mInvaders[i] = nullptr;
			mInvaders.erase(mInvaders.begin() + i);
		}
	}
}
void Engine::UpdatePickups(float dt)
{
	for (int i = 0; i < mMushrooms.size(); i++)
	{
		mMushrooms[i]->Yaw(dt);

		mMushrooms[i]->Update(mCam, dt);
		if (CamOnPickUp(mMushrooms[i])){ mMushrooms[i]->mDead = true; speedBonusTime += 5; } //PLAY SOUNDFX

		if (mMushrooms[i]->mDead)
		{
			delete mMushrooms[i];
			mMushrooms[i] = nullptr;
			mMushrooms.erase(mMushrooms.begin() + i);
		}
	}
}
void Engine::UpdateProjectiles(float dt)
{
	for (int i = 0; i < mProjectiles.size(); i++)
	{
		if (!ProjectileBounds(mProjectiles[i])){ mProjectiles[i]->mDead = true;}
		mProjectiles[i]->Walk(dt*1000);
		mProjectiles[i]->Update(mCam, dt);
	
		if (mProjectiles[i]->mDead)
		{
			delete mProjectiles[i];
			mProjectiles[i] = nullptr;
			mProjectiles.erase(mProjectiles.begin() + i);
		}
	}
}

void Engine::DrawScene()
{
	ClearScene();
	mSky->Draw(md3dImmediateContext, mCam); //Draw Sky First So The Z Buffer Takes Effect  ... Otherwise it Draws Over I


	md3dImmediateContext->IASetInputLayout(InputLayouts::Basic32);
    md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
 
	

	if( mWireMode )	md3dImmediateContext->RSSetState(RenderStates::WireframeRS);

	//SWITCH ON THE MODE THE GAME IS IN 
	switch (*StateMachine::pGameState)
	{
	case GameState::MAINMENU:	DrawMainMenu(); break;
	case GameState::ABOUT:		DrawAbout();	break;
	case GameState::PAUSED:		DrawPaused();	break;
	case GameState::WIN:		DrawWin();		break;
	case GameState::LOSE:		DrawLose();		break;
	case GameState::GAMEON:		DrawGameOn();	break;
	}

	
	// Draw particle systems last so it is blended with scene.
// 	mFire.SetEyePos(mCam.GetPosition());
// 	mFire.Draw(md3dImmediateContext, mCam);
// 	md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff); // restore default
	RestoreStates();
	HR(mSwapChain->Present(0, 0));
}


//HELPERS
void Engine::CameraHandler()
{
	// 
	// Clamp camera to terrain surface in walk mode.
	//
	if (mWalkCamMode)
	{
		XMFLOAT3 camPos = mCam.GetPosition();
		mCam.SetPosition(camPos.x, 50.0f, camPos.z);
	}

	mCam.UpdateViewMatrix();
}
void Engine::ClearScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Black));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void Engine::RestoreStates()
{
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	// restore default states.
	md3dImmediateContext->RSSetState(0);
	md3dImmediateContext->OMSetDepthStencilState(0, 0);
	md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
}
void Engine::ResetCamMainMenu()
{
	mCam.ResetCam();
	mCam.SetPosition(0.0f, 150.0f, -400.0f);
	mCam.Pitch(XM_PI / 6.5);
}
void Engine::ResetCamInGame()
{
	mCam.ResetCam();
	mCam.SetPosition(0.0f, 50.0f, 0.0f);
	//mCam.Pitch(XM_PI / 2);
	mWalkCamMode = true;
}
void Engine::IncProgress(float dt)
{
	(mCompBar->currProgress < 1.0) ? mCompBar->currProgress += dt / 100 : mCompBar->currProgress = 1.0;
	if (mCompBar->currProgress == 1.0){ *StateMachine::pGameState = GameState::WIN; }
}
void Engine::IncBugs(float bug)
{
	(mBugBar->currProgress < 1.0) ? mBugBar->currProgress += bug : mBugBar->currProgress = 1.0;
	if (mBugBar->currProgress == 1.0){ *StateMachine::pGameState = GameState::LOSE; }
}
bool Engine::CamOnPickUp(Button* pickup)
{
	//Convert from 0,0 at center of screen coordinates to 0,0 top left ...  cartesian to screen 
	float x, z;
	x = pickup->mPosition.x - pickup->mWidth / 2;
	z = pickup->mPosition.z - pickup->mWidth / 2;

	bool inX = false;
	bool inZ = false;

	if (mCam.GetPosition().x > x && mCam.GetPosition().x < x + pickup->mWidth)	{ inX = true; }
	if (mCam.GetPosition().z > z && mCam.GetPosition().z < z + pickup->mWidth)	{ inZ = true; }

	return(inX && inZ);
}
bool Engine::ProjectileBounds(Button* proj)
{
	float x, y, z, maxBound = 2000.0f;

	x = abs(proj->mPosition.x);
	y = abs(proj->mPosition.y);
	z = abs(proj->mPosition.z);

	return((x < maxBound) && (y < maxBound) && (z < maxBound));
}


//GAME INITS
void Engine::InitMainMenu()
{
	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, L"Textures/motherboard.dds", 0, 0, &mFloorTexSRV, 0));
	

	//MAKE BUTTONS
	mPlayButt		= new Button(md3dDevice,80.0f, 40.0f);
	mSoundButt		= new Button(md3dDevice, 40.0f, 20.0f);
	mMusicButt		= new Button(md3dDevice, 40.0f, 20.0f);
	mSOnButt		= new Button(md3dDevice, 40.0f, 20.0f);
	mSOffButt		= new Button(md3dDevice, 40.0f, 20.0f);
	mMOnButt		= new Button(md3dDevice, 40.0f, 20.0f);
	mMOffButt		= new Button(md3dDevice, 40.0f, 20.0f);
	mTitleButt		= new Button(md3dDevice, 140.0f, 50.0f);
	mAboutButt		= new Button(md3dDevice, 80.0f, 40.0f);
	mCompiledButt	= new Button(md3dDevice, 400.0f, 200.0f);
	mBymeButt		= new Button(md3dDevice, 110.0f, 30.0f);
	mBugsButt		= new Button(md3dDevice, 350.0f, 200.0f);
	mQuitButt		= new Button(md3dDevice, 350.0f, 200.0f);
	mRestartButt	= new Button(md3dDevice, 350.0f, 200.0f);
	mPausedButt		= new Button(md3dDevice, 600.0f, 300.0f);
	mBackButt		= new Button(md3dDevice, 80.0f, 40.0f);
	mAboutMsgButt	= new Button(md3dDevice, 110.0f, 110.0f);
	mYouWinButt		= new Button(md3dDevice, 350.0f, 200.0f);
	mYouLoseButt	= new Button(md3dDevice, 350.0f, 200.0f);
	mRetryButt		= new Button(md3dDevice, 350.0f, 200.0f);

	mPlayButt->LoadTexture(			md3dDevice, L"Textures/play.dds");
	mSoundButt->LoadTexture(		md3dDevice, L"Textures/sound.dds");
	mMusicButt->LoadTexture(		md3dDevice, L"Textures/music.dds");
	mSOnButt->LoadTexture(			md3dDevice, L"Textures/on.dds");
	mSOffButt->LoadTexture(			md3dDevice, L"Textures/off.dds");
	mMOnButt->LoadTexture(			md3dDevice, L"Textures/on.dds");
	mMOffButt->LoadTexture(			md3dDevice, L"Textures/off.dds");
	mTitleButt->LoadTexture(		md3dDevice, L"Textures/title.dds");
	mAboutButt->LoadTexture(		md3dDevice, L"Textures/about.dds");
	mCompiledButt->LoadTexture(		md3dDevice, L"Textures/compiled.dds");
	mBymeButt->LoadTexture(			md3dDevice, L"Textures/byme.dds");
	mBugsButt->LoadTexture(			md3dDevice, L"Textures/bugs.dds");
	mQuitButt->LoadTexture(			md3dDevice, L"Textures/quit.dds");
	mRestartButt->LoadTexture(		md3dDevice, L"Textures/restart.dds");
	mPausedButt->LoadTexture(		md3dDevice, L"Textures/paused.dds");
	mBackButt->LoadTexture(			md3dDevice, L"Textures/back.dds");
	mAboutMsgButt->LoadTexture(		md3dDevice, L"Textures/aboutmsg.dds");
	mYouWinButt->LoadTexture(		md3dDevice, L"Textures/youwin.dds");
	mYouLoseButt->LoadTexture(		md3dDevice, L"Textures/youlose.dds");
	mRetryButt->LoadTexture(		md3dDevice, L"Textures/retry.dds");


	mInvader = new Button(md3dDevice, 50.0f, 50.0f,false,true); 
	mInvader->LoadTexture(md3dDevice, L"Textures/invader.dds");


	//BINARY FLOORS ..Using Buttons .
	mNorthF			= new Button(md3dDevice,  2500.0f, 1000.0f);
	mSouthF			= new Button(md3dDevice,  2500.0f, 1000.0f);
	mWestF			= new Button(md3dDevice,  1000.0f, 500.0f);
	mEastF			= new Button(md3dDevice,  1000.0f, 500.0f);
	mNorthF->LoadTexture(md3dDevice, L"Textures/binary.dds");
	mSouthF->UseTexture(mNorthF->mTexSRV);
	mWestF->UseTexture(mNorthF->mTexSRV);
	mEastF->UseTexture(mNorthF->mTexSRV);

	mCompBar		= new Button(md3dDevice, 900.0f, 60.0f);
	mCompBarOL		= new Button(md3dDevice, 900.0f, 60.0f);
	mBugBar			= new Button(md3dDevice, 900.0f, 60.0f);
	mBugBarOL		= new Button(md3dDevice, 900.0f, 60.0f);
	mCompBar->LoadTexture(md3dDevice, L"Textures/compBar.dds");
	mCompBarOL->LoadTexture(md3dDevice, L"Textures/BarOL.dds");
	mBugBar->LoadTexture(md3dDevice, L"Textures/bugsBar.dds");
	mBugBarOL->LoadTexture(md3dDevice, L"Textures/BarOL.dds");

	//EXTRA INVADERS ONLY LOADED FOR SHARING THE TEXTURE.. 
	mInvader2 = new Button(md3dDevice, 50.0f, 50.0f, false, true);
	mInvader2->LoadTexture(md3dDevice, L"Textures/invader2.dds");
	mInvader3 = new Button(md3dDevice, 50.0f, 50.0f, false, true);
	mInvader3->LoadTexture(md3dDevice, L"Textures/invader3.dds");
	mInvader4 = new Button(md3dDevice, 50.0f, 50.0f, false, true);
	mInvader4->LoadTexture(md3dDevice, L"Textures/invader4.dds");
	mMushroom = new Button(md3dDevice, 20.0f, 20.0f, false, true);
	mMushroom->LoadTexture(md3dDevice, L"Textures/mushroom.dds");
	mProjectile = new Button(md3dDevice, 10.0f, 10.0f, true);
	mProjectile->LoadTexture(md3dDevice, L"Textures/diamondPlate.dds");

	//3D UI STUFF
	mPlayButt->SetPos(0.0f, 50.0f, -200.0f);
	mPlayButt->Pitch(XM_PI / 4.5);

	mSoundButt->SetPos(-140.0f, 50.0f, -200.0f);
	mSoundButt->Pitch(XM_PI / 4.5);

	mSOnButt->SetPos(-85.0f, 50.0f, -200.0f);
	mSOnButt->Pitch(XM_PI / 4.5);

	mSOffButt->SetPos(-85.0f, 50.0f, -200.0f);
	mSOffButt->Pitch(XM_PI / 4.5);

	mMusicButt->SetPos(-140.0f, 50.0f, -150.0f);
	mMusicButt->Pitch(XM_PI / 4.5);

	mMOnButt->SetPos(-85.0f, 50.0f, -150.0f);
	mMOnButt->Pitch(XM_PI / 4.5);

	mMOffButt->SetPos(-85.0f, 50.0f, -150.0f);
	mMOffButt->Pitch(XM_PI / 4.5);

	mTitleButt->SetPos(0.0f, 110.0f, -200.0f);
	mTitleButt->Pitch(XM_PI / 4.5);

	mAboutButt->SetPos(100.0f, 50.0f, -200.0f);
	mAboutButt->Pitch(XM_PI / 4.5);
	
	mBymeButt->SetPos(-120.0f, 50.0f, -180.0f);
	mBymeButt->Pitch(XM_PI / 4.5);

	mBackButt->SetPos(120.0f, 50.0f, -180.0f);
	mBackButt->Pitch(XM_PI / 4.5);

	mAboutMsgButt->SetPos(0.0f, 50.0f, -240.0f);
	mAboutMsgButt->Pitch(XM_PI / 4.5);

	//2D UI STUFF
	mRestartButt->SetPos(-700.0f, 100.0f, -90.0f);
	mRestartButt->Pitch(XM_PI / 4.5);

	mPausedButt->SetPos(0.0f, 150.0f, -90.0f);
	mPausedButt->Pitch(XM_PI / 4);



	mBugsButt->SetPos(-700.0f, -450.0f, -90.0f);
	mBugsButt->Pitch(XM_PI / 4);

	mCompiledButt->SetPos(-700.0f, 450.0f, 0.0f);
	mCompiledButt->Pitch(XM_PI / 4);

	mQuitButt->SetPos(700.0f, 100.0f, -90.0f);
	mQuitButt->Pitch(XM_PI / 4);

	mCompBar->SetPos(-30.0f, 450.0f, -90.0f);
	mCompBar->Pitch(XM_PI / 4);
	mCompBar->progressBar = true;

	mCompBarOL->SetPos(-30.0f, 450.0f, -90.0f);
	mCompBarOL->Pitch(XM_PI / 4);

	mBugBar->SetPos(-30.0f, -450.0f, -90.0f);
	mBugBar->Pitch(XM_PI / 4);
	mBugBar->progressBar = true;


	mBugBarOL->SetPos(-30.0f, -450.0f, -90.0f);
	mBugBarOL->Pitch(XM_PI / 4);


	mYouWinButt->SetPos(0.0f, 150.0f, -90.0f);
	mYouWinButt->Pitch(XM_PI / 4); 
	mYouLoseButt->SetPos(0.0f, 150.0f, -90.0f);
	mYouLoseButt->Pitch(XM_PI / 4);
	mRetryButt->SetPos(-700.0f, 100.0f, -90.0f);
	mRetryButt->Pitch(XM_PI / 4);

	mInvader->reverseLook = true;
	mInvader->SetPos(0.0f, 50.0f, 500.0f);
	mInvader->SetGoToPoint(0.0f, 50.0f, 0.0f);

	


	//FLOORS
	mNorthF->SetPos(0.0f, -0.1f, 750.0f);
	mSouthF->SetPos(0.0f, -0.1f, -750.0f);
	mWestF->SetPos(750.0f, -0.1f, 0.0f);
	mEastF->SetPos(-750.0f, -0.1f, 0.0f);

	mNorthF->origTexScale = 32.0f;
	mSouthF->origTexScale = 32.0f;
	mWestF->origTexScale =  12.0f;
	mEastF->origTexScale =  12.0f;

	float texSpeed = 0.01f;
	mNorthF->useTexTrans = true; mNorthF->texTransMult	= { 0.0f,		-texSpeed,		0.0f };
	mSouthF->useTexTrans = true; mSouthF->texTransMult	= { 0.0f,		texSpeed,		0.0f };
	mWestF->useTexTrans = true;  mWestF->texTransMult	= { texSpeed,	0.0f,			0.0f };
	mEastF->useTexTrans = true;  mEastF->texTransMult	= { -texSpeed,	0.0f,			0.0f };


	GeometryGenerator geoGen;

	//FLOOR PLANE
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mGridWorld, I);

	mGridMat.Ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mGridMat.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mGridMat.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 8.0f);

	GeometryGenerator::MeshData grid;

	geoGen.CreateGrid(500.0f, 500.0f, 2, 2, grid);

	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	mGridVertexOffset = 0; // FIRST OBJECT FOR OFFSET

	mPlayButt->SetVertexOffset(grid.Vertices.size());
	mSoundButt->SetVertexOffset(mPlayButt->GetVertOffset()		+ mPlayButt->mGrid.Vertices.size());
	mMusicButt->SetVertexOffset(mSoundButt->GetVertOffset()		+ mSoundButt->mGrid.Vertices.size());
	mSOnButt->SetVertexOffset(mMusicButt->GetVertOffset()		+ mMusicButt->mGrid.Vertices.size());
	mSOffButt->SetVertexOffset(mSOnButt->GetVertOffset()		+ mSOnButt->mGrid.Vertices.size());
	mMOnButt->SetVertexOffset(mSOffButt->GetVertOffset()		+ mSOffButt->mGrid.Vertices.size());
	mMOffButt->SetVertexOffset(mMOnButt->GetVertOffset()		+ mMOnButt->mGrid.Vertices.size());
	mTitleButt->SetVertexOffset(mMOffButt->GetVertOffset()		+ mMOffButt->mGrid.Vertices.size());
	mAboutButt->SetVertexOffset(mTitleButt->GetVertOffset()		+ mTitleButt->mGrid.Vertices.size());
	mCompiledButt->SetVertexOffset(mAboutButt->GetVertOffset()	+ mAboutButt->mGrid.Vertices.size());
	mBymeButt->SetVertexOffset(mCompiledButt->GetVertOffset()	+ mCompiledButt->mGrid.Vertices.size());
	mBugsButt->SetVertexOffset(mBymeButt->GetVertOffset()		+ mBymeButt->mGrid.Vertices.size());
	mQuitButt->SetVertexOffset(mBugsButt->GetVertOffset()		+ mBugsButt->mGrid.Vertices.size());
	mRestartButt->SetVertexOffset(mQuitButt->GetVertOffset()	+ mQuitButt->mGrid.Vertices.size());
	mPausedButt->SetVertexOffset(mRestartButt->GetVertOffset()	+ mRestartButt->mGrid.Vertices.size());
	mBackButt->SetVertexOffset(mPausedButt->GetVertOffset()		+ mPausedButt->mGrid.Vertices.size());
	mAboutMsgButt->SetVertexOffset(mBackButt->GetVertOffset()	+ mBackButt->mGrid.Vertices.size());
	mNorthF->SetVertexOffset(mAboutMsgButt->GetVertOffset()		+ mAboutMsgButt->mGrid.Vertices.size());
	mWestF->SetVertexOffset(mNorthF->GetVertOffset()			+ mNorthF->mGrid.Vertices.size());
	mEastF->SetVertexOffset(mWestF->GetVertOffset()				+ mWestF->mGrid.Vertices.size());
	mSouthF->SetVertexOffset(mEastF->GetVertOffset()			+ mEastF->mGrid.Vertices.size());
	mCompBar->SetVertexOffset(mSouthF->GetVertOffset()			+ mSouthF->mGrid.Vertices.size());
	mCompBarOL->SetVertexOffset(mCompBar->GetVertOffset()		+ mCompBar->mGrid.Vertices.size());
	mBugBar->SetVertexOffset(mCompBarOL->GetVertOffset()		+ mCompBarOL->mGrid.Vertices.size());
	mBugBarOL->SetVertexOffset(mBugBar->GetVertOffset()			+ mBugBar->mGrid.Vertices.size());
	mYouWinButt->SetVertexOffset(mBugBarOL->GetVertOffset()		+ mBugBarOL->mGrid.Vertices.size());
	mYouLoseButt->SetVertexOffset(mYouWinButt->GetVertOffset()	+ mYouWinButt->mGrid.Vertices.size());
	mRetryButt->SetVertexOffset(mYouLoseButt->GetVertOffset()	+ mYouLoseButt->mGrid.Vertices.size());
	mInvader->SetVertexOffset(mRetryButt->GetVertOffset()		+ mRetryButt->mGrid.Vertices.size());
	mMushroom->SetVertexOffset(mInvader->GetVertOffset()		+ mInvader->mGrid.Vertices.size());
	mProjectile->SetVertexOffset(mMushroom->GetVertOffset()		+ mMushroom->mGrid.Vertices.size());


	// Cache the index count of each object.
	mGridIndexCount = grid.Indices.size();

	mGridIndexOffset = 0;
	mPlayButt->SetIndexOffset(grid.Indices.size());
	mSoundButt->SetIndexOffset(		mPlayButt->GetIndOffset()		+ mPlayButt->mGrid.Indices.size());
	mMusicButt->SetIndexOffset(		mSoundButt->GetIndOffset()		+ mSoundButt->mGrid.Indices.size());
	mSOnButt->SetIndexOffset(		mMusicButt->GetIndOffset()		+ mMusicButt->mGrid.Indices.size());
	mSOffButt->SetIndexOffset(		mSOnButt->GetIndOffset()		+ mSOnButt->mGrid.Indices.size());
	mMOnButt->SetIndexOffset(		mSOffButt->GetIndOffset()		+ mSOffButt->mGrid.Indices.size());
	mMOffButt->SetIndexOffset(		mMOnButt->GetIndOffset()		+ mMOnButt->mGrid.Indices.size());
	mTitleButt->SetIndexOffset(		mMOffButt->GetIndOffset()		+ mMOffButt->mGrid.Indices.size());
	mAboutButt->SetIndexOffset(		mTitleButt->GetIndOffset()		+ mTitleButt->mGrid.Indices.size());
	mCompiledButt->SetIndexOffset(	mAboutButt->GetIndOffset()		+ mAboutButt->mGrid.Indices.size());
	mBymeButt->SetIndexOffset(		mCompiledButt->GetIndOffset()	+ mCompiledButt->mGrid.Indices.size());
	mBugsButt->SetIndexOffset(		mBymeButt->GetIndOffset()		+ mBymeButt->mGrid.Indices.size());
	mQuitButt->SetIndexOffset(		mBugsButt->GetIndOffset()		+ mBugsButt->mGrid.Indices.size());
	mRestartButt->SetIndexOffset(	mQuitButt->GetIndOffset()		+ mQuitButt->mGrid.Indices.size());
	mPausedButt->SetIndexOffset(	mRestartButt->GetIndOffset()	+ mRestartButt->mGrid.Indices.size());
	mBackButt->SetIndexOffset(		mPausedButt->GetIndOffset()		+ mPausedButt->mGrid.Indices.size());
	mAboutMsgButt->SetIndexOffset(	mBackButt->GetIndOffset()		+ mBackButt->mGrid.Indices.size());
	mNorthF->SetIndexOffset(		mAboutMsgButt->GetIndOffset()	+ mAboutMsgButt->mGrid.Indices.size());
	mWestF->SetIndexOffset(			mNorthF->GetIndOffset()			+ mNorthF->mGrid.Indices.size());
	mEastF->SetIndexOffset(			mWestF->GetIndOffset()			+ mWestF->mGrid.Indices.size());
	mSouthF->SetIndexOffset(		mEastF->GetIndOffset()			+ mEastF->mGrid.Indices.size());
	mCompBar->SetIndexOffset(		mSouthF->GetIndOffset()			+ mSouthF->mGrid.Indices.size());
	mCompBarOL->SetIndexOffset(		mCompBar->GetIndOffset()		+ mCompBar->mGrid.Indices.size());
	mBugBar->SetIndexOffset(		mCompBarOL->GetIndOffset()		+ mCompBarOL->mGrid.Indices.size());
	mBugBarOL->SetIndexOffset(		mBugBar->GetIndOffset()			+ mBugBar->mGrid.Indices.size());
	mYouWinButt->SetIndexOffset(	mBugBarOL->GetIndOffset()		+ mBugBarOL->mGrid.Indices.size());
	mYouLoseButt->SetIndexOffset(	mYouWinButt->GetIndOffset()		+ mYouWinButt->mGrid.Indices.size());
	mRetryButt->SetIndexOffset(		mYouLoseButt->GetIndOffset()	+ mYouLoseButt->mGrid.Indices.size());
	mInvader->SetIndexOffset(		mRetryButt->GetIndOffset()		+ mRetryButt->mGrid.Indices.size());
	mMushroom->SetIndexOffset(		mInvader->GetIndOffset()		+ mInvader->mGrid.Indices.size());
	mProjectile->SetIndexOffset(	mMushroom->GetIndOffset()		+ mMushroom->mGrid.Indices.size());


	UINT totalVertexCount = grid.Vertices.size()
		+ mPlayButt->mGrid.Vertices.size()
		+ mSoundButt->mGrid.Vertices.size()
		+ mMusicButt->mGrid.Vertices.size()
		+ mSOnButt->mGrid.Vertices.size()
		+ mSOffButt->mGrid.Vertices.size()
		+ mMOnButt->mGrid.Vertices.size()
		+ mMOffButt->mGrid.Vertices.size()
		+ mTitleButt->mGrid.Vertices.size()
		+ mAboutButt->mGrid.Vertices.size()
		+ mCompiledButt->mGrid.Vertices.size()
		+ mBymeButt->mGrid.Vertices.size()
		+ mBugsButt->mGrid.Vertices.size()
		+ mQuitButt->mGrid.Vertices.size()
		+ mRestartButt->mGrid.Vertices.size()
		+ mPausedButt->mGrid.Vertices.size()
		+ mBackButt->mGrid.Vertices.size()
		+ mAboutMsgButt->mGrid.Vertices.size()
		+ mNorthF->mGrid.Vertices.size()
		+ mWestF->mGrid.Vertices.size()
		+ mEastF->mGrid.Vertices.size()
		+ mSouthF->mGrid.Vertices.size()
		+ mCompBar->mGrid.Vertices.size()
		+ mCompBarOL->mGrid.Vertices.size()
		+ mBugBar->mGrid.Vertices.size()
		+ mBugBarOL->mGrid.Vertices.size()
		+ mYouWinButt->mGrid.Vertices.size()
		+ mYouLoseButt->mGrid.Vertices.size()
		+ mRetryButt->mGrid.Vertices.size()
		+ mInvader->mGrid.Vertices.size()
		+ mMushroom->mGrid.Vertices.size()
		+ mProjectile->mGrid.Vertices.size();

	UINT totalIndexCount = mGridIndexCount
		+ mPlayButt->mIndexCount
		+ mSoundButt->mIndexCount
		+ mMusicButt->mIndexCount
		+ mSOnButt->mIndexCount
		+ mSOffButt->mIndexCount
		+ mMOnButt->mIndexCount
		+ mMOffButt->mIndexCount
		+ mTitleButt->mIndexCount
		+ mAboutButt->mIndexCount
		+ mCompiledButt->mIndexCount
		+ mBymeButt->mIndexCount
		+ mBugsButt->mIndexCount
		+ mQuitButt->mIndexCount
		+ mRestartButt->mIndexCount
		+ mPausedButt->mIndexCount
		+ mBackButt->mIndexCount
		+ mAboutMsgButt->mIndexCount
		+ mNorthF->mIndexCount
		+ mWestF->mIndexCount
		+ mEastF->mIndexCount
		+ mSouthF->mIndexCount
		+ mCompBar->mIndexCount
		+ mCompBarOL->mIndexCount
		+ mBugBar->mIndexCount
		+ mBugBarOL->mIndexCount
		+ mYouWinButt->mIndexCount
		+ mYouLoseButt->mIndexCount
		+ mRetryButt->mIndexCount
		+ mInvader->mIndexCount
		+ mMushroom->mIndexCount
		+ mProjectile->mIndexCount;


	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//
	std::vector<Vertex::Basic32> vertices(totalVertexCount);
	UINT k = 0;
	for (size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = grid.Vertices[i].Position;
		vertices[k].Normal = grid.Vertices[i].Normal;
		vertices[k].Tex = grid.Vertices[i].TexC;
	}

//BUTTONS*******************************************************************
	mPlayButt->LoadVertData(	vertices, k);
	mSoundButt->LoadVertData(	vertices, k);
	mMusicButt->LoadVertData(	vertices, k);
	mSOnButt->LoadVertData(		vertices, k);
	mSOffButt->LoadVertData(	vertices, k);
	mMOnButt->LoadVertData(		vertices, k);
	mMOffButt->LoadVertData(	vertices, k);
	mTitleButt->LoadVertData(	vertices, k);
	mAboutButt->LoadVertData(	vertices, k);
	mCompiledButt->LoadVertData(vertices, k);
	mBymeButt->LoadVertData(	vertices, k);
	mBugsButt->LoadVertData(	vertices, k);
	mQuitButt->LoadVertData(	vertices, k);
	mRestartButt->LoadVertData(	vertices, k);
	mPausedButt->LoadVertData(	vertices, k);
	mBackButt->LoadVertData(	vertices, k);
	mAboutMsgButt->LoadVertData(vertices, k);
	mNorthF->LoadVertData(		vertices, k);
	mWestF->LoadVertData(		vertices, k);
	mEastF->LoadVertData(		vertices, k);
	mSouthF->LoadVertData(		vertices, k);
	mCompBar->LoadVertData(		vertices, k);
	mCompBarOL->LoadVertData(	vertices, k);
	mBugBar->LoadVertData(		vertices, k);
	mBugBarOL->LoadVertData(	vertices, k);
	mYouWinButt->LoadVertData(	vertices, k);
	mYouLoseButt->LoadVertData(	vertices, k);
	mRetryButt->LoadVertData(	vertices, k);
	mInvader->LoadVertData(		vertices, k);
	mMushroom->LoadVertData(	vertices, k);
	mProjectile->LoadVertData(	vertices, k);

	//****************************************************************************



	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Basic32) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mShapesVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//
	std::vector<UINT> indices;
	indices.insert(indices.end(), grid.Indices.begin(), grid.Indices.end());
	indices.insert(indices.end(), mPlayButt->mGrid.Indices.begin(),		mPlayButt->mGrid.Indices.end());
	indices.insert(indices.end(), mSoundButt->mGrid.Indices.begin(),	mSoundButt->mGrid.Indices.end());
	indices.insert(indices.end(), mMusicButt->mGrid.Indices.begin(),	mMusicButt->mGrid.Indices.end());
	indices.insert(indices.end(), mSOnButt->mGrid.Indices.begin(),		mSOnButt->mGrid.Indices.end());
	indices.insert(indices.end(), mSOffButt->mGrid.Indices.begin(),		mSOffButt->mGrid.Indices.end());
	indices.insert(indices.end(), mMOnButt->mGrid.Indices.begin(),		mMOnButt->mGrid.Indices.end());
	indices.insert(indices.end(), mMOffButt->mGrid.Indices.begin(),		mMOffButt->mGrid.Indices.end());
	indices.insert(indices.end(), mTitleButt->mGrid.Indices.begin(),	mTitleButt->mGrid.Indices.end());
	indices.insert(indices.end(), mAboutButt->mGrid.Indices.begin(),	mAboutButt->mGrid.Indices.end());
	indices.insert(indices.end(), mCompiledButt->mGrid.Indices.begin(), mCompiledButt->mGrid.Indices.end());
	indices.insert(indices.end(), mBymeButt->mGrid.Indices.begin(),		mBymeButt->mGrid.Indices.end());
	indices.insert(indices.end(), mBugsButt->mGrid.Indices.begin(),		mBugsButt->mGrid.Indices.end());
	indices.insert(indices.end(), mQuitButt->mGrid.Indices.begin(),		mQuitButt->mGrid.Indices.end());
	indices.insert(indices.end(), mRestartButt->mGrid.Indices.begin(),	mRestartButt->mGrid.Indices.end());
	indices.insert(indices.end(), mPausedButt->mGrid.Indices.begin(),	mPausedButt->mGrid.Indices.end());
	indices.insert(indices.end(), mBackButt->mGrid.Indices.begin(),		mBackButt->mGrid.Indices.end());
	indices.insert(indices.end(), mAboutMsgButt->mGrid.Indices.begin(), mAboutMsgButt->mGrid.Indices.end());
	indices.insert(indices.end(), mNorthF->mGrid.Indices.begin(),		mNorthF->mGrid.Indices.end());
	indices.insert(indices.end(), mWestF->mGrid.Indices.begin(),		mWestF->mGrid.Indices.end());
	indices.insert(indices.end(), mEastF->mGrid.Indices.begin(),		mEastF->mGrid.Indices.end());
	indices.insert(indices.end(), mSouthF->mGrid.Indices.begin(),		mSouthF->mGrid.Indices.end());
	indices.insert(indices.end(), mCompBar->mGrid.Indices.begin(),		mCompBar->mGrid.Indices.end());
	indices.insert(indices.end(), mCompBarOL->mGrid.Indices.begin(),	mCompBarOL->mGrid.Indices.end());
	indices.insert(indices.end(), mBugBar->mGrid.Indices.begin(),		mBugBar->mGrid.Indices.end());
	indices.insert(indices.end(), mBugBarOL->mGrid.Indices.begin(),		mBugBarOL->mGrid.Indices.end());
	indices.insert(indices.end(), mYouWinButt->mGrid.Indices.begin(),	mYouWinButt->mGrid.Indices.end());
	indices.insert(indices.end(), mYouLoseButt->mGrid.Indices.begin(),	mYouLoseButt->mGrid.Indices.end());
	indices.insert(indices.end(), mRetryButt->mGrid.Indices.begin(),	mRetryButt->mGrid.Indices.end());
	indices.insert(indices.end(), mInvader->mGrid.Indices.begin(),		mInvader->mGrid.Indices.end());
	indices.insert(indices.end(), mMushroom->mGrid.Indices.begin(),		mMushroom->mGrid.Indices.end());
	indices.insert(indices.end(), mProjectile->mGrid.Indices.begin(),	mProjectile->mGrid.Indices.end());
	

	//CREATE INDEX BUFFER
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mShapesIB));

}
void Engine::InitPaused()
{

}
void Engine::InitGameOn()
{

}
void Engine::InitWin()
{

}
void Engine::InitLose()
{

}



//GAME DRAWS
void Engine::DrawMainMenu()
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	mCam.UpdateViewMatrix();

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetEyePosW(mCam.GetPosition());

	// Figure out which technique to use.  Skull does not have texture coordinates,
	// so we need a separate technique for it.
	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light2TexAlphaClipTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	XMMATRIX world;
	XMMATRIX worldInvTranspose;
	XMMATRIX worldViewProj;
	activeTexTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);

		// Draw the grid.
		 world = XMLoadFloat4x4(&mGridWorld);
		 worldInvTranspose = MathHelper::InverseTranspose(world);
		 worldViewProj = world*view*proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
		Effects::BasicFX->SetMaterial(mGridMat);
		Effects::BasicFX->SetDiffuseMap(mFloorTexSRV);

		activeTexTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mGridIndexCount, mGridIndexOffset, mGridVertexOffset);

//DRAW BUTTS

		mPlayButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mSoundButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mMusicButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mTitleButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mAboutButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());

		if (*StateMachine::pSoundState == SoundState::SOUNDON){ mSOnButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime()); }
		if (*StateMachine::pSoundState == SoundState::SOUNDOFF){ mSOffButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime()); }
		if (*StateMachine::pMusicState == MusicState::MUSICON){ mMOnButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime()); }
		if (*StateMachine::pMusicState == MusicState::MUSICOFF){ mMOffButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime()); }


		mNorthF->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mWestF->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mEastF->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mSouthF->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());

	}

	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	XMMATRIX toTexSpace(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);
	stride = sizeof(Vertex::PosNormalTexTan);
	ID3DX11EffectTechnique* tech = Effects::BasicFX->Light3TexTech;
	tech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		for (UINT modelIndex = 0; modelIndex < mModelInstances.size(); ++modelIndex)
		{
			world = XMLoadFloat4x4(&mModelInstances[modelIndex].World);
			worldInvTranspose = MathHelper::InverseTranspose(world);
			worldViewProj = world*view*proj;

			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetWorldViewProjTex(worldViewProj*toTexSpace);
			Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));

			for (UINT subset = 0; subset < mModelInstances[modelIndex].Model->SubsetCount; ++subset)
			{
				Effects::BasicFX->SetMaterial(mModelInstances[modelIndex].Model->Mat[subset]);
				Effects::BasicFX->SetDiffuseMap(mModelInstances[modelIndex].Model->DiffuseMapSRV[subset]);
				//Effects::BasicFX->SetNormalMap(mModelInstances[modelIndex].Model->NormalMapSRV[subset]);

				tech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
				mModelInstances[modelIndex].Model->ModelMesh.Draw(md3dImmediateContext, subset);
			}
		}
	}
	RestoreStates();

	//SHADOWS *******************************************************************************************************

	//set shadow states
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	md3dImmediateContext->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);
	md3dImmediateContext->OMSetDepthStencilState(RenderStates::NoDoubleBlendDSS, 0); // changing 0 means overlaping draws
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);

	//Set Shadow Constants
	XMVECTOR shadowPlane = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // xz plane
	XMVECTOR toMainLight = -XMLoadFloat3(&mDirLights[0].Direction);
	XMMATRIX S = XMMatrixShadow(shadowPlane, toMainLight);
	float xOffSet = 43.0f; float zOffSet = 90.0f; float yOffSet = 0.1f; float sScale = 0.15f;

	mPlayButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mSoundButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mMusicButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mSOnButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mSOffButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mMOnButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mMOffButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mTitleButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet + 100.0f, mCam, mShadowMat);
	mAboutButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);

	// Restore default states.
	RestoreStates();

	if(!fullyLoaded)fullyLoaded = true;
}
void Engine::DrawAbout()
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	mCam.UpdateViewMatrix();

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetEyePosW(mCam.GetPosition());

	// Figure out which technique to use.  Skull does not have texture coordinates,
	// so we need a separate technique for it.
	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light2TexAlphaClipTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTexTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);

		// Draw the grid.
		XMMATRIX world = XMLoadFloat4x4(&mGridWorld);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
		Effects::BasicFX->SetMaterial(mGridMat);
		Effects::BasicFX->SetDiffuseMap(mFloorTexSRV);

		activeTexTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mGridIndexCount, mGridIndexOffset, mGridVertexOffset);

		//DRAW BUTTS

		mAboutMsgButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mTitleButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mBackButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mBymeButt->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());

		mNorthF->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mWestF->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mEastF->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mSouthF->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
	}

	//SHADOWS *******************************************************************************************************

	//set shadow states
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	md3dImmediateContext->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);
	md3dImmediateContext->OMSetDepthStencilState(RenderStates::NoDoubleBlendDSS, 0); // changing 0 means overlaping draws
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);

	//Set Shadow Constants
	XMVECTOR shadowPlane = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // xz plane
	XMVECTOR toMainLight = -XMLoadFloat3(&mDirLights[0].Direction);
	XMMATRIX S = XMMatrixShadow(shadowPlane, toMainLight);
	float xOffSet = 43.0f; float zOffSet = 90.0f; float yOffSet = 0.1f; float sScale = 0.1f;

	mAboutMsgButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mTitleButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet+100.0f, mCam, mShadowMat);
	mBackButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mBymeButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);

	// Restore default states.
	RestoreStates();
}
void Engine::DrawPaused()
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	mCam.UpdateViewMatrix();

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetEyePosW(mCam.GetPosition());

	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light1TexAlphaClipTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTexTech->GetDesc(&techDesc);
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mPausedButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		mQuitButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		mRestartButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
	}

	RestoreStates();
	DrawGameOn();
}
void Engine::DrawGameOn()
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	mCam.UpdateViewMatrix();

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetEyePosW(mCam.GetPosition());

	// Figure out which technique to use.  Skull does not have texture coordinates,
	// so we need a separate technique for it.
	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light1TexAlphaClipTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTexTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);

		// Draw the grid.
		XMMATRIX world = XMLoadFloat4x4(&mGridWorld);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
		Effects::BasicFX->SetMaterial(mGridMat);
		Effects::BasicFX->SetDiffuseMap(mFloorTexSRV);

		activeTexTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mGridIndexCount, mGridIndexOffset, mGridVertexOffset);











		mNorthF->Draw(			activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mWestF->Draw(			activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mEastF->Draw(			activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		mSouthF->Draw(			activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());

		for (int i = 0; i < mInvaders.size(); i++)
		{
			mInvaders[i]->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		}
		
		for (int i = 0; i < mMushrooms.size(); i++)
		{
			mMushrooms[i]->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		}

		for (int i = 0; i < mProjectiles.size(); i++)
		{
			mProjectiles[i]->Draw(activeTexTech, md3dImmediateContext, p, mCam, mTimer.DeltaTime());
		}

		//DRAW BUTTS
		activeTexTech = Effects::BasicFX->Light2TexAlphaClipTech;
		md3dImmediateContext->OMSetDepthStencilState(RenderStates::ZBufferDisabled, 0); // changing 0 means overlaping draws

		mBugsButt->Draw2D(		activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		mCompiledButt->Draw2D(	activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		mCompBar->Draw2D(		activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		mCompBarOL->Draw2D(		activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		mBugBar->Draw2D(		activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		mBugBarOL->Draw2D(		activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);


	}

	RestoreStates();
}
void Engine::DrawWin()
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	mCam.UpdateViewMatrix();

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetEyePosW(mCam.GetPosition());

	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light1TexAlphaClipTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTexTech->GetDesc(&techDesc);
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mYouWinButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		mQuitButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		mRestartButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
	}

	RestoreStates();
	DrawGameOn();
}
void Engine::DrawLose()
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	mCam.UpdateViewMatrix();

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetEyePosW(mCam.GetPosition());

	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light1TexAlphaClipTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTexTech->GetDesc(&techDesc);
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mShapesVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mShapesIB, DXGI_FORMAT_R32_UINT, 0);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mYouLoseButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		mQuitButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
		mRetryButt->Draw2D(activeTexTech, md3dImmediateContext, p, mCam, mOrthoWorld);
	}

	RestoreStates();
	DrawGameOn();
}



//KEYBOARD/MOUSE
void Engine::OnMouseDown(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		mLastMousePos.x = x;
		mLastMousePos.y = y;
		

		switch (*StateMachine::pGameState)
		{
		case GameState::MAINMENU:	BtnsMainMenu(x, y, true);		break;
		case GameState::ABOUT:		BtnsAbout(x, y, true);			break;
		case GameState::PAUSED:		BtnsPaused(x, y, true);			break;
		case GameState::WIN:		BtnsWin(x, y, true);			break;
		case GameState::LOSE:		BtnsLose(x, y, true);			break;
		case GameState::GAMEON:		BtnsGameOn(x, y, true);			break;
		}

		SetCapture(mhMainWnd);
	}
}
void Engine::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}
void Engine::OnMouseMove(WPARAM btnState, int x, int y)
{
	if (fullyLoaded)
	{
		switch (*StateMachine::pGameState)
		{
		case GameState::MAINMENU:	BtnsMainMenu(x, y, false);		break;
		case GameState::ABOUT:		BtnsAbout(x, y, false);			break;
		case GameState::PAUSED:		BtnsPaused(x, y, false);		break;
		case GameState::WIN:		BtnsWin(x, y, false);			break;
		case GameState::LOSE:		BtnsLose(x, y, false);			break;
		case GameState::GAMEON:		BtnsGameOn(x, y, false);		break;
		}
	}
	if (*StateMachine::pGameState == GameState::GAMEON)
	{
// 		if ((btnState & MK_LBUTTON) != 0)
// 		{
			// Make each pixel correspond to a quarter of a degree.
			float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
			float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));
	
			mCam.mPitch += dy; //Increment internal pitch 
	
			if (mCam.mPitch > -XM_PI/2 && mCam.mPitch < XM_PI/4){ mCam.Pitch(dy); } //LIMITS UP AND DOWN
			mCam.mPitch = MathHelper::Clamp(mCam.mPitch, -XM_PI / 2, XM_PI/4);
			
			mCam.RotateY(dx);
/*		}*/
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;

	//Mouse Wrap around
	if (*StateMachine::pGameState == GameState::GAMEON)
	{
		if (x <= 3)
		{
			SetCursorPos(mClientWidth, y);
			mLastMousePos.x = mClientWidth;
			mLastMousePos.y = y;
			return;
		}
		else if (x >= mClientWidth - 3)
		{
			SetCursorPos(5, y);
			mLastMousePos.x = 5;
			mLastMousePos.y = y;
			return;
		}
		
	} 


}
void Engine::OnKeyUP(WPARAM btnState)
{
	switch (btnState)
	{
	case 0x31:(mWireMode)	? mWireMode = false : mWireMode = true; break;	// 1 KEY
	case 0x32:(mBFCull)		? mBFCull	= false	: mBFCull	= true; break;	// 2 KEY
	case 0x50:if (*StateMachine::pGameState == GameState::GAMEON || *StateMachine::pGameState == GameState::PAUSED)
		{(*StateMachine::pGameState == GameState::PAUSED) ? *StateMachine::pGameState = GameState::GAMEON : *StateMachine::pGameState = GameState::PAUSED;}break;
	}
}
void Engine::KeyboardHandler(float dt)
{

	//
	// Control the camera.
	//
	if (*StateMachine::pGameState == GameState::GAMEON)
	{
		if (GetAsyncKeyState('W') & 0x8000)
			mCam.Walk(mMoveSpeed*dt);
	
		if (GetAsyncKeyState('S') & 0x8000)
			mCam.Walk(-mMoveSpeed*dt);
	
		if (GetAsyncKeyState('A') & 0x8000)
			mCam.Strafe(-mMoveSpeed*dt);
	
		if (GetAsyncKeyState('D') & 0x8000)
			mCam.Strafe(mMoveSpeed*dt);
	

	//
	// Walk/fly mode
	//
	if (GetAsyncKeyState('2') & 0x8000)
		mWalkCamMode = true;
	if (GetAsyncKeyState('3') & 0x8000)
		mWalkCamMode = false;

	}
	//
	// Reset particle systems.
	//
	// 	if (GetAsyncKeyState('R') & 0x8000)
	// 	{
	// 		mFire.Reset();
	// 		mRain.Reset();
	// 	}
}




//BUTTON HANDLERS
void Engine::BtnsMainMenu(float x, float y, bool clicked)
{
	if (InButton3D(x, y, mPlayButt))
	{	
		mPlayButt->hovering = true;
		if (clicked)
		{
			*StateMachine::pGameState = GameState::GAMEON;
			ResetCamInGame();
		}
	}
	else{ mPlayButt->hovering = false; }

	if (InButton3D(x, y, mAboutButt))
	{
		mAboutButt->hovering = true;
		if (clicked)
		{
			*StateMachine::pGameState = GameState::ABOUT;
		}
	}
	else{ mAboutButt->hovering = false; }

	if (InButton3D(x, y, mSoundButt))
	{
		mSoundButt->hovering = true;
		if (clicked)
		{
			if (*StateMachine::pSoundState == SoundState::SOUNDON)
			{ 
				*StateMachine::pSoundState = SoundState::SOUNDOFF; 
			}
			else
			{
				*StateMachine::pSoundState = SoundState::SOUNDON;
			}
		}
	}
	else{ mSoundButt->hovering = false; }

	if (InButton3D(x, y, mMusicButt))
	{
		mMusicButt->hovering = true;
		if (clicked)
		{
			
			if (*StateMachine::pMusicState == MusicState::MUSICON)
			{
				*StateMachine::pMusicState = MusicState::MUSICOFF;
			} 
			else
			{
				*StateMachine::pMusicState = MusicState::MUSICON;
			}
		}
	}
	else{ mMusicButt->hovering = false; }

}
void Engine::BtnsAbout(float x, float y, bool clicked)
{
	if (InButton3D(x, y, mBackButt))
	{
		mBackButt->hovering = true;
		if (clicked)
		{
			*StateMachine::pGameState = GameState::MAINMENU;
		}
	}
	else{ mBackButt->hovering = false; }
}
void Engine::BtnsPaused(float x, float y, bool clicked)
{
	if (InButton2D(x, y, mQuitButt))
	{
		mQuitButt->hovering = true;
		if (clicked)
		{
			*StateMachine::pGameState = GameState::MAINMENU;
			mWalkCamMode = false;
			ResetCamMainMenu();
			
		}
	}
	else{ mQuitButt->hovering = false; }

	if (InButton2D(x, y, mRestartButt))
	{
		mRestartButt->hovering = true;
		if (clicked)
		{
			mCompBar->currProgress = 0.0f;
			mBugBar->currProgress = 0.0f;
			*StateMachine::pGameState = GameState::GAMEON;
			ResetCamInGame();
		}
	}
	else{ mRestartButt->hovering = false; }
}
void Engine::BtnsGameOn(float x, float y, bool clicked)
{
	if (clicked){ SpawnProjectile(); }

// 	if (InButton3D(m_ScreenWidth/2, m_ScreenHeight/2, mInvader))
// 	{
// 		mInvader->hovering = true;
// 		if (clicked)
// 		{
// 
// 
// 		}
// 	}
// 	else{ mInvader->hovering = false; }
}
void Engine::BtnsWin(float x, float y, bool clicked)
{
	if (InButton2D(x, y, mQuitButt))
	{
		mQuitButt->hovering = true;
		if (clicked)
		{
			mCompBar->currProgress = 0.0f;
			mBugBar->currProgress = 0.0f;
			mWalkCamMode = false;
			*StateMachine::pGameState = GameState::MAINMENU;
			ResetCamMainMenu();
		}
	}
	else{ mQuitButt->hovering = false; }

	if (InButton2D(x, y, mRestartButt))
	{
		mRestartButt->hovering = true;
		if (clicked)
		{
			mCompBar->currProgress = 0.0f;
			mBugBar->currProgress = 0.0f;
			*StateMachine::pGameState = GameState::GAMEON;
		}
	}
	else{ mRestartButt->hovering = false; }
}
void Engine::BtnsLose(float x, float y, bool clicked)
{
	if (InButton2D(x, y, mQuitButt))
	{
		mQuitButt->hovering = true;
		if (clicked)
		{
			mCompBar->currProgress = 0.0f;
			mBugBar->currProgress = 0.0f;
			mWalkCamMode = false;
			*StateMachine::pGameState = GameState::MAINMENU;
			ResetCamMainMenu();
		}
	}
	else{ mQuitButt->hovering = false; }

	if (InButton2D(x, y, mRetryButt))
	{
		mRetryButt->hovering = true;
		if (clicked)
		{
			mCompBar->currProgress = 0.0f;
			mBugBar->currProgress = 0.0f;
			*StateMachine::pGameState = GameState::GAMEON;
		}
	}
	else{ mRetryButt->hovering = false; }
}
bool Engine::InButton3D(float x, float y, Button* button)
{
	XMMATRIX P = mCam.Proj();

	// Compute picking ray in view space.
	int newWidth, newHeight;
	float fs;
	if (mFullScreen){ newWidth = m_ScreenWidth; newHeight = m_ScreenHeight; fs = 1.0f; }
	else
	{
		fs = 1.0f;
		newWidth	= mClientWidth   ;
		newHeight	= mClientHeight  ;
	}

	float vx = (+2.0f*x / newWidth - fs) / P(0, 0);
	float vy = (-2.0f*y / newHeight + fs) / P(1, 1);


	// Ray definition in view space.
	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

	// Tranform ray to local space of Mesh.
	XMMATRIX V = mCam.View();
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);

	XMMATRIX W = XMLoadFloat4x4(&button->mWorld);
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	rayDir = XMVector3TransformNormal(rayDir, toLocal);

	// Make the ray direction unit length for the intersection tests.
	rayDir = XMVector3Normalize(rayDir);

	float tmin = 0.0f; // The Returned Distance
	if (XNA::IntersectRayAxisAlignedBox(rayOrigin, rayDir, &button->mMeshBox, &tmin))
	{
		//WE ARE IN THE MESH .. DO WHATEVER YOU WANT
		return true;
	}
	return false;
}
bool Engine::InButton2D(float sx, float sy, Button* button)
{
	//Convert from 0,0 at center of screen coordinates to 0,0 top left ...  cartesian to screen 
	float x,y;
	x = (button->mPosition.x - button->mWidth/2) + (mClientWidth / 2);
	y = (mClientHeight / 2) - (button->mPosition.y + button->mHeight/2);


	bool inX = false;
	bool inY = false;

 	if (sx > x && sx < x + button->mWidth)	{ inX = true; }
 	if (sy > y && sy < y + button->mHeight)	{ inY = true; }


	return(inX && inY);
}



//SPAWNERS 
void Engine::SpawnBug()
{
	//Varrying spawn sides.. 1000z or -1000z then Rand -1000 to 1000 on X orrrr
	//						 1000x or -1000x and  Rand -1000 to 1000 on the Z ..
	float outSkirtZ;
	float outSkirtX;
	if (MathHelper::RandF() > 0.5)
	{
		MathHelper::RandF() > 0.5 ? outSkirtZ = -1000 : outSkirtZ = 1000;
		outSkirtX = MathHelper::RandF(-1000.0f, 1000.0f);
	}
	else
	{
		MathHelper::RandF() > 0.5 ? outSkirtX = -1000 : outSkirtX = 1000;
		outSkirtZ = MathHelper::RandF(-1000.0f, 1000.0f);
	}
//	float randSize = MathHelper::RandF(10.0f, 50.0f); // TODO... IMPLEMENT A RANDOM SCALE SIZE;
	Button* Invader = new Button(md3dDevice, 50.0f, 50.0f, false, true);

	float num = MathHelper::RandF();
	num > 0.75 ? Invader->UseTexture(mInvader->mTexSRV)	 :
	num > 0.50 ? Invader->UseTexture(mInvader2->mTexSRV) :
	num > 0.25 ? Invader->UseTexture(mInvader3->mTexSRV) :
	Invader->UseTexture(mInvader4->mTexSRV);

	Invader->reverseLook = true;

	Invader->SetPos(outSkirtX, 30.0f, outSkirtZ);

	Invader->SetGoToPoint(0.0f, 30.0f, 0.0f); // Go to Center 
	
	Invader->SetVertexOffset(mInvader->GetVertOffset());
	Invader->SetIndexOffset(mInvader->mIndexOffset);
	Invader->mIndexCount = mInvader->mIndexCount;
	Invader->mMeshBox = mInvader->mMeshBox;
	mInvaders.push_back(Invader);
	IncBugs(0.03);
}
void Engine::SpawnMushroom()
{
	float outSkirtZ;
	float outSkirtX;

	outSkirtX = MathHelper::RandF(-500.0f, 500.0f);
	outSkirtZ = MathHelper::RandF(-500.0f, 500.0f);
	
	Button* Mush = new Button(md3dDevice, 50.0f, 50.0f, false, true);
	Mush->UseTexture(mMushroom->mTexSRV);
	Mush->reverseLook = true;
	Mush->SetPos(outSkirtX, 20.0f, outSkirtZ);

	Mush->SetVertexOffset(mMushroom->GetVertOffset());
	Mush->SetIndexOffset(mMushroom->mIndexOffset);
	Mush->mIndexCount = mMushroom->mIndexCount;
	Mush->mMeshBox = mMushroom->mMeshBox;
	mMushrooms.push_back(Mush);
}
void Engine::SpawnProjectile()
{
	float outSkirtZ;
	float outSkirtX;

	outSkirtX = mCam.GetPosition().x;
	outSkirtZ = mCam.GetPosition().z;

	Button* Proj = new Button(md3dDevice, 10.0f, 10.0f, true);
	Proj->UseTexture(mProjectile->mTexSRV);
	Proj->reverseLook = true;
	Proj->SetPos(outSkirtX, mCam.GetPosition().y-15.0f, outSkirtZ);

	Proj->SetVertexOffset(mProjectile->GetVertOffset());
	Proj->SetIndexOffset(mProjectile->mIndexOffset);
	Proj->mIndexCount = mProjectile->mIndexCount;
 	Proj->useTexTrans = true;
	Proj->texTransMult = { 1.0f, 0.0f, 0.0f };
	Proj->mLook = mCam.GetLook();
	
 	Proj->origTexScale = 2.0f;
	Proj->mMeshBox = mProjectile->mMeshBox;
	mProjectiles.push_back(Proj);
}