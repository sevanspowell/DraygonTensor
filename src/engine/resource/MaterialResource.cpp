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

            // Get path to texture samplers to use
            std::vector<std::string> textureKeys =
                config.GetObjectKeys("textures");

            // For each texture sampler, grab texture sampler details and add it
            // to the material resource
            for (auto key : textureKeys)
            {
                // Get path to sampler
                std::stringstream samplerKey;
                samplerKey << "textures"
                           << "." << key;

                // Get path to type of sampler
                std::stringstream samplerTypeKey;
                samplerTypeKey << samplerKey.str() << "."
                               << "type";

                // Convert type of the sampler as a string to an enum
                std::string samplerTypeAsString;
                // Type of the sampler as an enum
                ds_render::TextureType samplerType =
                    ds_render::TextureType::None;
                if (config.GetString(samplerTypeKey.str(),
                                     &samplerTypeAsString))
                {
                    if (samplerTypeAsString == "2D")
                    {
                        samplerType = ds_render::TextureType::TwoDimensional;
                    }
                    else if (samplerTypeAsString == "Cubemap")
                    {
                        samplerType = ds_render::TextureType::Cubemap;
                    }

                    // Create new texture sampler
                    static_cast<MaterialResource *>(materialResource.get())
                        ->AddTextureSampler(key, samplerType);
                }

                // Get the sampler object keys, we're going to iterate thru each
                // key (skipping the type key), and add the texture file path to
                // the material resource
                std::vector<std::string> samplerObjectKeys =
                    config.GetObjectKeys(samplerKey.str());
                // Check that the correct number of textures exist for the given
                // sampler type (samplerObjectsKeys.size() - 1) to ignore the
                // type key.
                switch (samplerType)
                {
                case ds_render::TextureType::TwoDimensional:
                {
                    if ((samplerObjectKeys.size() - 1) != 1)
                    {
                        std::cerr
                            << "MaterialResource::CreateFromFile: "
                               "incorrect number of textures specified (not 1) "
                               "for 2D texture for sampler '"
                            << key << "' in file: " << filePath << std::endl;
                    }
                    break;
                }
                case ds_render::TextureType::Cubemap:
                {
                    if ((samplerObjectKeys.size() - 1) != 6)
                    {
                        std::cerr
                            << "MaterialResource::CreateFromFile: "
                               "incorrect number of textures specified (not 6)"
                               "for cubemap texture for sampler '"
                            << key << "' in file: " << filePath << std::endl;
                    }
                    break;
                }
                default:
                {
                    std::cerr << "MaterialResource::CreateFromFile: "
                                 "Unknown sampler type '"
                              << samplerTypeAsString << "' for sampler '" << key
                              << "' in file: " << filePath << std::endl;
                    break;
                }
                }
                for (unsigned int i = 1; i < samplerObjectKeys.size(); ++i)
                {
                    // Key to the file path in the sampler object
                    std::stringstream textureFilePathKey;
                    textureFilePathKey << samplerKey.str() << "."
                                       << samplerObjectKeys[i];

                    // Get file path of that texture
                    std::string relTexturePath;
                    if (config.GetString(textureFilePathKey.str(),
                                         &relTexturePath))
                    {
                        // Make full texture path from relative one
                        std::stringstream fullTexturePath;
                        fullTexturePath << folder << relTexturePath;

                        // Add texture resource file path to this texture
                        // sampler
                        static_cast<MaterialResource *>(materialResource.get())
                            ->AddTextureSamplerFilePath(key,
                                                        fullTexturePath.str());
                    }
                    else
                    {
                        std::cerr
                            << "MaterialResource::CreateFromFile: could not "
                               "get texture path from key: '"
                            << textureFilePathKey.str() << "': " << filePath
                            << std::endl;
                    }
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

    for (auto samplerPath : m_textureSamplers)
    {
        textureSamplerNames.push_back(samplerPath.first);
    }

    return textureSamplerNames;
}

ds_render::TextureType
MaterialResource::GetTextureTextureType(const std::string &samplerName) const
{
    ds_render::TextureType type = ds_render::TextureType::None;

    std::map<std::string, SamplerEntry>::const_iterator it =
        m_textureSamplers.find(samplerName);

    if (it != m_textureSamplers.end())
    {
        type = it->second.type;
    }

    return type;
}

void MaterialResource::AddTextureSampler(const std::string &samplerName,
                                         const ds_render::TextureType &type)
{
    SamplerEntry entry;
    entry.type = type;
    entry.filePaths = std::vector<std::string>();

    m_textureSamplers[samplerName] = entry;
}

std::vector<std::string> MaterialResource::GetTextureSamplerFilePaths(
    const std::string &samplerName) const
{
    std::vector<std::string> filePaths;

    std::map<std::string, SamplerEntry>::const_iterator it =
        m_textureSamplers.find(samplerName);

    if (it != m_textureSamplers.end())
    {
        filePaths = it->second.filePaths;
    }

    return filePaths;
}

void MaterialResource::AddTextureSamplerFilePath(
    const std::string &samplerName, const std::string &textureResourceFilePath)
{
    std::map<std::string, SamplerEntry>::iterator it =
        m_textureSamplers.find(samplerName);

    if (it != m_textureSamplers.end())
    {
        it->second.filePaths.push_back(textureResourceFilePath);
    }
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
