#include <map>
#include <memory>
#include <string>
#include <vector>

#include "engine/resource/IResource.h"
#include "engine/system/render/RenderCommon.h"

namespace ds
{
class ShaderResource : public IResource
{
public:
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
     * Add a shader source to the shader resource.
     *
     * Only one of each shader type may exist in a shader resource. Will
     * overwrite old shader source of that type if one exists.
     *
     * @param  type    ds_render::ShaderType, type of the shader source.
     * @param  source  std::string, shader source as string.
     */
    void AddSource(ds_render::ShaderType type, std::string source);

    /**
     * Get a list of shader types present in this shader resource.
     *
     * @return  std::vector<ds_render::ShaderType>, list of shader types present
     * in this shader resource.
     */
    std::vector<ds_render::ShaderType> GetShaderTypes() const;

    /**
     * Get the shader source of the given shader type.
     *
     * @pre  ShaderResource has a shader source of that type (use GetShaderTypes
     * to check).
     *
     * @param   type  ds_render::ShaderType, type of the shader source to get.
     * return         const std::string &, shader source of the given type.
     */
    const std::string &GetShaderSource(ds_render::ShaderType type) const;

    /**
     * Create a shader resource from file.
     *
     * @param   filePath  std::string, file path to create shader resource
     * from.
     * @return          std::unique_ptr<IResource>, pointer to shader resource
     * created.
     */
    static std::unique_ptr<IResource> CreateFromFile(std::string filePath);

private:
    /** The path to this resource */
    std::string m_filePath;

    /** Map shader type to shader source */
    std::map<ds_render::ShaderType, std::string> m_sources;
};
}
