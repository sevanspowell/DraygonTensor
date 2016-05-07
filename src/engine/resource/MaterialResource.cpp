#include <cassert>
#include <iostream>
#include <sstream>

#include "engine/Config.h"
#include "engine/common/Common.h"
#include "engine/resource/MaterialResource.h"

namespace ds
{
std::unique_ptr<IResource>
MaterialResource::CreateFromFile(std::string filePath)
{
    std::unique_ptr<IResource> materialResource(nullptr);

    // Open material resource file
    Config config;
    bool didLoad = config.LoadFile(filePath);

    if (didLoad)
    {
        // Create empty material resource
        materialResource = std::unique_ptr<IResource>(new MaterialResource());
        materialResource->SetResourceFilePath(filePath);

        // Get containing folder of this material resource
        std::string folder = ds_com::GetParentDirectory(filePath);

        // Get path to shader to use
        std::string relShaderPath;
        if (config.GetString("shader", &relShaderPath))
        {
            std::stringstream fullShaderPath;
            fullShaderPath << folder << relShaderPath;

            static_cast<MaterialResource *>(materialResource.get())
                ->SetShaderResourceFilePath(fullShaderPath.str());

            // Get path to textures to use
            std::vector<std::string> textureKeys =
                config.GetObjectKeys("textures");

            // For each texture
            for (auto key : textureKeys)
            {
                std::string relTexturePath;

                std::stringstream configKey;
                configKey << "textures"
                          << "." << key;

                if (config.GetString(configKey.str(), &relTexturePath))
                {
                    // Make full texture path from relative one
                    std::stringstream fullTexturePath;
                    fullTexturePath << folder << relTexturePath;

                    // Set texture resource path of texture uniform name
                    static_cast<MaterialResource *>(materialResource.get())
                        ->SetTextureResourceFilePath(key,
                                                     fullTexturePath.str());
                }
                else
                {
                    std::cerr << "MaterialResource::CreateFromFile: could not "
                                 "get texture path from key: '"
                              << configKey.str() << "': " << filePath
                              << std::endl;
                }
            }

            // Get and load uniform blocks
            std::vector<std::string> uniformBlockKeys =
                config.GetObjectKeys("uniforms");

            // For each uniform block
            for (auto uniformBlockName : uniformBlockKeys)
            {
                // Create uniform block
                ds_render::UniformBlock uniformBlock;
                uniformBlock.SetName(uniformBlockName);

                // Get and load uniforms
                std::stringstream uniformsConfigKey;
                uniformsConfigKey << "uniforms"
                                  << "." << uniformBlockName;

                std::vector<std::string> uniformKeys =
                    config.GetObjectKeys(uniformsConfigKey.str());

                // For each uniform
                for (auto uniformName : uniformKeys)
                {
                    // Create new uniform
                    ds_render::Uniform uniform;
                    // Was the uniform read in correctly?
                    bool loadedCorrectly = false;

                    // Set uniform name
                    uniform.SetName(uniformName);

                    // Get type (key)
                    std::stringstream typeKey;
                    typeKey << uniformsConfigKey.str() << "." << uniformName;

                    // Should only be one key, the type
                    std::vector<std::string> typeKeys =
                        config.GetObjectKeys(typeKey.str());
                    if (typeKeys.size() > 0)
                    {
                        // Type of the uniform: int, vec4, float etc.
                        std::string type = typeKeys[0];

                        // Switch on type
                        if (type == "int")
                        {
                            // Set appropriate type
                            uniform.SetDataType(
                                ds_render::Uniform::UniformType::Int);

                            std::stringstream dataKey;
                            dataKey << typeKey.str() << "." << type;

                            // Get uniform data
                            int integer;
                            if (config.GetInt(dataKey.str(), &integer))
                            {
                                uniform.SetUniformData(sizeof(int), &integer);
                                loadedCorrectly = true;
                            }
                        }
                        // TODO: Handle float, vec3, mat4, etc.
                        else if (type == "vec4")
                        {
                            // Set appropriate type
                            uniform.SetDataType(
                                ds_render::Uniform::UniformType::Vec4);

                            std::stringstream dataKey;
                            dataKey << typeKey.str() << "." << type;

                            // Get uniform data
                            std::vector<float> vec4;
                            config.GetFloatArray(dataKey.str(), &vec4);

                            if (vec4.size() == 4)
                            {
                                uniform.SetUniformData(sizeof(float) * 4,
                                                       &vec4[0]);
                                loadedCorrectly = true;
                            }
                            else
                            {
                                std::cerr
                                    << "MaterialResource::CreateFromFile: vec4 "
                                       "'"
                                    << dataKey.str() << "' must be of size 4! "
                                    << "In material resource: " << filePath
                                    << std::endl;
                            }
                        }
                    }
                    else
                    {
                        std::cerr << "MaterialResource::CreateFromFile: could "
                                     "not find "
                                     "uniform data for uniform: "
                                  << uniformName
                                  << "in material resource: " << filePath
                                  << std::endl;
                    }

                    if (loadedCorrectly == true)
                    {
                        uniformBlock.AddUniform(uniform);
                    }
                }

                static_cast<MaterialResource *>(materialResource.get())
                    ->AddUniformBlock(uniformBlock);
            }
        }
        else
        {
            std::cerr << "MaterialResource::CreateFromFile: could not find "
                         "'shader' field in material resource: "
                      << filePath << std::endl;
        }
    }
    else
    {
        std::cerr
            << "MaterialResource::CreateFromFile: could not open material "
               "resource file (json file): "
            << filePath << std::endl;
    }
    // TODO: Parse file

    // TODO: Create mesh resource

    // Return resource (if any)
    return materialResource;
}

const std::string &MaterialResource::GetResourceFilePath() const
{
    return m_filePath;
}

void MaterialResource::SetResourceFilePath(const std::string &filePath)
{
    m_filePath = filePath;
}

const std::string &MaterialResource::GetShaderResourceFilePath() const
{
    return m_shaderPath;
}

void MaterialResource::SetShaderResourceFilePath(
    const std::string &shaderFilePath)
{
    m_shaderPath = shaderFilePath;
}

std::vector<std::string> MaterialResource::GetTextureSamplerNames() const
{
    std::vector<std::string> textureSamplerNames;

    for (auto samplerPath : m_textures)
    {
        textureSamplerNames.push_back(samplerPath.first);
    }

    return textureSamplerNames;
}

const std::string &MaterialResource::GetTextureResourceFilePath(
    const std::string &textureSamplerName) const
{
    std::map<std::string, std::string>::const_iterator it =
        m_textures.find(textureSamplerName);

    if (it == m_textures.end())
    {
        assert("MaterialResource::GetTextureResourceFilePath: No texture with "
               "that sampler name exists.");
    }

    return it->second;
}

void MaterialResource::SetTextureResourceFilePath(
    const std::string &textureUniformName,
    const std::string &textureResourceFilePath)
{
    m_textures[textureUniformName] = textureResourceFilePath;
}

void MaterialResource::AddUniformBlock(
    const ds_render::UniformBlock &uniformBlock)
{
    m_uniformBlocks.push_back(uniformBlock);
}

const std::vector<ds_render::UniformBlock> &
MaterialResource::GetUniformBlocks() const
{
    return m_uniformBlocks;
}

// void MaterialResource::AddUniform(const ds_render::Uniform &uniform)
// {
//     // Find a uniform with the same name
//     std::vector<ds_render::Uniform>::iterator it =
//         find_if(m_uniforms.begin(), m_uniforms.end(),
//                 [&](const ds_render::Uniform &materialUniform)
//                 {
//                     if (materialUniform.GetName() == uniform.GetName())
//                     {
//                         return true;
//                     }
//                     else
//                     {
//                         return false;
//                     }
//                 });

//     // If one exists
//     if (it != m_uniforms.end())
//     {
//         // Overwrite it
//         (*it) = uniform;
//     }
//     else
//     {
//         // Add a new uniform
//         m_uniforms.push_back(uniform);
//     }
// }


// const std::vector<ds_render::Uniform> &MaterialResource::GetUniforms() const
// {
//     return m_uniforms;
// }
}
