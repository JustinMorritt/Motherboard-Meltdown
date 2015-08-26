#include "Engine.h"
//TODO: make back Button , Set Up Buttons With a Nice Draw ... This Will come In Handy...



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

	
	mPlayButt->Update(mCam, dt);
	mSoundButt->Update(mCam, dt);
	mMusicButt->Update(mCam, dt);
	mSOnButt->Update(mCam, dt);
	mSOffButt->Update(mCam, dt);
	mMOnButt->Update(mCam, dt);
	mMOffButt->Update(mCam, dt);
	mTitleButt->Update(mCam, dt);
	mAboutButt->Update(mCam, dt);
	
	//mButton1->RotateY(mButton1->rotation);


	//UPDATE PARTICLE SYSTEMS
	mFire.Update(dt, mTimer.TotalTime());
	mRain.Update(dt, mTimer.TotalTime());

	
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

void Engine::CameraHandler()
{
	// 
	// Clamp camera to terrain surface in walk mode.
	//
	if (mWalkCamMode)
	{
		XMFLOAT3 camPos = mCam.GetPosition();
		float y = mTerrain.GetHeight(camPos.x, camPos.z);
		mCam.SetPosition(camPos.x, y + 2.0f, camPos.z);
	}

	mCam.UpdateViewMatrix();
}

void Engine::ClearScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Silver));
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

	mPlayButt->SetPos(0.0f, 100.0f, -90.0f);
	mPlayButt->Pitch(XM_PI / 4.5);

	mSoundButt->SetPos(-140.0f, 100.0f, -90.0f);
	mSoundButt->Pitch(XM_PI / 4.5);

	mSOnButt->SetPos(-100.0f, 100.0f, -90.0f);
	mSOnButt->Pitch(XM_PI / 4.5);

	mSOffButt->SetPos(-100.0f, 100.0f, -90.0f);
	mSOffButt->Pitch(XM_PI / 4.5);

	mMusicButt->SetPos(-140.0f, 100.0f, -50.0f);
	mMusicButt->Pitch(XM_PI / 4.5);

	mMOnButt->SetPos(-100.0f, 100.0f, -50.0f);
	mMOnButt->Pitch(XM_PI / 4.5);

	mMOffButt->SetPos(-100.0f, 100.0f, -50.0f);
	mMOffButt->Pitch(XM_PI / 4.5);

	mTitleButt->SetPos(0.0f, 170.0f, -40.0f);
	mTitleButt->Pitch(XM_PI / 4.5);

	mAboutButt->SetPos(100.0f, 100.0f, -90.0f);
	mAboutButt->Pitch(XM_PI / 4.5);
	

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
	mSoundButt->SetVertexOffset(mPlayButt->GetVertOffset()  + mPlayButt->mGrid.Vertices.size());
	mMusicButt->SetVertexOffset(mSoundButt->GetVertOffset() + mSoundButt->mGrid.Vertices.size());
	mSOnButt->SetVertexOffset(mMusicButt->GetVertOffset()   + mMusicButt->mGrid.Vertices.size());
	mSOffButt->SetVertexOffset(mSOnButt->GetVertOffset()	+ mSOnButt->mGrid.Vertices.size());
	mMOnButt->SetVertexOffset(mSOffButt->GetVertOffset()	+ mSOffButt->mGrid.Vertices.size());
	mMOffButt->SetVertexOffset(mMOnButt->GetVertOffset()	+ mMOnButt->mGrid.Vertices.size());
	mTitleButt->SetVertexOffset(mMOffButt->GetVertOffset()  + mMOffButt->mGrid.Vertices.size());
	mAboutButt->SetVertexOffset(mTitleButt->GetVertOffset() + mTitleButt->mGrid.Vertices.size());

	// Cache the index count of each object.
	mGridIndexCount = grid.Indices.size();

	mGridIndexOffset = 0;
	mPlayButt->SetIndexOffset(grid.Indices.size());
	mSoundButt->SetIndexOffset(		mPlayButt->GetIndOffset()	+ mPlayButt->mGrid.Indices.size());
	mMusicButt->SetIndexOffset(		mSoundButt->GetIndOffset()	+ mSoundButt->mGrid.Indices.size());
	mSOnButt->SetIndexOffset(		mMusicButt->GetIndOffset()	+ mMusicButt->mGrid.Indices.size());
	mSOffButt->SetIndexOffset(		mSOnButt->GetIndOffset()	+ mSOnButt->mGrid.Indices.size());
	mMOnButt->SetIndexOffset(		mSOffButt->GetIndOffset()	+ mSOffButt->mGrid.Indices.size());
	mMOffButt->SetIndexOffset(		mMOnButt->GetIndOffset()	+ mMOnButt->mGrid.Indices.size());
	mTitleButt->SetIndexOffset(		mMOffButt->GetIndOffset()	+ mMOffButt->mGrid.Indices.size());
	mAboutButt->SetIndexOffset(		mTitleButt->GetIndOffset()	+ mTitleButt->mGrid.Indices.size());



	UINT totalVertexCount = grid.Vertices.size()
		+ mPlayButt->mGrid.Vertices.size()
		+ mSoundButt->mGrid.Vertices.size()
		+ mMusicButt->mGrid.Vertices.size()
		+ mSOnButt->mGrid.Vertices.size()
		+ mSOffButt->mGrid.Vertices.size()
		+ mMOnButt->mGrid.Vertices.size()
		+ mMOffButt->mGrid.Vertices.size()
		+ mTitleButt->mGrid.Vertices.size()
		+ mAboutButt->mGrid.Vertices.size();
	
	UINT totalIndexCount = mGridIndexCount
		+ mPlayButt->mIndexCount
		+ mSoundButt->mIndexCount
		+ mMusicButt->mIndexCount
		+ mSOnButt->mIndexCount
		+ mSOffButt->mIndexCount
		+ mMOnButt->mIndexCount
		+ mMOffButt->mIndexCount
		+ mTitleButt->mIndexCount
		+ mAboutButt->mIndexCount;

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
	XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	XMVECTOR vMin = XMLoadFloat3(&vMinf3);
	XMVECTOR vMax = XMLoadFloat3(&vMaxf3);

	for (size_t i = 0; i < mPlayButt->mGrid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos		= mPlayButt->mGrid.Vertices[i].Position;
		vertices[k].Normal	= mPlayButt->mGrid.Vertices[i].Normal;
		vertices[k].Tex		= mPlayButt->mGrid.Vertices[i].TexC;

		//Copy Into The Buttons Messh For Future Collision Check 
		mPlayButt->mMeshVertices[i].Pos		= mPlayButt->mGrid.Vertices[i].Position;
		mPlayButt->mMeshVertices[i].Normal	= mPlayButt->mGrid.Vertices[i].Normal;

		XMVECTOR P = XMLoadFloat3(&mPlayButt->mMeshVertices[i].Pos);

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}
	XMStoreFloat3(&mPlayButt->mMeshBox.Center, 0.5f*(vMin + vMax));
	XMStoreFloat3(&mPlayButt->mMeshBox.Extents, 0.5f*(vMax - vMin));

//SOUNDBUTT*******************************************************************
	 vMin = XMLoadFloat3(&vMinf3);
	 vMax = XMLoadFloat3(&vMaxf3);

	for (size_t i = 0; i < mSoundButt->mGrid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = mSoundButt->mGrid.Vertices[i].Position;
		vertices[k].Normal = mSoundButt->mGrid.Vertices[i].Normal;
		vertices[k].Tex = mSoundButt->mGrid.Vertices[i].TexC;

		//Copy Into The Buttons Messh For Future Collision Check 
		mSoundButt->mMeshVertices[i].Pos = mSoundButt->mGrid.Vertices[i].Position;
		mSoundButt->mMeshVertices[i].Normal = mSoundButt->mGrid.Vertices[i].Normal;

		XMVECTOR P = XMLoadFloat3(&mSoundButt->mMeshVertices[i].Pos);

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}
	XMStoreFloat3(&mSoundButt->mMeshBox.Center, 0.5f*(vMin + vMax));
	XMStoreFloat3(&mSoundButt->mMeshBox.Extents, 0.5f*(vMax - vMin));

//MUSICBUTT*******************************************************************
	 vMin = XMLoadFloat3(&vMinf3);
	 vMax = XMLoadFloat3(&vMaxf3);

	for (size_t i = 0; i < mMusicButt->mGrid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = mMusicButt->mGrid.Vertices[i].Position;
		vertices[k].Normal = mMusicButt->mGrid.Vertices[i].Normal;
		vertices[k].Tex = mMusicButt->mGrid.Vertices[i].TexC;

		//Copy Into The Buttons Messh For Future Collision Check 
		mMusicButt->mMeshVertices[i].Pos = mMusicButt->mGrid.Vertices[i].Position;
		mMusicButt->mMeshVertices[i].Normal = mMusicButt->mGrid.Vertices[i].Normal;

		XMVECTOR P = XMLoadFloat3(&mMusicButt->mMeshVertices[i].Pos);

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}
	XMStoreFloat3(&mMusicButt->mMeshBox.Center, 0.5f*(vMin + vMax));
	XMStoreFloat3(&mMusicButt->mMeshBox.Extents, 0.5f*(vMax - vMin));

//SOUNDONBUTT*******************************************************************
	
		 vMin = XMLoadFloat3(&vMinf3);
		 vMax = XMLoadFloat3(&vMaxf3);
	
		for (size_t i = 0; i < mSOnButt->mGrid.Vertices.size(); ++i, ++k)
		{
			vertices[k].Pos = mSOnButt->mGrid.Vertices[i].Position;
			vertices[k].Normal = mSOnButt->mGrid.Vertices[i].Normal;
			vertices[k].Tex = mSOnButt->mGrid.Vertices[i].TexC;
	
			//Copy Into The Buttons Messh For Future Collision Check 
			mSOnButt->mMeshVertices[i].Pos = mSOnButt->mGrid.Vertices[i].Position;
			mSOnButt->mMeshVertices[i].Normal = mSOnButt->mGrid.Vertices[i].Normal;
	
			XMVECTOR P = XMLoadFloat3(&mSOnButt->mMeshVertices[i].Pos);
	
			vMin = XMVectorMin(vMin, P);
			vMax = XMVectorMax(vMax, P);
		}
		XMStoreFloat3(&mSOnButt->mMeshBox.Center, 0.5f*(vMin + vMax));
		XMStoreFloat3(&mSOnButt->mMeshBox.Extents, 0.5f*(vMax - vMin));
	

//SOUNDOFFBUTT*******************************************************************

		vMin = XMLoadFloat3(&vMinf3);
		vMax = XMLoadFloat3(&vMaxf3);
	
		for (size_t i = 0; i < mSOffButt->mGrid.Vertices.size(); ++i, ++k)
		{
			vertices[k].Pos = mSOffButt->mGrid.Vertices[i].Position;
			vertices[k].Normal = mSOffButt->mGrid.Vertices[i].Normal;
			vertices[k].Tex = mSOffButt->mGrid.Vertices[i].TexC;
	
			//Copy Into The Buttons Messh For Future Collision Check 
			mSOffButt->mMeshVertices[i].Pos = mSOffButt->mGrid.Vertices[i].Position;
			mSOffButt->mMeshVertices[i].Normal = mSOffButt->mGrid.Vertices[i].Normal;
	
			XMVECTOR P = XMLoadFloat3(&mSOffButt->mMeshVertices[i].Pos);
	
			vMin = XMVectorMin(vMin, P);
			vMax = XMVectorMax(vMax, P);
		}
		XMStoreFloat3(&mSOffButt->mMeshBox.Center, 0.5f*(vMin + vMax));
		XMStoreFloat3(&mSOffButt->mMeshBox.Extents, 0.5f*(vMax - vMin));
	

//MUSICONBUTT*******************************************************************
	vMin = XMLoadFloat3(&vMinf3);
	vMax = XMLoadFloat3(&vMaxf3);

	for (size_t i = 0; i < mMOnButt->mGrid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = mMOnButt->mGrid.Vertices[i].Position;
		vertices[k].Normal = mMOnButt->mGrid.Vertices[i].Normal;
		vertices[k].Tex = mMOnButt->mGrid.Vertices[i].TexC;

		//Copy Into The Buttons Messh For Future Collision Check 
		mMOnButt->mMeshVertices[i].Pos = mMOnButt->mGrid.Vertices[i].Position;
		mMOnButt->mMeshVertices[i].Normal = mMOnButt->mGrid.Vertices[i].Normal;

		XMVECTOR P = XMLoadFloat3(&mMOnButt->mMeshVertices[i].Pos);

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}
	XMStoreFloat3(&mMOnButt->mMeshBox.Center, 0.5f*(vMin + vMax));
	XMStoreFloat3(&mMOnButt->mMeshBox.Extents, 0.5f*(vMax - vMin));

//MUSICOFFBUTT*******************************************************************
	vMin = XMLoadFloat3(&vMinf3);
	vMax = XMLoadFloat3(&vMaxf3);

	for (size_t i = 0; i < mMOffButt->mGrid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = mMOffButt->mGrid.Vertices[i].Position;
		vertices[k].Normal = mMOffButt->mGrid.Vertices[i].Normal;
		vertices[k].Tex = mMOffButt->mGrid.Vertices[i].TexC;

		//Copy Into The Buttons Messh For Future Collision Check 
		mMOffButt->mMeshVertices[i].Pos = mMOffButt->mGrid.Vertices[i].Position;
		mMOffButt->mMeshVertices[i].Normal = mMOffButt->mGrid.Vertices[i].Normal;

		XMVECTOR P = XMLoadFloat3(&mMOffButt->mMeshVertices[i].Pos);

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}
	XMStoreFloat3(&mMOffButt->mMeshBox.Center, 0.5f*(vMin + vMax));
	XMStoreFloat3(&mMOffButt->mMeshBox.Extents, 0.5f*(vMax - vMin));

//mTitleButt*******************************************************************
	vMin = XMLoadFloat3(&vMinf3);
	vMax = XMLoadFloat3(&vMaxf3);

	for (size_t i = 0; i < mTitleButt->mGrid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = mTitleButt->mGrid.Vertices[i].Position;
		vertices[k].Normal = mTitleButt->mGrid.Vertices[i].Normal;
		vertices[k].Tex = mTitleButt->mGrid.Vertices[i].TexC;

		//Copy Into The Buttons Messh For Future Collision Check 
		mTitleButt->mMeshVertices[i].Pos = mTitleButt->mGrid.Vertices[i].Position;
		mTitleButt->mMeshVertices[i].Normal = mTitleButt->mGrid.Vertices[i].Normal;

		XMVECTOR P = XMLoadFloat3(&mTitleButt->mMeshVertices[i].Pos);

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}
	XMStoreFloat3(&mTitleButt->mMeshBox.Center, 0.5f*(vMin + vMax));
	XMStoreFloat3(&mTitleButt->mMeshBox.Extents, 0.5f*(vMax - vMin));


//mAboutButt*******************************************************************
	vMin = XMLoadFloat3(&vMinf3);
	vMax = XMLoadFloat3(&vMaxf3);

	for (size_t i = 0; i < mAboutButt->mGrid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = mAboutButt->mGrid.Vertices[i].Position;
		vertices[k].Normal = mAboutButt->mGrid.Vertices[i].Normal;
		vertices[k].Tex = mAboutButt->mGrid.Vertices[i].TexC;

		//Copy Into The Buttons Messh For Future Collision Check 
		mAboutButt->mMeshVertices[i].Pos = mAboutButt->mGrid.Vertices[i].Position;
		mAboutButt->mMeshVertices[i].Normal = mAboutButt->mGrid.Vertices[i].Normal;

		XMVECTOR P = XMLoadFloat3(&mAboutButt->mMeshVertices[i].Pos);

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}
	XMStoreFloat3(&mAboutButt->mMeshBox.Center, 0.5f*(vMin + vMax));
	XMStoreFloat3(&mAboutButt->mMeshBox.Extents, 0.5f*(vMax - vMin));





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
//PLAY BUTTON****************************************************************************
		world = XMLoadFloat4x4(&mPlayButt->mWorld);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;
		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
		Effects::BasicFX->SetMaterial(mPlayButt->mMat);
		Effects::BasicFX->SetDiffuseMap(mPlayButt->mTexSRV);

		activeTexTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mPlayButt->mIndexCount, mPlayButt->GetIndOffset(), mPlayButt->GetVertOffset());

//SOUND BUTTON****************************************************************************
		world = XMLoadFloat4x4(&mSoundButt->mWorld);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;
		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
		Effects::BasicFX->SetMaterial(mSoundButt->mMat);
		Effects::BasicFX->SetDiffuseMap(mSoundButt->mTexSRV);

		activeTexTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mSoundButt->mIndexCount, mSoundButt->GetIndOffset(), mSoundButt->GetVertOffset());

//mMusicButt****************************************************************************
		world = XMLoadFloat4x4(&mMusicButt->mWorld);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;
		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
		Effects::BasicFX->SetMaterial(mMusicButt->mMat);
		Effects::BasicFX->SetDiffuseMap(mMusicButt->mTexSRV);

		activeTexTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mMusicButt->mIndexCount, mMusicButt->GetIndOffset(), mMusicButt->GetVertOffset());

//mSOnButt****************************************************************************
		if(*StateMachine::pSoundState == SoundState::SOUNDON)
		{
			world = XMLoadFloat4x4(&mSOnButt->mWorld);
			worldInvTranspose = MathHelper::InverseTranspose(world);
			worldViewProj = world*view*proj;
			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
			Effects::BasicFX->SetMaterial(mSOnButt->mMat);
			Effects::BasicFX->SetDiffuseMap(mSOnButt->mTexSRV);
	
			activeTexTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(mSOnButt->mIndexCount, mSOnButt->GetIndOffset(), mSOnButt->GetVertOffset());
		}

//mSOffButt****************************************************************************
		if (*StateMachine::pSoundState == SoundState::SOUNDOFF)
		{
			world = XMLoadFloat4x4(&mSOffButt->mWorld);
			worldInvTranspose = MathHelper::InverseTranspose(world);
			worldViewProj = world*view*proj;
			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
			Effects::BasicFX->SetMaterial(mSOffButt->mMat);
			Effects::BasicFX->SetDiffuseMap(mSOffButt->mTexSRV);
	
			activeTexTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(mSOffButt->mIndexCount, mSOffButt->GetIndOffset(), mSOffButt->GetVertOffset());
		}

//mMOnButt****************************************************************************
		if (*StateMachine::pMusicState == MusicState::MUSICON)
		{
			world = XMLoadFloat4x4(&mMOnButt->mWorld);
			worldInvTranspose = MathHelper::InverseTranspose(world);
			worldViewProj = world*view*proj;
			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
			Effects::BasicFX->SetMaterial(mMOnButt->mMat);
			Effects::BasicFX->SetDiffuseMap(mMOnButt->mTexSRV);
	
			activeTexTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(mMOnButt->mIndexCount, mMOnButt->GetIndOffset(), mMOnButt->GetVertOffset());
		}

//mSOffButt****************************************************************************
		if (*StateMachine::pMusicState == MusicState::MUSICOFF)
		{
			world = XMLoadFloat4x4(&mSOffButt->mWorld);
			worldInvTranspose = MathHelper::InverseTranspose(world);
			worldViewProj = world*view*proj;
			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
			Effects::BasicFX->SetMaterial(mSOffButt->mMat);
			Effects::BasicFX->SetDiffuseMap(mSOffButt->mTexSRV);
	
			activeTexTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(mSOffButt->mIndexCount, mSOffButt->GetIndOffset(), mSOffButt->GetVertOffset());
		}

//mTitleButt****************************************************************************
		world = XMLoadFloat4x4(&mTitleButt->mWorld);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;
		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
		Effects::BasicFX->SetMaterial(mTitleButt->mMat);
		Effects::BasicFX->SetDiffuseMap(mTitleButt->mTexSRV);

		activeTexTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mTitleButt->mIndexCount, mTitleButt->GetIndOffset(), mTitleButt->GetVertOffset());

//mAboutButt****************************************************************************
		world = XMLoadFloat4x4(&mAboutButt->mWorld);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;
		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
		Effects::BasicFX->SetMaterial(mAboutButt->mMat);
		Effects::BasicFX->SetDiffuseMap(mAboutButt->mTexSRV);

		activeTexTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mAboutButt->mIndexCount, mAboutButt->GetIndOffset(), mAboutButt->GetVertOffset());

		// 		mSoundButt
		// 		mMusicButt
		// 		mSOnButt
		// 		mSOffButt
		// 		mMOnButt
		// 		mMOffButt
		// 		mTitleButt
		// 		mAboutButt
	}

	if(!fullyLoaded)fullyLoaded = true;
}
void Engine::DrawPaused()
{

}
void Engine::DrawGameOn()
{
	md3dImmediateContext->RSSetState(0);
	mSky->Draw(md3dImmediateContext, mCam);
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
		//BtnsMainMenu(x, y, true);
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
		case GameState::PAUSED:		BtnsPaused(x, y, false);		break;
		case GameState::WIN:		BtnsWin(x, y, false);			break;
		case GameState::LOSE:		BtnsLose(x, y, false);			break;
		case GameState::GAMEON:		BtnsGameOn(x, y, false);		break;
		}
	}
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		mCam.mPitch += dy; //Increment internal pitch 

		if (mCam.mPitch > -XM_PI/2 && mCam.mPitch < XM_PI/4){ mCam.Pitch(dy); } //LIMITS UP AND DOWN
		mCam.mPitch = MathHelper::Clamp(mCam.mPitch, -XM_PI / 2, XM_PI/4);
		
		mCam.RotateY(dx);
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
	}
}
void Engine::KeyboardHandler(float dt)
{

	//
	// Control the camera.
	//
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
			//Button Clicked
		}
	}
	else{ mPlayButt->hovering = false; }

	if (InButton(x, y, mAboutButt))
	{
		mAboutButt->hovering = true;
		if (clicked)
		{
			//Button Clicked
		}
	}
	else{ mAboutButt->hovering = false; }

	if (InButton(x, y, mSoundButt))
	{
		mSoundButt->hovering = true;
		if (clicked)
		{
			//Button Clicked
		}
	}
	else{ mSoundButt->hovering = false; }

	if (InButton(x, y, mMusicButt))
	{
		mMusicButt->hovering = true;
		if (clicked)
		{
			//Button Clicked
		}
	}
	else{ mMusicButt->hovering = false; }
}
void Engine::BtnsPaused(float x, float y, bool clicked)
{

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

