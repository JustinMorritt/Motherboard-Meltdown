#include "Engine.h"




Engine::Engine(HINSTANCE hInstance)
	: D3DApp(hInstance), mSky(0), mRandomTexSRV(0), mFlareTexSRV(0), mRainTexSRV(0), mFloorTexSRV(0), mWalkCamMode(false), mWireMode(false), mBFCull(false)
{
	mMainWndCaption = L"Motherboard Meltdown";
	mEnable4xMsaa = false;

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	mCam.SetPosition(0.0f, 200.0f, -350.0f);
	mCam.Pitch(XM_PI / 4.5);

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



	mSky = new Sky(md3dDevice, L"Textures/grasscube1024.dds", 5000.0f);


//FIRE EMITTER
//mRandomTexSRV = d3dHelper::CreateRandomTexture1DSRV(md3dDevice);
// 	std::vector<std::wstring> flares;
// 	flares.push_back(L"Textures\\flare0.dds");
// 	mFlareTexSRV = d3dHelper::CreateTexture2DArraySRV(md3dDevice, md3dImmediateContext, flares);
// 	mFire.Init(md3dDevice, Effects::FireFX, mFlareTexSRV, mRandomTexSRV, 500); 
// 	mFire.SetEmitPos(XMFLOAT3(0.0f, 1.0f, 120.0f));

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
	KeyboardHandler(dt);
	CameraHandler();


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

	// Draw particle systems last so it is blended with scene.
// 	mFire.SetEyePos(mCam.GetPosition());
// 	mFire.Draw(md3dImmediateContext, mCam);
// 	md3dImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff); // restore default
	RestoreStates();
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

	geoGen.CreateGrid(500.0f, 500.0f, 60, 40, grid);

	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	mGridVertexOffset = 0; // FIRST OBJECT FOR OFFSET

	// Cache the index count of each object.
	mGridIndexCount = grid.Indices.size();

	mGridIndexOffset = 0;

	UINT totalVertexCount =	grid.Vertices.size();

	UINT totalIndexCount =	mGridIndexCount;

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
	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light1TexTech;

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
	}
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