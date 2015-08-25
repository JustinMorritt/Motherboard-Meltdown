#ifndef Button_h__
#define Button_h__


#include "Sky.h"
#include "GeometryGenerator.h"
#include "Camera.h"
#include "Vertex.h"
#include "Effects.h"
#include "xnacollision.h"

class Button
{
public:
	Button(ID3D11Device* device,std::wstring texFilename, float width, float height);
	~Button();
	void SetPos(float x, float y, float z);
	void SetRot(float x, float y, float z);
	void Update(const Camera& camera, float dt);
	void SetVertexOffset(int offSet);
	void SetIndexOffset(int offSet);

	// Strafe/Walk
	void Strafe(float d);
	void Roll(float d);
	void Walk(float d);

	// Rotate 
	void Pitch(float angle);
	void RotateY(float angle);

	//Scale
	void Scale(float scale);

	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

	//Getters
	int GetVertOffset();
	int GetIndOffset();

	XMVECTOR GetRightXM()const;
	XMFLOAT3 GetRight()const;
	XMVECTOR GetUpXM()const;
	XMFLOAT3 GetUp()const;
	XMVECTOR GetLookXM()const;
	XMFLOAT3 GetLook()const;
	XMVECTOR GetPositionXM()const;


	GeometryGenerator::MeshData GetMeshData();


	ID3D11ShaderResourceView*	mTexSRV;
	Material					mMat;
	XMFLOAT4X4					mWorld;
	int							mVertexOffset;
	UINT						mIndexOffset;
	UINT						mIndexCount;
	GeometryGenerator::MeshData mGrid;



	XMFLOAT3 mPosition;
	XMFLOAT3 mRight;
	XMFLOAT3 mUp;
	XMFLOAT3 mLook;

	int mWidth;
	int mHeight;

	float rotationY;
	float prevPitch;
	float prevRoll;

	bool hovering;

	//SYSTEM COPIES OF MESH FOR PICKING
	std::vector<Vertex::Basic32> mMeshVertices;
	std::vector<UINT> mMeshIndices;

	XNA::AxisAlignedBox mMeshBox;

};

#endif // Button_h__
