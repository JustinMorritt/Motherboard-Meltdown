#include "Button.h"

Button::Button(ID3D11Device* device, std::wstring texFilename, float width, float height) :
mPosition(0.0f, 0.0f, 0.0f),
mRight(1.0f, 0.0f, 0.0f),
mUp(0.0f, 1.0f, 0.0f),
mLook(0.0f, 0.0f, 1.0f),
prevPitch(0.0f),
rotationY(0.0f),
prevRoll(0.0f),
mWidth(width),
mHeight(mHeight),
hovering(false)
{
	//CREAT TEXTURE
	HR(D3DX11CreateShaderResourceViewFromFile(device, texFilename.c_str(), 0, 0, &mTexSRV, 0));


	//SET MATERIAL
	mMat.Ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mMat.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);


	GeometryGenerator geoGen;

	//FLOOR PLANE
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mWorld, I);

	geoGen.CreateGrid(width, height, 4, 4, mGrid);

	mIndexCount = mGrid.Indices.size();
	mMeshVertices.resize(16);

}

Button::~Button()
{
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

	// Keep camera's axes orthogonal to each other and of unit length.
	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = XMVector3Cross(U, L);

	// Fill in the view matrix entries.
	float x = XMVectorGetX(XMVector3Dot(P, R));
	float y = XMVectorGetX(XMVector3Dot(P, U));
	float z = XMVectorGetX(XMVector3Dot(P, L));

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

	mWorld(0, 2) = mLook.x;
	mWorld(1, 2) = mLook.y;
	mWorld(2, 2) = mLook.z;
	mWorld(3, 2) = z;

	mWorld(0, 3) = 0.0f;
	mWorld(1, 3) = 0.0f;
	mWorld(2, 3) = 0.0f;
	mWorld(3, 3) = 1.0f;

	if (hovering)
	{
		//SCALING THE WAY I THOUGHT NEEDED DONE
		XMMATRIX curr = XMLoadFloat4x4(&mWorld);
		XMMATRIX trans = XMMatrixTranslation(curr.m[3][0], curr.m[3][1], curr.m[3][2]);
		XMMATRIX scaling = XMMatrixScaling(2.0f, 2.0f, 2.0f);
		XMStoreFloat4x4(&mWorld, scaling * trans); // Scaled Then sent Back To Original Position;
		//Roll(prevRoll);
		//Pitch(prevPitch);
		//RotateY(rotationY);
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
	// mPosition += d*mLook
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

void Button::Scale(float scale)
{
	//SCALING THE WAY I THOUGHT NEEDED DONE
	XMMATRIX curr = XMLoadFloat4x4(&mWorld);
	XMMATRIX trans = XMMatrixTranslation(curr.m[3][0], curr.m[3][1], curr.m[3][2]);
	XMMATRIX scaling = XMMatrixScaling(scale,scale,scale);
	XMStoreFloat4x4(&mWorld, scaling * trans); // Scaled Then sent Back To Original Position;
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
