#include "Engine.h"
//TODO: make back Button ....Make A Button Able To Have a Start And End Position and it Goes to it ..
/*Clamp Cursor to Center of screen ... its stopping half way when turning */




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
	fullyLoaded(false)
{
	mMainWndCaption = L"Motherboard Meltdown";
	mEnable4xMsaa = false;

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	mCam.SetPosition(0.0f, 150.0f, -350.0f);
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
}

Engine::~Engine()
{
	if (md3dImmediateContext){ md3dImmediateContext->ClearState(); }
	
	ReleaseCOM(mRandomTexSRV);
	ReleaseCOM(mFlareTexSRV);
	ReleaseCOM(mRainTexSRV);

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

	InputLayouts::InitAll(md3dDevice);
	RenderStates::InitAll(md3dDevice);


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

	
}

void Engine::DrawScene()
{
	ClearScene();

	md3dImmediateContext->IASetInputLayout(InputLayouts::Basic32);
    md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
 
	if( mWireMode )	md3dImmediateContext->RSSetState(RenderStates::WireframeRS);

	//SWITCH ON THE MODE THE GAME IS IN 
	switch (*StateMachine::pGameState)
	{
	case GameState::MAINMENU:	DrawMainMenu(); break;
	case GameState::ABOUT:		DrawAbout(); break;
	case GameState::PAUSED:		DrawPaused();	break;
	case GameState::WIN:		DrawWin();		break;
	case GameState::LOSE:		DrawLose();		break;
	case GameState::GAMEON:		DrawGameOn();	break;
	}

	mSky->Draw(md3dImmediateContext, mCam);
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




//GAME INITS
void Engine::InitMainMenu()
{
	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, L"Textures/motherboard.dds", 0, 0, &mFloorTexSRV, 0));
	
	//MAKE BUTTONS
	mPlayButt		= new Button(md3dDevice, L"Textures/play.dds",		80.0f, 40.0f);
	mSoundButt		= new Button(md3dDevice, L"Textures/sound.dds",		40.0f, 20.0f);
	mMusicButt		= new Button(md3dDevice, L"Textures/music.dds",		40.0f, 20.0f);
	mSOnButt		= new Button(md3dDevice, L"Textures/on.dds",		40.0f, 20.0f);
	mSOffButt		= new Button(md3dDevice, L"Textures/off.dds",		40.0f, 20.0f);
	mMOnButt		= new Button(md3dDevice, L"Textures/on.dds",		40.0f, 20.0f);
	mMOffButt		= new Button(md3dDevice, L"Textures/off.dds",		40.0f, 20.0f);
	mTitleButt		= new Button(md3dDevice, L"Textures/title.dds",		140.0f, 50.0f);
	mAboutButt		= new Button(md3dDevice, L"Textures/about.dds",		80.0f, 40.0f);

	mCompiledButt	= new Button(md3dDevice, L"Textures/compiled.dds",	80.0f, 40.0f);
	mBymeButt		= new Button(md3dDevice, L"Textures/byme.dds",		110.0f, 30.0f);
	mBugsButt		= new Button(md3dDevice, L"Textures/bugs.dds",		80.0f, 40.0f);
	mQuitButt		= new Button(md3dDevice, L"Textures/quit.dds",		80.0f, 40.0f);
	mRestartButt	= new Button(md3dDevice, L"Textures/restart.dds",	80.0f, 40.0f);
	mPausedButt		= new Button(md3dDevice, L"Textures/paused.dds",	80.0f, 40.0f);
	mBackButt		= new Button(md3dDevice, L"Textures/back.dds",		80.0f, 40.0f);
	mAboutMsgButt	= new Button(md3dDevice, L"Textures/aboutmsg.dds",	110.0f, 110.0f);

	mPlayButt->SetPos(0.0f, 100.0f, -90.0f);
	mPlayButt->Pitch(XM_PI / 4.5);

	mSoundButt->SetPos(-140.0f, 100.0f, -90.0f);
	mSoundButt->Pitch(XM_PI / 4.5);

	mSOnButt->SetPos(-85.0f, 100.0f, -90.0f);
	mSOnButt->Pitch(XM_PI / 4.5);

	mSOffButt->SetPos(-85.0f, 100.0f, -90.0f);
	mSOffButt->Pitch(XM_PI / 4.5);

	mMusicButt->SetPos(-140.0f, 100.0f, -50.0f);
	mMusicButt->Pitch(XM_PI / 4.5);

	mMOnButt->SetPos(-85.0f, 100.0f, -50.0f);
	mMOnButt->Pitch(XM_PI / 4.5);

	mMOffButt->SetPos(-85.0f, 100.0f, -50.0f);
	mMOffButt->Pitch(XM_PI / 4.5);

	mTitleButt->SetPos(0.0f, 170.0f, -40.0f);
	mTitleButt->Pitch(XM_PI / 4.5);

	mAboutButt->SetPos(100.0f, 100.0f, -90.0f);
	mAboutButt->Pitch(XM_PI / 4.5);
	


	mBymeButt->SetPos(-130.0f, 100.0f, -90.0f);
	mBymeButt->Pitch(XM_PI / 4.5);

	mRestartButt->SetPos(-100.0f, 100.0f, -90.0f);
	mRestartButt->Pitch(XM_PI / 4.5);

	mPausedButt->SetPos(0.0f, 150.0f, -90.0f);
	mPausedButt->Pitch(XM_PI / 4.5);

	mBackButt->SetPos(120.0f, 100.0f, -90.0f);
	mBackButt->Pitch(XM_PI / 4.5);

	mAboutMsgButt->SetPos(0.0f, 160.0f, -115.0f);
	mAboutMsgButt->Pitch(XM_PI / 4.5);

	mBugsButt->SetPos(-100.0f, 100.0f, -90.0f);
	mBugsButt->Pitch(XM_PI / 4.5);

	mCompiledButt->SetPos(-100.0f, 150.0f, -10.0f);
	mCompiledButt->Pitch(XM_PI / 4.5);

	mQuitButt->SetPos(100.0f, 100.0f, -90.0f);
	mQuitButt->Pitch(XM_PI / 4.5);

	GeometryGenerator geoGen;

	//FLOOR PLANE
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mGridWorld, I);

	//XMMATRIX transform = XMMatrixRotationX(XM_PI/2);
	//XMMATRIX transform2 = XMMatrixRotationY(XM_PI);
	//XMStoreFloat4x4(&mGridWorld, transform*transform2);

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
		+ mAboutMsgButt->mGrid.Vertices.size();

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
		+ mAboutMsgButt->mIndexCount;

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
	indices.insert(indices.end(), mPlayButt->mGrid.Indices.begin(), mPlayButt->mGrid.Indices.end());
	indices.insert(indices.end(), mSoundButt->mGrid.Indices.begin(), mSoundButt->mGrid.Indices.end());
	indices.insert(indices.end(), mMusicButt->mGrid.Indices.begin(), mMusicButt->mGrid.Indices.end());
	indices.insert(indices.end(), mSOnButt->mGrid.Indices.begin(), mSOnButt->mGrid.Indices.end());
	indices.insert(indices.end(), mSOffButt->mGrid.Indices.begin(), mSOffButt->mGrid.Indices.end());
	indices.insert(indices.end(), mMOnButt->mGrid.Indices.begin(), mMOnButt->mGrid.Indices.end());
	indices.insert(indices.end(), mMOffButt->mGrid.Indices.begin(), mMOffButt->mGrid.Indices.end());
	indices.insert(indices.end(), mTitleButt->mGrid.Indices.begin(), mTitleButt->mGrid.Indices.end());
	indices.insert(indices.end(), mAboutButt->mGrid.Indices.begin(), mAboutButt->mGrid.Indices.end());
	indices.insert(indices.end(), mCompiledButt->mGrid.Indices.begin(), mCompiledButt->mGrid.Indices.end());
	indices.insert(indices.end(), mBymeButt->mGrid.Indices.begin(), mBymeButt->mGrid.Indices.end());
	indices.insert(indices.end(), mBugsButt->mGrid.Indices.begin(), mBugsButt->mGrid.Indices.end());
	indices.insert(indices.end(), mQuitButt->mGrid.Indices.begin(), mQuitButt->mGrid.Indices.end());
	indices.insert(indices.end(), mRestartButt->mGrid.Indices.begin(), mRestartButt->mGrid.Indices.end());
	indices.insert(indices.end(), mPausedButt->mGrid.Indices.begin(), mPausedButt->mGrid.Indices.end());
	indices.insert(indices.end(), mBackButt->mGrid.Indices.begin(), mBackButt->mGrid.Indices.end());
	indices.insert(indices.end(), mAboutMsgButt->mGrid.Indices.begin(), mAboutMsgButt->mGrid.Indices.end());

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

//DRAW BUTTS

		mPlayButt->Draw(activeTexTech, md3dImmediateContext, p, mCam);
		mSoundButt->Draw(activeTexTech, md3dImmediateContext, p, mCam);
		mMusicButt->Draw(activeTexTech, md3dImmediateContext, p, mCam);
		mTitleButt->Draw(activeTexTech, md3dImmediateContext, p, mCam);
		mAboutButt->Draw(activeTexTech, md3dImmediateContext, p, mCam);

		if (*StateMachine::pSoundState == SoundState::SOUNDON){mSOnButt->Draw(activeTexTech, md3dImmediateContext, p, mCam);}
		if (*StateMachine::pSoundState == SoundState::SOUNDOFF){mSOffButt->Draw(activeTexTech, md3dImmediateContext, p, mCam);}
		if (*StateMachine::pMusicState == MusicState::MUSICON){mMOnButt->Draw(activeTexTech, md3dImmediateContext, p, mCam);}
		if (*StateMachine::pMusicState == MusicState::MUSICOFF){mMOffButt->Draw(activeTexTech, md3dImmediateContext, p, mCam);}

		// 		mSoundButt
		// 		mMusicButt
		// 		mSOnButt
		// 		mSOffButt
		// 		mMOnButt
		// 		mMOffButt
		// 		mTitleButt
		// 		mAboutButt
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
	float xOffSet = 43.0f; float zOffSet = 90.0f; float yOffSet = 0.1f; float sScale = 0.15f;

	mPlayButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mSoundButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mMusicButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mSOnButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mSOffButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mMOnButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mMOffButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
	mTitleButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
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

		//DRAW BUTTS

		mAboutMsgButt->Draw(activeTexTech, md3dImmediateContext, p, mCam);
		mTitleButt->Draw(activeTexTech, md3dImmediateContext, p, mCam);
		mBackButt->Draw(activeTexTech, md3dImmediateContext, p, mCam);
		mBymeButt->Draw(activeTexTech, md3dImmediateContext, p, mCam);


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
	mTitleButt->DrawShadow(activeTexTech, md3dImmediateContext, shadowPlane, toMainLight, S, sScale, xOffSet, yOffSet, zOffSet, mCam, mShadowMat);
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
		mPausedButt->Draw(activeTexTech, md3dImmediateContext, p, mCam);
		mQuitButt->Draw(activeTexTech, md3dImmediateContext, p, mCam);
		mRestartButt->Draw(activeTexTech, md3dImmediateContext, p, mCam);
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

		//DRAW BUTTS

		mBugsButt->Draw(activeTexTech, md3dImmediateContext, p, mCam);
		mCompiledButt->Draw(activeTexTech, md3dImmediateContext, p, mCam);

	}

	RestoreStates();
}
void Engine::DrawWin()
{

}
void Engine::DrawLose()
{

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
}
void Engine::OnKeyUP(WPARAM btnState)
{
	switch (btnState)
	{
	case 0x31:(mWireMode) ? mWireMode = false : mWireMode = true; break;	// 1 KEY
	case 0x32:(mBFCull) ? mBFCull = false : mBFCull = true; break;	// 2 KEY
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
			mCam.Walk(1300.0f*dt);
	
		if (GetAsyncKeyState('S') & 0x8000)
			mCam.Walk(-1300.0f*dt);
	
		if (GetAsyncKeyState('A') & 0x8000)
			mCam.Strafe(-1300.0f*dt);
	
		if (GetAsyncKeyState('D') & 0x8000)
			mCam.Strafe(1300.0f*dt);
	

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
	if (InButton(x, y, mPlayButt))
	{	
		mPlayButt->hovering = true;
		if (clicked)
		{
			*StateMachine::pGameState = GameState::GAMEON;
		}
	}
	else{ mPlayButt->hovering = false; }

	if (InButton(x, y, mAboutButt))
	{
		mAboutButt->hovering = true;
		if (clicked)
		{
			*StateMachine::pGameState = GameState::ABOUT;
		}
	}
	else{ mAboutButt->hovering = false; }

	if (InButton(x, y, mSoundButt))
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

	if (InButton(x, y, mMusicButt))
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
	if (InButton(x, y, mBackButt))
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
	if (InButton(x, y, mQuitButt))
	{
		mQuitButt->hovering = true;
		if (clicked)
		{
			*StateMachine::pGameState = GameState::MAINMENU;
		}
	}
	else{ mQuitButt->hovering = false; }

	if (InButton(x, y, mRestartButt))
	{
		mRestartButt->hovering = true;
		if (clicked)
		{
			*StateMachine::pGameState = GameState::GAMEON;
		}
	}
	else{ mRestartButt->hovering = false; }
}
void Engine::BtnsGameOn(float x, float y, bool clicked)
{

}
void Engine::BtnsWin(float x, float y, bool clicked)
{

}
void Engine::BtnsLose(float x, float y, bool clicked)
{

}
bool Engine::InButton(float x, float y, Button* button)
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

	float tmin = 0.0f;
	if (XNA::IntersectRayAxisAlignedBox(rayOrigin, rayDir, &button->mMeshBox, &tmin))
	{
		//WE ARE IN THE MESH .. DO WHATEVER YOU WANT
		return true;
	}
	return false;
}

