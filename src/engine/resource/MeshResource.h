/**
 * Skeletal animation techniques from Etay Meiri
 * http://www.ogldev.org/www/tutorial38/tutorial38.html
 */
#include <map>
#include <memory>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "math/Vector3.h"
#include "math/Matrix4.h"
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
    struct VertexBoneData;

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
     */
    MeshResource();

    /**
     * Destructor.
     */
    ~MeshResource();

    /**
     * Get the file path to the resource.
     *
     * @return  const std::string &, resource file path.
     */
    virtual const std::string &GetResourceFilePath() const;

    /**
     * Set the file path to the resource.
     *
     * @param  filePath  const std::string &, file path of this resource.
     */
    virtual void SetResourceFilePath(const std::string &filePath);

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
     * Set the mesh count.
     *
     * @param  meshCount  unsigned int, number of meshes.
     */
    void SetMeshCount(unsigned int meshCount);

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
     * Get the importer this mesh uses.
     *
     * @return  Assimp::Importer &, importer used to load mesh.
     */
    Assimp::Importer &GetImporter();

    /**
     * Get the scene containing this mesh.
     *
     * @return  const aiScene *, scene containing this mesh.
     */
    const aiScene *GetScene() const;

    /**
     * Set the scene containing this mesh.
     *
     * @param  scene  const aiScene *, scene containing this mesh.
     */
    void SetScene(const aiScene *scene);

    /**
     * Get the global inverse transform of this mesh.
     *
     * @return  const ds_math::Matrix4 &, global inverse transform matrix.
     */
    const ds_math::Matrix4 &GetGlobalInverseTransform() const;

    /**
     * Set this mesh's global inverse transform.
     *
     * @param  globalInvTransfrom  const ds_math::Matrix4 &, global inverse
     * transform matrix.
     */
    void SetGlobalInverseTransform(const ds_math::Matrix4 &globalInvTransform);

    /**
     * Get the number of bones in the mesh.
     *
     * @return  unsigned int, number of bones in the mesh.
     */
    unsigned int GetBoneCount() const;

    /**
     * Set the number of bones in the mesh.
     *
     * @param  boneCount  unsigned int, number of bones in the mesh.
     */
    void SetBoneCount(unsigned int boneCount);

    /**
     * Get a collection of bone transforms.
     *
     * @param  deltaTime   float, time since last animation sample in
     *                     seconds.
     * @param  transforms  std::vector<ds_math::Matrix4> *, array of
     *                     transforms out. Vector must have
     *                     MeshResource::MAX_BONES elements.
     */
    void BoneTransform(float deltaTime,
                       std::vector<ds_math::Matrix4> *transforms);

    /**
     * Set the index of the animation to be sampled by the BoneTransform
     * method.
     *
     * @param  animationIndex  int, index of animation to be sampled by the
     * BoneTransform method.
     */
    void SetAnimationIndex(int animationIndex);

    /**
     * Get the number of bones in the scene.
     *
     * @return  unsigned int, number of bones in the scene.
     */
    unsigned int GetNumBones() const;

    const std::vector<VertexBoneData> &GetVertexBoneData() const;

    void SetVertexBoneData(const std::vector<VertexBoneData> &vertexBoneData);

    size_t GetBaseVertex(unsigned int meshIndex) const;

    size_t GetBaseIndex(unsigned int meshIndex) const;

    size_t GetNumIndices(unsigned int meshIndex) const;

    /** Maximum number of bones */
    static const int MAX_BONES = 100;

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

    /** Maximum number of bones that can influence one vertice */
    static const int NUM_BONES_PER_VERTEX = 4;

    /** Vertex bone data */
    struct VertexBoneData
    {
        unsigned int ids[NUM_BONES_PER_VERTEX];
        float weights[NUM_BONES_PER_VERTEX];

        VertexBoneData()
        {
            Reset();
        };

        void Reset()
        {
            // Zero memory of structure
            memset(ids, 0, NUM_BONES_PER_VERTEX * sizeof(unsigned int));
            memset(weights, 0, NUM_BONES_PER_VERTEX * sizeof(float));
        };

        void AddBoneData(unsigned int boneId, float weight)
        {
            // Search for empty slot (weight = 0.0)
            for (unsigned int i = 0; i < NUM_BONES_PER_VERTEX; ++i)
            {
                if (weights[i] == 0.0f)
                {
                    ids[i] = boneId;
                    weights[i] = weight;
                    return;
                }
            }

            assert(false && "Tried to add bone to full vertex bone data "
                            "structure - no more room.");
        };
    };

    /** Bone information */
    struct BoneInfo
    {
        ds_math::Matrix4 boneOffset;
        ds_math::Matrix4 finalTransform;
    };

private:
    /**
     * Stores mesh positions.
     *
     * @param	meshNumber		  	The mesh number.
     * @param [in,out]	singleMesh	If non-null, the single mesh.
     */
    void StoreMeshPositions(unsigned int meshNumber, aiMesh *singleMesh);

    /**
     * Stores texture coordinates.
     *
     * @param	meshNumber		  	The mesh number.
     * @param [in,out]	singleMesh	If non-null, the single mesh.
     */
    void StoreTextureCoords(unsigned int meshNumber, aiMesh *singleMesh);

    /**
     * Stores normal coordinates.
     *
     * @param	meshNumber		  	The mesh number.
     * @param [in,out]	singleMesh	If non-null, the single mesh.
     */
    void StoreNormalCoords(unsigned int meshNumber, aiMesh *singleMesh);

    /**
     * Stores the faces.
     *
     * @param	meshNumber		  	The mesh number.
     * @param [in,out]	singleMesh	If non-null, the single mesh.
     */
    void StoreFaces(unsigned int meshNumber, aiMesh *singleMesh);

    /**
     * Load vertex bone data into bone array.
     *
     * @param  meshIndex  unsigned int, index of mesh to load bone data of in
     * scene.
     * @param  mesh       const aiMesh *, mesh to load bone data from.
     * @param  bones      std::vector<VertexBoneData> *, bone data out.
     */
    void LoadBones(unsigned int meshIndex,
                   const aiMesh *mesh,
                   std::vector<VertexBoneData> *bones);

    /**
     * Recursive method.
     *
     * Get transformation of node at a given time.
     */
    void ReadNodeHeirarchy(float animationTime,
                           const aiNode *node,
                           const ds_math::Matrix4 &parentTransform);

    /**
     * Interpolate between key frames to find the scale of a vertex at the
     * given animation time.
     *
     * @param   animationTime  float, animation time to find scale of vertex at.
     * @param   nodeAnim       const aiNodeAnim *, animation data structure.
     * @return                 aiVector3D, scale of vertex at given animation
     * time.
     */
    aiVector3D CalcInterpolatedScaling(float animationTime,
                                       const aiNodeAnim *nodeAnim);

    /**
     * Interpolate between key frames to find the rotation of a vertex at the
     * given animation time.
     *
     * @param   animationTime  float, animation time to find rotation of vertex
     *                         at.
     * @param   nodeAnim       const aiNodeAnim *, animation data structure.
     * @return                 aiQuaternion, rotation of vertex at given
     *                         animation time.
     */
    aiQuaternion CalcInterpolatedRotation(float animationTime,
                                          const aiNodeAnim *nodeAnim);

    /**
     * Interpolate between key frames to find the position of a vertex at the
     * given animation time.
     *
     * @param   animationTime  float, animation time to find position of vertex
     *                         at.
     * @param   nodeAnim       const aiNodeAnim *, animation data structure.
     * @return                 aiVector3D, position of vertex at given animation
     *                         time.
     */
    aiVector3D CalcInterpolatedPosition(float animationTime,
                                        const aiNodeAnim *nodeAnim);

    /**
     * Find the index of the scaling key frame just before the given animation
     * time in the given animation.
     *
     * @param   animationTime  float, time in the animation from which to find
     *                         the nearest previous scaling key frame index.
     * @param   nodeAnim       const aiNodeAnim *, animation data to find
     *                         scaling key frame from.
     * @return                 unsigned int, index of scaling key frame just
     *                         previous to the given animation time.
     */
    unsigned int FindScaling(float animationTime, const aiNodeAnim *nodeAnim);

    /**
     * Find the index of the rotation key frame just before the given animation
     * time in the given animation.
     *
     * @param   animationTime  float, time in the animation from which to find
     *                         the nearest previous rotation key frame index.
     * @param   nodeAnim       const aiNodeAnim *, animation data to find
     *                         rotation key frame from.
     * @return                 unsigned int, index of rotation key frame just
     *                         previous to the given animation time.
     */
    unsigned int FindRotation(float animationTime, const aiNodeAnim *nodeAnim);

    /**
     * Find the index of the position key frame just before the given animation
     * time in the given animation.
     *
     * @param   animationTime  float, time in the animation from which to find
     *                         the nearest previous position key frame index.
     * @param   nodeAnim       const aiNodeAnim *, animation data to find
     *                         position key frame from.
     * @return                 unsigned int, index of position key frame just
     *                         previous to the given animation time.
     */
    unsigned int FindPosition(float animationTime, const aiNodeAnim *nodeAnim);

    const aiNodeAnim *FindNodeAnim(const aiAnimation *animation,
                                   const std::string &nodeName);

    void SetMeshEntry(size_t meshIndex,
                      size_t baseVertex,
                      size_t baseIndex,
                      size_t numIndices);

    void SetPositionBufferSize(size_t size);

    void SetNormalBufferSize(size_t size);

    void SetTexCoordBufferSize(size_t size);

    void SetIndexBufferSize(size_t size);

    void LoadMeshData(unsigned int meshIndex, const aiMesh *mesh);


    /** Collection of meshes. */
    std::vector<struct SingularMesh> m_meshCollection;
    /** The path to this resource */
    std::string m_filePath;
    /** Our importer, we need to keep this around for animation */
    Assimp::Importer m_importer;
    /** Our scene, we need to keep this around for animation */
    const aiScene *m_scene;

    /** Map bone name to bone index */
    std::map<std::string, int> m_boneMapping;
    unsigned int m_numBones;
    std::vector<BoneInfo> m_boneInfo;
    /** Transformation relative to the node's parent */
    ds_math::Matrix4 m_globalInverseTransform;
    /** Vertex bone data */
    std::vector<VertexBoneData> m_vertexBoneData;

    struct MeshEntry
    {
        MeshEntry()
        {
            baseVertex = 0;
            baseIndex = 0;
            numIndices = 0;
        }

        /** Where in the vertex buffer to set as index 0 */
        size_t baseVertex;
        /** Where in index buffer to begin mesh indices */
        size_t baseIndex;
        /** Number of indices used to create mesh */
        size_t numIndices;
    };

    std::vector<ds_math::Vector3> m_positionBuffer;
    std::vector<ds_math::Vector3> m_normalBuffer;
    std::vector<ds_math::Vector3> m_texCoordBuffer;
    std::vector<unsigned int> m_indexBuffer;
    std::vector<MeshEntry> m_meshEntries;

    // Material information - diffuse texture path.
    std::vector<std::string> m_diffuseTexturePaths;

    // Time accumulator (for animation)
    float m_animationTime;
    // Current animation to use
    int m_currentAnimationIndex;
};
}
