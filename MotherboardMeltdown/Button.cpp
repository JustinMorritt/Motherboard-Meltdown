#include "Button.h"

//Makes a Square by default 
Button::Button(ID3D11Device* device, float width, float height, bool sphere, bool upRightSquare) :
mPosition(0.0f, 0.0f, 0.0f),
mRight(1.0f, 0.0f, 0.0f),
mUp(0.0f, 1.0f, 0.0f),
mLook(0.0f, 0.0f, 1.0f),
prevPitch(0.0f),
rotationY(0.0f),
prevRoll(0.0f),
origTexScale(1.0f),
texTrans(0.0f, 0.0f, 0.0f),
texTransMult(0.0f, 0.0f, 0.0f),
mGoToPos(0.0f, 0.0f, 0.0f),
currProgress(0.0f),
rotationZ(0.0f),
mDistanceLeft(0.0f),
mWidth(width),
mHeight(height),
hovering(false),
useTexTrans(false),
progressBar(false),
goToPos(false),
billboard(false),
flipUpright(false),
reverseLook(false),
mDead(false),
turnAngle(0.0f)
{
	//CREATE TEXTURE


	//SET MATERIAL
	mMat.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mMat.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 48.0f);


	GeometryGenerator geoGen;

	//FLOOR PLANE
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mWorld, I);
	if (sphere)
	{		
		geoGen.CreateSphere(width, height, height, mGrid);  //height is slice count .. width for radius


	}
	if (upRightSquare)
	{
	
		geoGen.CreateUprightSquare(width, height, mGrid); 



	}
	if (!sphere && !upRightSquare)
	{
		geoGen.CreateGrid(width, height, 2 ,2, mGrid);
	}
	

	mIndexCount = mGrid.Indices.size();
	mMeshVertices.resize(mGrid.Vertices.size());

}

Button::~Button()
{
	//Commented so you have to intentionally release these .. some are sharing textures
// 	mTexSRV->Release();
 	mTexSRV = nullptr;
}

void Button::SetPos(float x, float y, float z)
{
	mPosition = XMFLOAT3(x, y, z);
}

void Button::SetRot(float x, float y, float z)
{
	XMMATRIX rotX = XMMatrixRotationX(XM_PI*x);
	XMMATRIX rotY = XMMatrixRotationY(XM_PI*y);
	XMMATRIX rotZ = XMMatrixRotationZ(XM_PI*z);
	XMStoreFloat4x4(&mWorld, rotX * rotY * rotZ);
}


void Button::Update(const Camera& camera, float dt)
{
	
	XMVECTOR R = XMLoadFloat3(&mRight);
	XMVECTOR U = XMLoadFloat3(&mUp);
	XMVECTOR L = XMLoadFloat3(&mLook);
	XMVECTOR P = XMLoadFloat3(&mPosition);

	
	// Keep axes orthogonal to each other and of unit length.
	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = XMVector3Cross(U, L);

	
	// Fill in the world matrix entries.
// 	float x = XMVectorGetX(XMVector3Dot(P, R));
// 	float y = XMVectorGetX(XMVector3Dot(P, U));
// 	float z = XMVectorGetX(XMVector3Dot(P, L));

	float x = XMVectorGetX(P);
	float y = XMVectorGetY(P);
	float z = XMVectorGetZ(P);

	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
	XMStoreFloat3(&mLook, L);

	mWorld(0, 0) = mRight.x;
	mWorld(1, 0) = mRight.y;
	mWorld(2, 0) = mRight.z;
	mWorld(3, 0) = x;

	mWorld(0, 1) = mUp.x;
	mWorld(1, 1) = mUp.y;
	mWorld(2, 1) = mUp.z;
	mWorld(3, 1) = y;

	if (reverseLook){
	mWorld(0, 2) = -mLook.x;
	mWorld(1, 2) = -mLook.y;
	mWorld(2, 2) = -mLook.z;
	mWorld(3, 2) = z;}
	else
	{	mWorld(0, 2) = mLook.x;
		mWorld(1, 2) = mLook.y;
		mWorld(2, 2) = mLook.z;
		mWorld(3, 2) = z;}


	mWorld(0, 3) = 0.0f;
	mWorld(1, 3) = 0.0f;
	mWorld(2, 3) = 0.0f;
	mWorld(3, 3) = 1.0f;

	if (hovering)
	{
		XMMATRIX M = XMLoadFloat4x4(&mWorld);
		XMMATRIX scaling = XMMatrixScaling(1.3f, 1.3f, 1.3f);
		XMStoreFloat4x4(&mWorld, scaling * M);
	}

	if (progressBar)
	{
		ScaleX(currProgress);
	}

	if (billboard)
	{
		XMMATRIX M		= XMLoadFloat4x4(&mWorld);
		XMVECTOR L		= XMVector3Normalize(XMVectorSubtract(camera.GetPositionXM(), GetPositionXM()));
		XMVECTOR Look	= XMLoadFloat3(&mLook);
		XMVECTOR angle	= XMVector3AngleBetweenNormals(L, Look);
		float theAngle	= XMVectorGetY(angle);

		XMMATRIX rotY;
		camera.GetPosition().x < mPosition.x ? rotY = XMMatrixRotationY(-theAngle) : rotY = XMMatrixRotationY(theAngle); 

		XMStoreFloat4x4(&mWorld, rotY * M); 
	}

	if (goToPos)
	{
		if (mDistanceLeft <= 0){ goToPos = false; }
		else
		{
			
		}
	}


}

void Button::Draw(ID3DX11EffectTechnique* activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, float dt)
{
	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
	XMMATRIX worldViewProj = world*camera.View()*camera.Proj();
	Effects::BasicFX->SetWorld(world);
	Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
	Effects::BasicFX->SetWorldViewProj(worldViewProj);
	if (!useTexTrans){ Effects::BasicFX->SetTexTransform(XMMatrixScaling(origTexScale, origTexScale, origTexScale)); }
	if (useTexTrans)
	{
		Effects::BasicFX->SetTexTransform(XMMatrixTranslation(texTrans.x, texTrans.y, texTrans.z)*XMMatrixScaling(origTexScale, origTexScale, origTexScale));
		texTrans.x += dt*texTransMult.x;
		texTrans.y += dt*texTransMult.y;
		texTrans.z += dt*texTransMult.z;
	}
	Effects::BasicFX->SetMaterial(mMat);
	Effects::BasicFX->SetDiffuseMap(mTexSRV);

	activeTech->GetPassByIndex(pass)->Apply(0, context);
	context->DrawIndexed(mIndexCount, mIndexOffset, mVertexOffset);
}

void Button::Draw2D(ID3DX11EffectTechnique* activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, XMMATRIX& ortho)
{
	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);

	XMMATRIX worldViewProj = world*ortho;
	Effects::BasicFX->SetWorld(world);
	Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
	Effects::BasicFX->SetWorldViewProj(worldViewProj);
	Effects::BasicFX->SetTexTransform(XMMatrixScaling(origTexScale, origTexScale, origTexScale));
	Effects::BasicFX->SetMaterial(mMat);
	Effects::BasicFX->SetDiffuseMap(mTexSRV);

	activeTech->GetPassByIndex(pass)->Apply(0, context);
	context->DrawIndexed(mIndexCount, mIndexOffset, mVertexOffset);
}

void Button::DrawShadow(ID3DX11EffectTechnique* activeTech, ID3D11DeviceContext* context, const XMVECTOR& shadPlane, const XMVECTOR& lightDir, const XMMATRIX& S, float scale, float xOff, float yOff, float zOff, const Camera& camera, const Material& mat)
{
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		ID3DX11EffectPass* pass = activeTech->GetPassByIndex(p);
		XMMATRIX shadowScale = XMMatrixScaling(scale, scale, scale);

		XMMATRIX trans = XMMatrixTranslation(mWorld.m[3][0] + xOff, yOff, mWorld.m[3][2] + zOff); // ORIGINAL TRANSLATION
		XMMATRIX world = XMLoadFloat4x4(&mWorld)*S*shadowScale*trans;
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*camera.View()*camera.Proj();
		Effects::BasicFX->SetTexTransform(XMMatrixScaling(origTexScale, origTexScale, origTexScale));
		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetMaterial(mat);
		Effects::BasicFX->SetDiffuseMap(mTexSRV);

		pass->Apply(0, context);
		context->DrawIndexed(mIndexCount, mIndexOffset, 0);
	}
}




void Button::SetVertexOffset(int offSet)
{
	mVertexOffset = offSet;
}

void Button::SetIndexOffset(int offSet)
{
	mIndexOffset = offSet;
}

void Button::LoadVertData(std::vector<Vertex::Basic32>& verts, UINT& k)
{
	XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	XMVECTOR vMin = XMLoadFloat3(&vMinf3);
	XMVECTOR vMax = XMLoadFloat3(&vMaxf3);

	for (size_t i = 0; i < mGrid.Vertices.size(); ++i, ++k)
	{
		verts[k].Pos	= mGrid.Vertices[i].Position;
		verts[k].Normal = mGrid.Vertices[i].Normal;
		verts[k].Tex	= mGrid.Vertices[i].TexC;

		//Copy Into The Buttons Messh For Future Collision Check 
		mMeshVertices[i].Pos	= mGrid.Vertices[i].Position;
		mMeshVertices[i].Normal = mGrid.Vertices[i].Normal;

		XMVECTOR P = XMLoadFloat3(&mMeshVertices[i].Pos);

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}
	XMStoreFloat3(&mMeshBox.Center, 0.5f*(vMin + vMax));
	XMStoreFloat3(&mMeshBox.Extents, 0.5f*(vMax - vMin));
}




void Button::LoadTexture(ID3D11Device* device , std::wstring texFilename)
{
	HR(D3DX11CreateShaderResourceViewFromFile(device, texFilename.c_str(), 0, 0, &mTexSRV, 0));
}

void Button::UseTexture(ID3D11ShaderResourceView* tex)
{
	mTexSRV = tex;
}

int Button::GetVertOffset()
{
	return mVertexOffset;
}

int Button::GetIndOffset()
{
	return mIndexOffset;
}

GeometryGenerator::MeshData Button::GetMeshData()
{
	return mGrid;
}

XMVECTOR Button::GetRightXM()const
{
	return XMLoadFloat3(&mRight);
}

XMFLOAT3 Button::GetRight()const
{
	return mRight;
}

XMVECTOR Button::GetUpXM()const
{
	return XMLoadFloat3(&mUp);
}

XMFLOAT3 Button::GetUp()const
{
	return mUp;
}

XMVECTOR Button::GetLookXM()const
{
	return XMLoadFloat3(&mLook);
}

XMFLOAT3 Button::GetLook()const
{
	return mLook;
}

void Button::Strafe(float d)
{
	// mPosition += d*mRight
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, r, p));
}

void Button::Walk(float d)
{
	if (goToPos){ mDistanceLeft -= d; }
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&mLook);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));
}

void Button::Pitch(float angle)
{
	prevPitch = angle;
	// Rotate up and look vector about the right vector.

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);

	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}

void Button::Yaw(float angle)
{
	// Rotate right and look vector about the up vector.

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mUp), angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}

void Button::Roll(float angle)
{
	//Rotate Up and Right on Look Vector
	prevRoll = angle;

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mLook), angle);

	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
}

void Button::RotateY(float angle)
{
	rotationY = angle;
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}

void Button::RotateX(float angle)
{
	//rotationY = angle;
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationX(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}

void Button::RotateZ(float angle)
{
	XMVECTOR P = XMLoadFloat3(&mPosition);
	XMMATRIX R = XMMatrixRotationZ(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

	XMStoreFloat3(&mPosition, P);

}

void Button::Scale(float scale)
{
	//SCALING THE WAY I THOUGHT NEEDED DONE
	XMMATRIX curr = XMLoadFloat4x4(&mWorld);
	XMMATRIX trans = XMMatrixTranslation(curr.m[3][0], curr.m[3][1], curr.m[3][2]);
	XMMATRIX scaling = XMMatrixScaling(scale,scale,scale);
	XMStoreFloat4x4(&mWorld, scaling * trans); // Scaled Then sent Back To Original Position;
}

//USED FOR THE PROGRESS BARS
void Button::ScaleX(float scale)
{
	//Progress gets offset on the X depending on the currProgress
	XMMATRIX trans = XMMatrixTranslation(mWorld.m[3][0]-(mWidth/2 - (mWidth/2*currProgress)), mWorld.m[3][1], mWorld.m[3][2]); // ORIGINAL TRANSLATION
	XMMATRIX rotX = XMMatrixRotationX(-XM_PI / 4);
	XMMATRIX scaling = XMMatrixScaling(scale, 1.0f, 1.0f);
	XMStoreFloat4x4(&mWorld, scaling * rotX * trans); // Scaled Then sent Back To Original Position;
}

XMVECTOR Button::GetPositionXM()const
{
	return XMLoadFloat3(&mPosition);
}

void Button::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);


	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mLook, L);
	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
}

void Button::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);
}

void Button::SetGoToPoint(float x, float y, float z)
{
	mGoToPos.x = x; mGoToPos.y = y, mGoToPos.z = z;
	goToPos = true;
	XMVECTOR first = XMLoadFloat3(&mGoToPos);
	XMVECTOR second = XMLoadFloat3(&mPosition);
	//sqrt of Final - curr  Vectors
	XMVECTOR end = XMVectorSubtract(first, second);
	mDistanceLeft = sqrt((XMVectorGetX(end)*XMVectorGetX(end)) + (XMVectorGetY(end)*XMVectorGetY(end)) + (XMVectorGetZ(end)*XMVectorGetZ(end)));

	//Calculate Direction Vector..
	XMVECTOR L = XMVector3Normalize(end);
	XMVECTOR Look = XMLoadFloat3(&mLook);
	XMVECTOR angle = XMVector3AngleBetweenNormals(L, Look);

	float theAngle = XMVectorGetZ(angle);
	turnAngle = theAngle;
	XMVectorGetX(end) < mPosition.x ? Yaw(-theAngle) : Yaw(theAngle);
}
