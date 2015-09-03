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
	Button(ID3D11Device* device,std::wstring texFilename, float width, float height, bool sphere = false, bool upRightSquare= false);
	~Button();
	void SetPos(float x, float y, float z);
	void SetRot(float x, float y, float z);
	void Update(const Camera& camera, float dt);
	void Draw(ID3DX11EffectTechnique* activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, float dt);
	void Draw2D(ID3DX11EffectTechnique* activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, XMMATRIX& ortho);
	void DrawShadow(ID3DX11EffectTechnique* activeTech, ID3D11DeviceContext* context,const XMVECTOR& shadPlane,const XMVECTOR& lightDir,const XMMATRIX& S, float scale, float xOff, float yOff, float zOff, const Camera& camera, const Material& mat);
	void SetVertexOffset(int offSet);
	void SetIndexOffset(int offSet);
	void LoadVertData(std::vector<Vertex::Basic32>& verts, UINT& k);

	// Strafe/Walk
	void Strafe(float d);
	void Roll(float d);
	void Walk(float d);

	// Rotate 
	void Pitch(float angle);
	void Yaw(float angle);
	void RotateY(float angle);
	void RotateX(float angle);
	void RotateZ(float angle);

	//Scale
	void Scale(float scale);
	void ScaleX(float scale);

	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);
	void SetGoToPoint(float x, float y, float z);


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
	XMFLOAT3 mGoToPos;
	XMFLOAT3 mRight;
	XMFLOAT3 mUp;
	XMFLOAT3 mLook;

	int mWidth;
	int mHeight;

	XMFLOAT3 texTrans;
	XMFLOAT3 texTransMult;
	

	float rotationY;
	float rotationZ;
	float prevPitch;
	float prevRoll;
	float currProgress;
	float mDistanceLeft;
	float turnAngle;

	bool hovering;
	bool clicked;
	bool useTexTrans;
	bool progressBar;
	bool goToPos;
	bool billboard;
	bool flipUpright;
	bool reverseLook;

	float origTexScale;

	//SYSTEM COPIES OF MESH FOR PICKING
	std::vector<Vertex::Basic32> mMeshVertices;
	std::vector<UINT> mMeshIndices;

	XNA::AxisAlignedBox mMeshBox;

};

#endif // Button_h__
