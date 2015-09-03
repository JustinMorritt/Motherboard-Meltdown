#include "LoadOBJ.h"
#include <fstream>

using namespace std;
 
#define MAX_VERTICES 20000000
#define MAX_INDICES 20000000


wistream& operator>>(wistream& is, XMFLOAT3& v)
{
	is >> v.x >> v.y >> v.z;
	return is;
}

wistream& operator>>(wistream& is, XMFLOAT2& v)
{
	is >> v.x >> v.y;
	return is;
}

bool operator == (const XMFLOAT3& lhs, const XMFLOAT3& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator == (const XMFLOAT2& lhs, const XMFLOAT2& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool OBJLoader::LoadOBJ(ID3D11Device* device,
						const std::string& filename, 
						std::vector<Vertex::Basic32>& vertices,
						std::vector<USHORT>& indices,
						std::vector<MeshGeometry::Subset>& subsets,
						std::vector<ObjMaterial>& mats,
						bool vFlipped, bool RHSModel)
{
	// create an index vector for each material (so we can rearrange face ordering so
	// subgroups are contiguous)
	std::vector<std::vector<USHORT>> subgroupIndices;
	std::vector<USHORT> subgroup;

	//init first subgroup in case there is no mtl file
	subgroupIndices.push_back(subgroup);

	//create buffers to hold the individual components of the vertex
	XMFLOAT3* positions = new XMFLOAT3[MAX_VERTICES];
	XMFLOAT3* normals = new XMFLOAT3[MAX_VERTICES];
	XMFLOAT2* texCoords = new XMFLOAT2[MAX_VERTICES];

	//create and NULL a hash table
	HashEntry** hashTable = new HashEntry*[MAX_VERTICES];
	for(int i = 0; i < MAX_VERTICES; i++)
	{
		hashTable[i] = 0;
	}

	wifstream fin(filename.c_str());

	if(!fin)
	{
		OutputDebugString(L"Error loading mesh \n");
		return false;
	}

	//Create indices to keep track of the individual 
	//components since we are reading them in one at a time.
	unsigned int posIndex = 0;
	unsigned int texIndex = 0;
	unsigned int normIndex = 0;
	unsigned int vertexIndex = 0;

	unsigned int activeSubgroup = 0;

	//read in the file
	wstring symbol;
	while(fin >> symbol)
	{
		//Comment
		if(symbol == L"#")
		{
		}

		//Material Library
		else if(symbol == L"mtllib")
		{
			wstring materialFile;
			fin >> materialFile;
			materialFile = L"Models\\" + materialFile;
			ifstream fin(materialFile.c_str());
			ReadMaterials(fin, mats);

			//init our index buffers
			for(int i = 1; i < mats.size(); ++i)
			{
				std::vector<USHORT> temp;
				subgroupIndices.push_back(temp);
			}
		}

		//Vertex Position
		else if(symbol == L"v")
		{
			fin >> positions[posIndex];
			posIndex++;
		}

		//Texture Coordinate
		else if(symbol == L"vt")
		{
			fin >> texCoords[texIndex];
			texIndex++;
		}

		//Normals
		else if(symbol == L"vn")
		{
			fin >> normals[normIndex];
			normIndex++;
		}

		//Faces
		else if(symbol == L"f")
		{
			for(int i = 0; i < 4; i++)
			{
				//if only 3 vertices
				if(fin.peek() == '\n')
				{
					break;
				}

				int tempPosIndex = 0;
				fin >> tempPosIndex;

				Vertex::Basic32 newVertex;
				newVertex.Pos = positions[tempPosIndex - 1];

				if(fin.peek() == '/')
				{
					fin.ignore();
					int tempTexIndex = 0;

					//skip an empty texture coord.
					if(fin.peek() != '/')
					{
						fin >> tempTexIndex;

						newVertex.Tex = texCoords[tempTexIndex - 1];
						//flip the v coord if needed
						if(vFlipped)
						{
							newVertex.Tex.y = 1 - newVertex.Tex.y;
						}
					}
					if(fin.peek() == '/')
					{
						fin.ignore();
						int tempNormalIndex = 0;
						
						fin >> tempNormalIndex;
						newVertex.Normal = normals[tempNormalIndex - 1];
					}
				}
				


				//add an entire face for the fourth vertex
				if(i == 3)
				{
					if(RHSModel)
					{
						UINT subgroupSize = subgroupIndices[activeSubgroup].size();
						subgroupIndices[activeSubgroup].push_back
							(subgroupIndices[activeSubgroup][subgroupSize - 2]);
						subgroupIndices[activeSubgroup].push_back
							(subgroupIndices[activeSubgroup][subgroupSize - 3]);
					}
					else
					{
						UINT subgroupSize = subgroupIndices[activeSubgroup].size();
						subgroupIndices[activeSubgroup].push_back
							(subgroupIndices[activeSubgroup][subgroupSize - 2]);
						subgroupIndices[activeSubgroup].push_back
							(subgroupIndices[activeSubgroup][subgroupSize - 1]);
					}
				}

				subgroupIndices[activeSubgroup].push_back
					(AddVertex(newVertex, vertices, hashTable, vertexIndex, tempPosIndex));

				//flip the first face if needed
				if(RHSModel && i == 2)
				{
					USHORT temp = subgroupIndices[activeSubgroup][subgroupIndices[activeSubgroup].size() - 2];
					subgroupIndices[activeSubgroup][subgroupIndices[activeSubgroup].size() - 2] = 
						subgroupIndices[activeSubgroup][subgroupIndices[activeSubgroup].size() - 1];
					subgroupIndices[activeSubgroup][subgroupIndices[activeSubgroup].size() - 1] = temp;
				}
			}
		}

		else if(symbol == L"usemtl")
		{
			wstring subsetName;
			fin >> subsetName;
			activeSubgroup = GetSubset(subsetName, mats);
		}

		fin.ignore(1000, '\n');
	}

	int indexCount = 0;
	//combine subgroup indices into one group
	for(int i = 0; i < subgroupIndices.size(); ++i)
	{
		indexCount += subgroupIndices[i].size();
	}

	if(indexCount > MAX_INDICES)
	{
		return false;
	}

	indices.reserve(indexCount);

	for(int i = 0; i < subgroupIndices.size(); ++i)
	{
		MeshGeometry::Subset subset;
		subset.FaceCount = subgroupIndices[i].size() / 3;
		subset.FaceStart = indices.size() / 3;
		subset.Id = i;

		indices.insert(indices.end(), subgroupIndices[i].begin(), 
			subgroupIndices[i].end());
		subsets.push_back(subset);
	}
	

	//cleanup the buffer memory
	delete [] positions;
	delete [] normals;
	delete [] texCoords;

	for(int i = 0; i < MAX_VERTICES; i++)
	{
		if(hashTable[i])
		{
			HashEntry* pCurrent = hashTable[i];
			HashEntry* pNext;
			while(pCurrent->pNext)
			{
				pNext = pCurrent->pNext;
				delete pCurrent;
				pCurrent = pNext;
			}
			delete pCurrent;
		}
	}

	delete [] hashTable;

	return true;
}

unsigned int OBJLoader::AddVertex(Vertex::Basic32 vertex, 
								  std::vector<Vertex::Basic32>& vertexArray, 
								  HashEntry** hashTable, 
								  unsigned int& currentIndex, 
								  unsigned int hashVal)
{
	//check is the vertex is already added using our hash table
	HashEntry* currEntry = hashTable[hashVal];
	HashEntry* prevEntry = NULL;
	while(currEntry != NULL)
	{
		if(vertex.Pos == vertexArray[currEntry->index].Pos && 
			vertex.Tex == vertexArray[currEntry->index].Tex && 
			vertex.Normal == vertexArray[currEntry->index].Normal)
		{
			return currEntry->index;
		}

		prevEntry = currEntry;
		currEntry = currEntry->pNext;
	}

	//if the index wasn't found, create a new entry for our hash table
	currEntry = new HashEntry();

	//create vertex and add to the hash table
	currEntry->index = currentIndex;
	currEntry->pNext = NULL;
	
	//if there is at least one vertex at the current hash key in the table, 
	//link it to the last vertex
	//at this hash key.
	if(prevEntry != NULL)
	{
		prevEntry->pNext = currEntry;
	}
	else
	{
		hashTable[hashVal] = currEntry;
	}

	//actually add our vertex to the array.
	vertexArray.push_back(Vertex::Basic32());
	vertexArray[currentIndex].Pos = vertex.Pos;
	vertexArray[currentIndex].Tex = vertex.Tex;
	vertexArray[currentIndex].Normal = vertex.Normal;
	currentIndex++;

	return currentIndex - 1;
}

void OBJLoader::ReadMaterials(std::ifstream& fin, std::vector<ObjMaterial>& mats)
{
	//setup a default material
	ObjMaterial currMaterial;
	currMaterial.AlphaClip = false;
	currMaterial.DiffuseMapName = L"default_diffuse.png";
	currMaterial.EffectTypeName = "BasicEffect";
	currMaterial.NormalMapName = L"default_normal.png";
	currMaterial.Mat.Ambient = XMFLOAT4(0.5f,0.5f,0.5f,1.0f);
	currMaterial.Mat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	currMaterial.Mat.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	currMaterial.Mat.Reflect = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	currMaterial.name = "default";

	string symbol;

	//handle if the file doesn't exist
	if(!(fin >> symbol))
	{
		OutputDebugString(L"Error loading materials \n");

		mats.push_back(currMaterial);
	}

	//parse the file
	while(!fin.eof())
	{
		//read in until we get newmtl
		while(symbol != "newmtl" && fin >> symbol);

		fin >> symbol;

		//check to find where in our materials array the unloaded 
		//material is.
		int i = 0;
		while(i < mats.size() && symbol != mats[i].name)
		{
			i++;
		}

		//check if the material hasn't been added to the vector yet
		if(i == mats.size())
		{
			//add the material
			currMaterial.name = symbol;
			while((fin >> symbol) && symbol != "newmtl")
			{
				if(symbol == "Ns")
				{
					fin >> currMaterial.Mat.Specular.w;
				}
				else if(symbol == "Ka")
				{
					fin >> currMaterial.Mat.Ambient.x;
					fin >> currMaterial.Mat.Ambient.y;
					fin >> currMaterial.Mat.Ambient.z;
					currMaterial.Mat.Ambient.w = 1.0f;
				}
				else if(symbol == "Kd")
				{
					fin >> currMaterial.Mat.Diffuse.x;
					fin >> currMaterial.Mat.Diffuse.y;
					fin >> currMaterial.Mat.Diffuse.z;
					currMaterial.Mat.Diffuse.w = 1.0f;
				}
				else if(symbol == "Ks")
				{
					fin >> currMaterial.Mat.Specular.x;
					fin >> currMaterial.Mat.Specular.y;
					fin >> currMaterial.Mat.Specular.z;
				}
				else if(symbol == "map_Kd")
				{
					std::string diffuseMapName;
					fin >> diffuseMapName;
					currMaterial.DiffuseMapName.resize(diffuseMapName.size(), ' ');
					std::copy(diffuseMapName.begin(), diffuseMapName.end(), 
								currMaterial.DiffuseMapName.begin());
				}

			}

			if(currMaterial.DiffuseMapName == L"")
			{
				currMaterial.DiffuseMapName = L"default_diffuse";
			}
			mats.push_back(currMaterial);
			
		}
	}

	//Check for the case where no materials are provided and init
	//with a default texture.
	if(mats.size() == 0)
	{
		mats.push_back(currMaterial);
	}
}

UINT OBJLoader::GetSubset(const std::wstring& name, const std::vector<ObjMaterial>& mats)
{
	std::string strName;
	strName.assign(name.begin(), name.end());

	for(UINT i = 0; i < mats.size(); ++i)
	{
		if(mats[i].name == strName)
		{
			return i;
		}
	}

	return 0;
}