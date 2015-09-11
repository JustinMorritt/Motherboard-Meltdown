//***************************************************************************************
// Camera.h by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "Camera.h"

Camera::Camera()
	: mPosition(0.0f, 0.0f, 0.0f),
	mRight(1.0f, 0.0f, 0.0f),
	mUp(0.0f, 1.0f, 0.0f),
	mLook(0.0f, 0.0f, 1.0f),
	mPitch(0.0f),
	mUseConstraints(false),
	x1(0.0f), x2(0.0f), y1(0.0f), y2(0.0f), z1(0.0f), z2(0.0f)
{
	SetLens(0.25f*MathHelper::Pi, 1.0f, 1.0f, 1000.0f);
	mSphereCollider.Center = mPosition;
	mSphereCollider.Radius = 20.0f;
}

Camera::~Camera()
{
}

XMVECTOR Camera::GetPositionXM()const
{
	return XMLoadFloat3(&mPosition);
}

XMFLOAT3 Camera::GetPosition()const
{
	return mPosition;
}

void Camera::SetPosition(float x, float y, float z)
{
	mPosition = XMFLOAT3(x, y, z);
}

void Camera::SetPosition(const XMFLOAT3& v)
{
	mPosition = v;
}

void Camera::ResetCam()
{
	mPosition = { 0.0f, 0.0f, 0.0f };
	mRight = { 1.0f, 0.0f, 0.0f };
	mUp = { 0.0f, 1.0f, 0.0f };
	mLook = { 0.0f, 0.0f, 1.0f };
	mPitch =0.0f;
}

XMVECTOR Camera::GetRightXM()const
{
	return XMLoadFloat3(&mRight);
}

XMFLOAT3 Camera::GetRight()const
{
	return mRight;
}

XMVECTOR Camera::GetUpXM()const
{
	return XMLoadFloat3(&mUp);
}

XMFLOAT3 Camera::GetUp()const
{
	return mUp;
}

XMVECTOR Camera::GetLookXM()const
{
	return XMLoadFloat3(&mLook);
}

XMFLOAT3 Camera::GetLook()const
{
	return mLook;
}

float Camera::GetNearZ()const
{
	return mNearZ;
}

float Camera::GetFarZ()const
{
	return mFarZ;
}

float Camera::GetAspect()const
{
	return mAspect;
}

float Camera::GetFovY()const
{
	return mFovY;
}

float Camera::GetFovX()const
{
	float halfWidth = 0.5f*GetNearWindowWidth();
	return 2.0f*atan(halfWidth / mNearZ);
}

float Camera::GetNearWindowWidth()const
{
	return mAspect * mNearWindowHeight;
}

float Camera::GetNearWindowHeight()const
{
	return mNearWindowHeight;
}

float Camera::GetFarWindowWidth()const
{
	return mAspect * mFarWindowHeight;
}

float Camera::GetFarWindowHeight()const
{
	return mFarWindowHeight;
}

void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf( 0.5f*mFovY );
	mFarWindowHeight  = 2.0f * mFarZ * tanf( 0.5f*mFovY );

	XMMATRIX P = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	XMStoreFloat4x4(&mProj, P);
}

void Camera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mLook, L);
	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
}

void Camera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);
}

XMMATRIX Camera::View()const
{
	return XMLoadFloat4x4(&mView);
}

XMMATRIX Camera::Proj()const
{
	return XMLoadFloat4x4(&mProj);
}

XMMATRIX Camera::ViewProj()const
{
	return XMMatrixMultiply(View(), Proj());
}

void Camera::Strafe(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMFLOAT3 test;
	XMStoreFloat3(&test, XMVectorMultiplyAdd(s, r, p));
	if (mUseConstraints){if(BoundsCheck(test)){mPosition = test;}}
	else{mPosition = test;}
}

void Camera::Walk(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&mLook);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMFLOAT3 test;
	XMStoreFloat3(&test, XMVectorMultiplyAdd(s, l, p));
	if (mUseConstraints){ if (BoundsCheck(test)){ mPosition = test; } }
	else{ mPosition = test; }
}

void Camera::SetConstraints(float X1, float X2, float Y1, float Y2, float Z1, float Z2)
{
	x1 = X1; x2 = X2; y1 = Y1; y2 = Y2; z1 = Z1; z2 = Z2;
}

bool Camera::BoundsCheck(XMFLOAT3 pos)
{
	return (pos.x > x1 && pos.x < x2 &&
			pos.y > y1 && pos.y < y2 &&
			pos.z > z1 && pos.z < z2 );
}

void Camera::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.


	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);

	XMStoreFloat3(&mUp,   XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));


}

void Camera::Roll(float angle)
{
	//Rotate Up and Right on Look Vector

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mLook), angle);

	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
}

void Camera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&mRight,		XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp,			XMVector3TransformNormal(XMLoadFloat3(&mUp),	R));
	XMStoreFloat3(&mLook,		XMVector3TransformNormal(XMLoadFloat3(&mLook),	R));
}

void Camera::UpdateViewMatrix()
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
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
	XMStoreFloat3(&mLook, L);

	mView(0,0) = mRight.x; 
	mView(1,0) = mRight.y; 
	mView(2,0) = mRight.z; 
	mView(3,0) = x;   

	mView(0,1) = mUp.x;
	mView(1,1) = mUp.y;
	mView(2,1) = mUp.z;
	mView(3,1) = y;  

	mView(0,2) = mLook.x; 
	mView(1,2) = mLook.y; 
	mView(2,2) = mLook.z; 
	mView(3,2) = z;   

	mView(0,3) = 0.0f;
	mView(1,3) = 0.0f;
	mView(2,3) = 0.0f;
	mView(3,3) = 1.0f;

	mSphereCollider.Center = mPosition;
}


