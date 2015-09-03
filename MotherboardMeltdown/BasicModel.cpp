#include "BasicModel.h"
#include "LoadOBJ.h"

BasicModel::BasicModel(ID3D11Device* device, TextureMgr& texMgr,  const std::string& modelFilename,  const std::wstring& texturePath, 
	MeshGeometry::Type type)
	:
	ModelMesh(type)
{
	std::vector<ObjMaterial> mats;


	OBJLoader objLoader;



	objLoader.LoadOBJ(device, modelFilename, BasicVertices, Indices, Subsets, mats, true, true);
	ModelMesh.SetVertices(device, &BasicVertices[0], BasicVertices.size());
	ModelMesh.SetIndices(device, &Indices[0], Indices.size());
	ModelMesh.SetSubsetTable(Subsets);

	SubsetCount = mats.size();

	for(UINT i = 0; i < SubsetCount; ++i)
	{
		Material newMat;
		Mat.push_back(mats[i].Mat);

		ID3D11ShaderResourceView* diffuseMapSRV = texMgr.CreateTexture(texturePath + mats[i].DiffuseMapName);
		DiffuseMapSRV.push_back(diffuseMapSRV);

		//ID3D11ShaderResourceView* normalMapSRV = texMgr.CreateTexture(texturePath + mats[i].NormalMapName);
		//NormalMapSRV.push_back(normalMapSRV);
	}
}

BasicModel::~BasicModel()
{
}
