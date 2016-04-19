#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "engine/resource/IResource.h"
#include "engine/system/render/UniformBlock.h"

namespace ds
{
class MaterialResource : public IResource
{
public:
    /**
     * Create a material resource from file.
     *
     * @param   filePath  std::string, file path to create material resource
     * from.
     * @return            std::unique_ptr<IResource>, pointer to material
     * resource created.
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

    /**
     * Return the path to the shader resource that this material resource uses.
     *
     * @return  const std::string &, file path to shader resource.
     */
    const std::string &GetShaderResourceFilePath() const;

    /**
     * Set the path to the shader resource that this material resource uses.
     *
     * @param  shaderFilePath  const std::string &, file path to shader
     * resource.
     */
    void SetShaderResourceFilePath(const std::string &shaderFilePath);

    /**
     * Get a list of texture sampler names this material resource contains.
     *
     * Corresponds to the name used in the shader for the texture.
     *
     * @return  std::vector<std::string>, list of texture sampler names.
     */
    std::vector<std::string> GetTextureSamplerNames() const;

    /**
     * Get the texture resource file path for the texture with the given texture
     * sampler name (corresponding to the name of the texture used in the
     * shader).
     *
     * @pre  textureSamplerName must exist, use GetTextureSamplerNames to check.
     *
     * @param   textureSamplerName  const std::string &, sampler name of the
     * texture.
     * @return                      const std::string &, texture resource file
     * path for the given texture sampler name.
     */
    const std::string &
    GetTextureResourceFilePath(const std::string &textureSamplerName) const;

    /**
     * Set the texture resource file path of a given texture uniform name.
     *
     * If textureUniformName already exists, will overwrite
     * textureResourceFilePath, otherwise will add new texture entry.
     *
     * @param  textureUniformName  const std::string &, uniform name of the
     * texture.
     * @param  textureResourceFilePath  const std::string &, path to the texture
     * resource.
     */
    void SetTextureResourceFilePath(const std::string &textureUniformName,
                                    const std::string &textureResourceFilePath);

    /**
     * Add a uniform block to the material.
     *
     * @param  uniformBlock  const ds_render::UniformBlock &, uniform block.
     */
    void AddUniformBlock(const ds_render::UniformBlock &uniformBlock);

    /**
     * Get all uniform blocks associated with this material.
     *
     * @return const std::vector<ds_render::UniformBlock>
     */
    const std::vector<ds_render::UniformBlock> &GetUniformBlocks() const;

private:
    /** Path to material shader file */
    std::string m_shaderPath;
    /** Map texture sampler name to path of texture */
    std::map<std::string, std::string> m_textures;
    /** Uniform blocks */
    std::vector<ds_render::UniformBlock> m_uniformBlocks;
    /** This resource's file path */
    std::string m_filePath;
};
}
