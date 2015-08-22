#include "Engine.h"




Engine::Engine(HINSTANCE hInstance)
	: D3DApp(hInstance), mSky(0), mRandomTexSRV(0), mFlareTexSRV(0), mRainTexSRV(0), mWalkCamMode(false), mWireMode(false), mBFCull(false)
{
	mMainWndCaption = L"Motherboard Meltdown";
	mEnable4xMsaa = false;

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	mCam.SetPosition(0.0f, 2.0f, 100.0f);

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
	md3dImmediateContext->ClearState();
	
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

	mSky = new Sky(md3dDevice, L"Textures/grasscube1024.dds", 5000.0f);

	Terrain::InitInfo tii;
	tii.HeightMapFilename = L"Textures/terrain.raw";
	tii.LayerMapFilename0 = L"Textures/grass.dds";
	tii.LayerMapFilename1 = L"Textures/darkdirt.dds";
	tii.LayerMapFilename2 = L"Textures/stone.dds";
	tii.LayerMapFilename3 = L"Textures/lightdirt.dds";
	tii.LayerMapFilename4 = L"Textures/snow.dds";
	tii.BlendMapFilename = L"Textures/blend.dds";
	tii.HeightScale = 50.0f;
	tii.HeightmapWidth = 2049;
	tii.HeightmapHeight = 2049;
	tii.CellSpacing = 0.5f;

	mTerrain.Init(md3dDevice, md3dImmediateContext, tii);

	mRandomTexSRV = d3dHelper::CreateRandomTexture1DSRV(md3dDevice);

	std::vector<std::wstring> flares;
	flares.push_back(L"Textures\\flare0.dds");
	mFlareTexSRV = d3dHelper::CreateTexture2DArraySRV(md3dDevice, md3dImmediateContext, flares);

	mFire.Init(md3dDevice, Effects::FireFX, mFlareTexSRV, mRandomTexSRV, 500); 
	mFire.SetEmitPos(XMFLOAT3(0.0f, 1.0f, 120.0f));

	std::vector<std::wstring> raindrops;
	raindrops.push_back(L"Textures\\raindrop.dds");
	mRainTexSRV = d3dHelper::CreateTexture2DArraySRV(md3dDevice, md3dImmediateContext, raindrops);

	mRain.Init(md3dDevice, Effects::RainFX, mRainTexSRV, mRandomTexSRV, 10000); 


	*StateMachine::pGameState = GameState::MAINMENU;

	return true;
}

void Engine::OnResize()
{
	D3DApp::OnResize();

	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 3000.0f);
}

void Engine::UpdateScene(float dt)
{
	//
	// Control the camera.
	//
	if( GetAsyncKeyState('W') & 0x8000 )
		mCam.Walk(10.0f*dt);

	if( GetAsyncKeyState('S') & 0x8000 )
		mCam.Walk(-10.0f*dt);

	if( GetAsyncKeyState('A') & 0x8000 )
		mCam.Strafe(-10.0f*dt);

	if( GetAsyncKeyState('D') & 0x8000 )
		mCam.Strafe(10.0f*dt);

	//
	// Walk/fly mode
	//
	if( GetAsyncKeyState('2') & 0x8000 )
		mWalkCamMode = true;
	if( GetAsyncKeyState('3') & 0x8000 )
		mWalkCamMode = false;

	// 
	// Clamp camera to terrain surface in walk mode.
	//
	if( mWalkCamMode )
	{
		XMFLOAT3 camPos = mCam.GetPosition();
		float y = mTerrain.GetHeight(camPos.x, camPos.z);
		mCam.SetPosition(camPos.x, y + 2.0f, camPos.z);
	}

	//
	// Reset particle systems.
	//
	if(GetAsyncKeyState('R') & 0x8000)
	{
		mFire.Reset();
		mRain.Reset();
	}
 
	mFire.Update(dt, mTimer.TotalTime());
	mRain.Update(dt, mTimer.TotalTime());

	mCam.UpdateViewMatrix();
}

void Engine::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Silver));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	md3dImmediateContext->IASetInputLayout(InputLayouts::Basic32);
    md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
 
	float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};

	if( mWireMode )	md3dImmediateContext->RSSetState(RenderStates::WireframeRS);





	mTerrain.Draw(md3dImmediateContext, mCam, mDirLights);

	md3dImmediateContext->RSSetState(0);

	mSky->Draw(md3dImmediateContext, mCam);


	// Draw particle systems last so it is blended with scene.
// 	mFire.SetEyePos(mCam.GetPosition());
// 	mFire.Draw(md3dImmediateContext, mCam);
// 	md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff); // restore default

// 	mRain.SetEyePos(mCam.GetPosition());
// 	mRain.SetEmitPos(mCam.GetPosition());
// 	mRain.Draw(md3dImmediateContext, mCam);



	// restore default states.
	md3dImmediateContext->RSSetState(0);
	md3dImmediateContext->OMSetDepthStencilState(0, 0);
	md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff); 

	HR(mSwapChain->Present(0, 0));
}

void Engine::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void Engine::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void Engine::OnMouseMove(WPARAM btnState, int x, int y)
{
	if( (btnState & MK_LBUTTON) != 0 )
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		mCam.Pitch(dy);
		mCam.RotateY(dx);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void Engine::OnKeyUP(WPARAM btnState)
{
	switch (btnState)
	{
	case 0x31:(mWireMode)	?	mWireMode	= false : mWireMode		= true; break;	// 1 KEY
	case 0x32:(mBFCull)		?	mBFCull		= false : mBFCull		= true; break;	// 2 KEY
	}
}