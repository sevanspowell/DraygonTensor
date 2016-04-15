#include <memory>
#include <vector>

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

private:
    // TODO: std::vector<ds_math::Vector3> m_vertices;
    // TODO: std::vector<ds_math::Vector3> m_texCoords;
    // TODO: std::vector<ds_math::Vector3> m_normals;
    // TODO: std::vector<unsigned int> m_indices;
    /** The path to this resource */
    std::string m_filePath;
};
}
