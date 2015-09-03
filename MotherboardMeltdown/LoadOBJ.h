#ifndef LOADOBJ_H
#define LOADOBJ_H

#include "MeshGeometry.h"
#include "LightHelper.h"
#include "Vertex.h"


struct ObjMaterial
{
	std::string name;
	Material Mat;
	bool AlphaClip;
	std::string EffectTypeName;
	std::wstring DiffuseMapName;
	std::wstring NormalMapName;
};

class OBJLoader
{
	struct HashEntry
	{
		HashEntry* pNext;
		int index;
	};

public:
	bool LoadOBJ(ID3D11Device* device,
						const std::string& filename, 
						std::vector<Vertex::Basic32>& vertices,
						std::vector<USHORT>& indices,
						std::vector<MeshGeometry::Subset>& subsets,
						std::vector<ObjMaterial>& mats,
						bool vFlipped = false, bool RHSModel = false);

private:
	unsigned int AddVertex( Vertex::Basic32 vertex, 
							std::vector<Vertex::Basic32>& vertexArray, 
							HashEntry** hashTable, unsigned int& currentIndex, 
							unsigned int hashVal);

	void ReadMaterials(std::ifstream& fin, std::vector<ObjMaterial>& mats);

	UINT GetSubset(const std::wstring& name, const std::vector<ObjMaterial>& mats);
};

#endif