#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "engine/resource/IResource.h"
#include "engine/system/render/RenderCommon.h"
#include "engine/system/render/ShaderParameter.h"

namespace ds
{
/**
 * @author Samuel Evans-Powell
 */
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
     * Add a new empty texture sampler to the material resource with no texture
     * file paths. If a texture sampler with that name already exists, it will
     * be erased and a blank sampler entry created.
     *
     * @param  samplerName  const std::string &, new texture sampler name.
     * @param  textureResourceFilePath  const std::string &, path to texture
     * resource to set for this texture sampler.
     */
    void AddTextureSampler(const std::string &samplerName,
                           const std::string &textureResourceFilePath);

    /**
     * Get the texture resource file path for the sampler with the given
     * sampler name (corresponding to the name of the texture sampler used in
     * the shader).
     *
     * If no texture sampler with that name, returns empty string.
     *
     * @param   samplerName  const std::string &, sampler name of the texture.
     * @return               std::string, texture resource file
     *                       path for the given texture sampler name.
     */
    std::string
    GetSamplerTextureResourceFilePath(const std::string &samplerName) const;

    /**
     * Get the shader parameters associated with this material.
     *
     * @return  const std::vector<ds_render::ShaderParameter> &, shader
     * parameters associated with this material.
     */
    const std::vector<ds_render::ShaderParameter> &
    GetMaterialParameters() const;

    /**
     * Add a material parameter to this material resource.
     *
     * @param  parameter  const ShaderParameter &, material parameter to add to
     * this material resource.
     */
    void AddMaterialParameter(const ds_render::ShaderParameter &parameter);

private:
    /**
     * A collection of textures that exist for a given texture type. Texture
     * types might be 2D (1 image, 2D sampler), 3D (1 image, 3D sampler),
     * Cubemap (6 images, 3D sampler) or HDR (1 image, 3D sampler).
     */
    struct SamplerEntry
    {
        /** Texture resource file path for the texture to be bound to this
         * sampler */
        std::string textureResourceFilePath;
    };

    /** Path to material shader file */
    std::string m_shaderPath;
    /** Map texture sampler name to SamplerEntry */
    std::map<std::string, SamplerEntry> m_textureSamplers;
    /** Material parameters */
    std::vector<ds_render::ShaderParameter> m_parameters;
    /** This resource's file path */
    std::string m_filePath;
};
}
