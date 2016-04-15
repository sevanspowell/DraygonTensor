#include <memory>
#include <vector>
#include <assimp/Importer.hpp>     
#include <assimp/scene.h>   
#include <assimp/postprocess.h>

#include "math/Vector3.h"
#include "engine/resource/IResource.h"

namespace ds
{
/**
 * A mesh resource contains all the data in a mesh resource file, for now this
 * is simply the vertices, normals, texture coordinates and indices of the mesh.
 */
class MeshResource : public IResource
{
public:

    /**
     * Create a mesh resource from file.
     *
     * @param   filePath  std::string , file path to create mesh resource
     * from.
     * @return          std::unique_ptr<IResource>, pointer to mesh resource
     * created.
     */
    static std::unique_ptr<IResource> CreateFromFile(std::string filePath);

	/**
	 * Constructor.
	 *
	 * @param	numMeshes	Number of meshes in the imported model.
	 */

	MeshResource(unsigned int numMeshes);

	/**
	 * Destructor.
	 */

	~MeshResource();

	/**
	 * Gets number of verts in the first mesh.
	 *
	 * @return	The vertical count.
	 */

	unsigned int GetVertCount() const;

	/**
	 * Gets count of tex coordinates for the first mesh.
	 *
	 * @return	The tex coordinate count.
	 */

	unsigned int GetTexCoordCount() const;

	/**
	 * Gets normals count for the first mesh.
	 *
	 * @return	The normals count.
	 */

	unsigned int GetNormalsCount() const;

	/**
	 * Gets indices count for first mesh.
	 *
	 * @return	The indices count.
	 */

	unsigned int GetIndicesCount() const;

	/**
	 * Gets the vertice count for the specified
	 * mesh.
	 *
	 * @param	meshNumber	The mesh number.
	 * @return	The verts count for specified mesh.
	 */

	unsigned int GetVertCount(unsigned int meshNumber) const;

	/**
	 * Gets tex coordinate count for the specified
	 * mesh.
	 *
	 * @param	meshNumber	The mesh number.
	 * @return	The tex coordinate count for specified mesh.
	 */

	unsigned int GetTexCoordCount(unsigned int meshNumber) const;

	/**
	 * Gets normals count for the specified mesh.
	 *
	 * @param	meshNumber	The mesh number.
	 * @return	The normals count for specified mesh.
	 */

	unsigned int GetNormalsCount(unsigned int meshNumber) const;

	/**
	 * Gets indices count for the specified mesh.
	 *
	 * @param	meshNumber	The mesh number.
	 * @return	The indices count for the mesh number.
	 */

	unsigned int GetIndicesCount(unsigned int meshNumber) const;

	/**
	 * Gets mesh count.
	 *
	 * @return	The mesh count.
	 */

	unsigned int GetMeshCount() const;

	/**
	 * Query if this object has single mesh.
	 *
	 * @return	true if single mesh, false if not.
	 */

	bool HasSingleMesh() const;

	/**
	 * Query if this object has multi mesh.
	 *
	 * @return	true if multi mesh, false if not.
	 */

	bool HasMultiMesh() const;

	/**
	 * Gets the vertices for first mesh.
	 *
	 * @return	The vertices for the first mesh.
	 */

	std::vector<ds_math::Vector3> GetVerts() const;

	/**
	 * Gets tex coordinates for the first mesh.
	 *
	 * @return	The tex coordinates for first mesh.
	 */

	std::vector<ds_math::Vector3> GetTexCoords() const;

	/**
	 * Gets the normals for the first mesh.
	 *
	 * @return	The normals for the first mesh.
	 */

	std::vector<ds_math::Vector3> GetNormals() const;

	/**
	 * Gets the indices for the first mesh.
	 *
	 * @return	The indices.
	 */

	std::vector<unsigned int> GetIndices() const;

	/**
	 * Gets the vertices for specified mesh.
	 *
	 * @param	meshNumber	The mesh number.
	 * @return	The vertices.
	 */

	std::vector<ds_math::Vector3> GetVerts(unsigned int meshNumber) const;

	/**
	 * Gets tex coordinates for specified mesh.
	 * 
	 * @param	meshNumber	The mesh number.
	 * @return	The tex coordinates.
	 */

	std::vector<ds_math::Vector3> GetTexCoords(unsigned int meshNumber) const;

	/**
	 * Gets the normals for specified mesh.
	 * 
	 * @param	meshNumber	The mesh number.
	 * @return	The normals.
	 */

	std::vector<ds_math::Vector3> GetNormals(unsigned int meshNumber) const;

	/**
	 * Gets the indices for specified mesh.
	 * 
	 * @param	meshNumber	The mesh number.
	 * @return	The indices.
	 */

	std::vector<unsigned int> GetIndices(unsigned int meshNumber) const;

	/**
	 * Represents a single mesh.
	 */

	struct SingularMesh
	{
		/** The vertices. */
		std::vector<ds_math::Vector3> m_vertices;
		/** The tex coordinates. */
		std::vector<ds_math::Vector3> m_texCoords;
		/** The normals. */
		std::vector<ds_math::Vector3> m_normals;
		/** The indices. */
		std::vector<unsigned int> m_indices;
	};

private:

	/** Collection of meshes. */
	std::vector<struct SingularMesh> m_meshCollection;

	/**
	 * Stores mesh positions.
	 *
	 * @param	meshNumber		  	The mesh number.
	 * @param [in,out]	singleMesh	If non-null, the single mesh.
	 */

	void StoreMeshPositions(unsigned int meshNumber, aiMesh * singleMesh);

	/**
	 * Stores texture coordinates.
	 *
	 * @param	meshNumber		  	The mesh number.
	 * @param [in,out]	singleMesh	If non-null, the single mesh.
	 */

	void StoreTextureCoords(unsigned int meshNumber, aiMesh * singleMesh);

	/**
	 * Stores normal coordinates.
	 *
	 * @param	meshNumber		  	The mesh number.
	 * @param [in,out]	singleMesh	If non-null, the single mesh.
	 */

	void StoreNormalCoords(unsigned int meshNumber, aiMesh * singleMesh);

	/**
	 * Stores the faces.
	 *
	 * @param	meshNumber		  	The mesh number.
	 * @param [in,out]	singleMesh	If non-null, the single mesh.
	 */

	void StoreFaces(unsigned int meshNumber, aiMesh * singleMesh);
};
}
