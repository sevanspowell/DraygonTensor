#include <assimp/Importer.hpp>

#include "engine/resource/MeshResource.h"

namespace ds
{
	std::unique_ptr<IResource> MeshResource::CreateFromFile(std::string filePath)
	{
		Assimp::Importer importer;

		const aiScene * ourScene = importer.ReadFile(filePath.c_str(),
			aiProcess_Triangulate);
		
		
		if (!ourScene)
		{
			std::cout << "Could not load model: " << std::endl;
			std::cout << filePath << std::endl;
			std::cout << importer.GetErrorString() << std::endl;
		}

		int meshCount = ourScene->mNumMeshes;
		std::unique_ptr<MeshResource> meshCollection(new MeshResource(meshCount));
		
		for (unsigned int iMesh = 0; iMesh < meshCount; iMesh++)
		{
			meshCollection->StoreMeshPositions(iMesh, ourScene->mMeshes[iMesh]);
			meshCollection->StoreNormalCoords(iMesh, ourScene->mMeshes[iMesh]);
			meshCollection->StoreTextureCoords(iMesh, ourScene->mMeshes[iMesh]);
			meshCollection->StoreFaces(iMesh, ourScene->mMeshes[iMesh]);
		}
		
		importer.FreeScene();;
		std::unique_ptr<IResource> meshResource = std::move(meshCollection);

		return meshResource;
	}


	MeshResource::MeshResource(unsigned int numMeshes)
	{
		m_meshCollection.resize(numMeshes);
	}


	MeshResource::~MeshResource()
	{
		for (int iMesh = 0; iMesh < m_meshCollection.size(); iMesh++)
		{
			m_meshCollection[iMesh].m_vertices.clear();
			m_meshCollection[iMesh].m_texCoords.clear();
			m_meshCollection[iMesh].m_normals.clear();
			m_meshCollection[iMesh].m_indices.clear();
		}

		m_meshCollection.clear();
	}


	void MeshResource::StoreFaces(unsigned int meshNumber, aiMesh * singleMesh)
	{
		if (singleMesh->HasFaces())
		{
			int numOfFaces = singleMesh->mNumFaces;
			m_meshCollection[meshNumber].m_indices.reserve(numOfFaces * 3);

			for (int iFaces = 0; iFaces < numOfFaces; iFaces++)
			{
				unsigned int firstInd;
				unsigned int secondInd;
				unsigned int thirdInd;

				firstInd = singleMesh->mFaces[iFaces].mIndices[0];
				secondInd = singleMesh->mFaces[iFaces].mIndices[1];
				thirdInd = singleMesh->mFaces[iFaces].mIndices[2];

				m_meshCollection[meshNumber].m_indices.push_back(firstInd);
				m_meshCollection[meshNumber].m_indices.push_back(secondInd);
				m_meshCollection[meshNumber].m_indices.push_back(thirdInd);
			}
		}
	}


	void MeshResource::StoreMeshPositions(unsigned int meshNumber, aiMesh * singleMesh)
	{
		if (singleMesh->HasPositions())
		{
			
			int numberOfVerts = singleMesh->mNumVertices;
			m_meshCollection[meshNumber].m_vertices.reserve(numberOfVerts);

			for (int iVert = 0; iVert < numberOfVerts; iVert++)
			{
				ds_math::Vector3 singlePos;
				singlePos.x = singleMesh->mVertices[iVert].x;
				singlePos.y = singleMesh->mVertices[iVert].y;
				singlePos.z = singleMesh->mVertices[iVert].z;
				m_meshCollection[meshNumber].m_vertices.push_back(singlePos);

			}
		}
	}


	void MeshResource::StoreTextureCoords(unsigned int meshNumber, aiMesh * singleMesh)
	{
		
		int textExistIdxIndicator = 0;

		if (singleMesh->HasTextureCoords(textExistIdxIndicator))
		{

			int numberOfVerts = singleMesh->mNumVertices;
			m_meshCollection[meshNumber].m_texCoords.resize(numberOfVerts);

			for (int iTex = 0; iTex < numberOfVerts; iTex++)
			{
				ds_math::Vector3 singleTexCoord;
				singleTexCoord.x = singleMesh->mTextureCoords[0][iTex].x;
				singleTexCoord.y = singleMesh->mTextureCoords[0][iTex].y;
				singleTexCoord.z = 0.0f;
				m_meshCollection[meshNumber].m_texCoords.push_back(singleTexCoord);
			}
		}
	}


	void MeshResource::StoreNormalCoords(unsigned int meshNumber, aiMesh * singleMesh)
	{
		if (singleMesh->HasNormals())
		{
			int numberOfVerts = singleMesh->mNumVertices;
			m_meshCollection[meshNumber].m_normals.resize(numberOfVerts);

			for (int iNorms = 0; iNorms < numberOfVerts; iNorms++)
			{
				ds_math::Vector3 singleNormCoords;
				singleNormCoords.x = singleMesh->mNormals[iNorms].x;
				singleNormCoords.y = singleMesh->mNormals[iNorms].y;
				singleNormCoords.z = singleMesh->mNormals[iNorms].z;
				m_meshCollection[meshNumber].m_normals.push_back(singleNormCoords);
			}
		}

	}


	unsigned int MeshResource::GetVertCount(unsigned int meshNumber) const
	{
		unsigned int verts = 0;

		if (m_meshCollection.size() >= meshNumber) {
			verts = m_meshCollection[meshNumber].m_vertices.size();
		}

		return verts;
	}


	unsigned int MeshResource::GetTexCoordCount(unsigned int meshNumber) const
	{
		unsigned int texCoords = 0;

		if (m_meshCollection.size() >= meshNumber) {
			texCoords = m_meshCollection[meshNumber].m_texCoords.size();
		}

		return texCoords;
	}


	unsigned int MeshResource::GetNormalsCount(unsigned int meshNumber) const
	{
		unsigned int norms = 0;

		if (m_meshCollection.size() >= meshNumber) {
			norms = m_meshCollection[meshNumber].m_normals.size();
		}

		return norms;
	}


	unsigned int MeshResource::GetIndicesCount(unsigned int meshNumber) const
	{
		unsigned int indcs = 0;
		
		if (m_meshCollection.size() >= meshNumber) {
			indcs = m_meshCollection[meshNumber].m_indices.size();
		}

		return indcs;
	}


	bool MeshResource::HasSingleMesh() const
	{
		bool flag = false;

		if (m_meshCollection.size() == 1)
		{
			flag = true;
		}

		return flag;
	}


	bool MeshResource::HasMultiMesh() const
	{
		bool flag = false;

		if (m_meshCollection.size() > 1)
		{
			flag = true;
		}

		return flag;
	}


	unsigned int MeshResource::GetVertCount() const
	{
		return m_meshCollection[0].m_vertices.size();
	}


	unsigned int MeshResource::GetTexCoordCount() const
	{
		return m_meshCollection[0].m_texCoords.size();
	}


	unsigned int MeshResource::GetNormalsCount() const
	{
		return m_meshCollection[0].m_normals.size();
	}


	unsigned int MeshResource::GetIndicesCount() const
	{
		return m_meshCollection[0].m_indices.size();
	}


	unsigned int MeshResource::GetMeshCount() const
	{
		return m_meshCollection.size();;
	}


	std::vector<ds_math::Vector3> MeshResource::GetVerts() const
	{
		return m_meshCollection[0].m_vertices;
	}


	std::vector<ds_math::Vector3> MeshResource::GetTexCoords() const
	{
		return m_meshCollection[0].m_texCoords;
	}


	std::vector<ds_math::Vector3> MeshResource::GetNormals() const
	{
		return m_meshCollection[0].m_normals;
	}


	std::vector<unsigned int> MeshResource::GetIndices() const
	{
		return m_meshCollection[0].m_indices;
	}


	std::vector<ds_math::Vector3> MeshResource::GetVerts(unsigned int meshNumber) const
	{
		return m_meshCollection[meshNumber].m_vertices;
	}


	std::vector<ds_math::Vector3> MeshResource::GetTexCoords(unsigned int meshNumber) const
	{
		return m_meshCollection[meshNumber].m_texCoords;
	}


	std::vector<ds_math::Vector3> MeshResource::GetNormals(unsigned int meshNumber) const
	{
		return m_meshCollection[meshNumber].m_normals;
	}


	std::vector<unsigned int> MeshResource::GetIndices(unsigned int meshNumber) const
	{
		return m_meshCollection[meshNumber].m_indices;
	}
}