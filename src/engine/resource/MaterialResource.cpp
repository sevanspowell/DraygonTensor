#include <cassert>
#include <iostream>
#include <sstream>

#include "engine/Config.h"
#include "engine/common/Common.h"
#include "engine/resource/MaterialResource.h"
#include "math/Matrix4.h"
#include "math/Vector4.h"

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
        }
        else
        {
            std::cerr << "MaterialResource::CreateFromFile: could not find "
                         "'shader' field in material resource: "
                      << filePath << std::endl;
        }

        // Get path to texture samplers to use
        std::vector<std::string> textureKeys = config.GetObjectKeys("textures");

        // For each texture sampler, grab texture sampler details and add it
        // to the material resource
        for (auto key : textureKeys)
        {
            // Get path to sampler
            std::stringstream samplerKey;
            samplerKey << "textures"
                       << "." << key;

            // Key to texture field in config file
            std::stringstream textureFieldKey;
            textureFieldKey << samplerKey.str() << "."
                            << "texture";

            // Get path to texture resource file
            std::string textureResourcePath;

            if (config.GetString(textureFieldKey.str(), &textureResourcePath))
            {
                std::stringstream fullTextureResourcePath;
                fullTextureResourcePath << folder << textureResourcePath;

                // Create new texture sampler
                static_cast<MaterialResource *>(materialResource.get())
                    ->AddTextureSampler(key, fullTextureResourcePath.str());
            }
        }

        // Get and load shader parameters
        std::vector<std::string> shaderParameters =
            config.GetObjectKeys("parameters");

        // For each parameter
        for (auto parameterName : shaderParameters)
        {
            ds_render::ShaderParameter parameter;

            // Did the parameter load correctly?
            bool loadedCorrectly = false;

            // Setup parameter name
            parameter.SetName(parameterName);

            std::stringstream parameterKey;
            parameterKey << "parameters"
                         << "." << parameterName;

            // Get parameter data type -- should only be one key
            std::vector<std::string> dataType =
                config.GetObjectKeys(parameterKey.str());
            if (dataType.size() > 0)
            {
                // Type of the shader parameter: float, int, mat4, vec3,
                // vec4
                std::string type = dataType[0];

                // Get key to data
                std::stringstream dataKey;
                dataKey << parameterKey.str() << "." << type;

                if (type == "float")
                {
                    // Set appropriate type
                    parameter.SetDataType(
                        ds_render::ShaderParameter::ShaderParameterType::Float);

                    // Get parameter data
                    float data;
                    if (config.GetFloat(dataKey.str(), &data))
                    {
                        parameter.SetData(sizeof(float), &data);
                        loadedCorrectly = true;
                    }
                }
                else if (type == "int")
                {
                    // Set appropriate type
                    parameter.SetDataType(
                        ds_render::ShaderParameter::ShaderParameterType::Int);

                    // Get parameter data
                    int data;
                    if (config.GetInt(dataKey.str(), &data))
                    {
                        parameter.SetData(sizeof(int), &data);
                        loadedCorrectly = true;
                    }
                }
                else if (type == "vec3")
                {
                    // Set appropriate type
                    parameter.SetDataType(ds_render::ShaderParameter::
                                              ShaderParameterType::Vector3);

                    // Get parameter data
                    std::vector<float> vec3;
                    config.GetFloatArray(dataKey.str(), &vec3);

                    if (vec3.size() == 3)
                    {
                        parameter.SetData(sizeof(float) * 3, &vec3[0]);
                        loadedCorrectly = true;
                        std::cout << parameter.GetName()
                                  << " is a vec3 with value: "
                                  << *((ds_math::Vector3 *)parameter.GetData())
                                  << std::endl;
                    }
                    else
                    {
                        std::cerr << "MaterialResource::CreateFromFile: vec3 "
                                     "'"
                                  << dataKey.str() << "' must be of size 3! "
                                  << "In material resource: " << filePath
                                  << std::endl;
                    }
                }
                else if (type == "vec4")
                {
                    // Set appropriate type
                    parameter.SetDataType(ds_render::ShaderParameter::
                                              ShaderParameterType::Vector4);

                    // Get parameter data
                    std::vector<float> vec4;
                    config.GetFloatArray(dataKey.str(), &vec4);

                    if (vec4.size() == 4)
                    {
                        parameter.SetData(sizeof(float) * 4, &vec4[0]);
                        loadedCorrectly = true;
                        std::cout << parameter.GetName()
                                  << " is a vec4 with value: "
                                  << *((ds_math::Vector4 *)parameter.GetData())
                                  << std::endl;
                    }
                    else
                    {
                        std::cerr << "MaterialResource::CreateFromFile: vec4 "
                                     "'"
                                  << dataKey.str() << "' must be of size 4! "
                                  << "In material resource: " << filePath
                                  << std::endl;
                    }
                }
                else if (type == "mat4")
                {
                    // Set appropriate type
                    parameter.SetDataType(ds_render::ShaderParameter::
                                              ShaderParameterType::Matrix4);

                    // Get parameter data
                    std::vector<float> mat4;
                    config.GetFloatArray(dataKey.str(), &mat4);

                    if (mat4.size() == 16)
                    {
                        parameter.SetData(sizeof(float) * 16, &mat4[0]);
                        loadedCorrectly = true;
                        std::cout << parameter.GetName()
                                  << " is a mat4 with value: "
                                  << *((ds_math::Matrix4 *)parameter.GetData())
                                  << std::endl;
                    }
                    else
                    {
                        std::cerr << "MaterialResource::CreateFromFile: mat4 "
                                     "'"
                                  << dataKey.str() << "' must be of size 16! "
                                  << "In material resource: " << filePath
                                  << std::endl;
                    }
                }

                if (loadedCorrectly)
                {
                    // Add parameter to material resource
                    static_cast<MaterialResource *>(materialResource.get())
                        ->AddMaterialParameter(parameter);
                }
                else
                {
                    std::cerr << "Material parameter '" << parameterName
                              << "' failed to load from file correctly."
                              << std::endl;
                }
            }
            else
            {
                std::cout << "Material parameter '" << parameterName
                          << "' in file: " << filePath << " needs a type."
                          << std::endl;
            }
        }
    }
    else
    {
        std::cerr
            << "MaterialResource::CreateFromFile: could not open material "
               "resource file (json file): "
            << filePath << std::endl;
    }

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

void MaterialResource::AddTextureSampler(
    const std::string &samplerName, const std::string &textureResourceFilePath)
{
    SamplerEntry entry;
    entry.textureResourceFilePath = textureResourceFilePath;

    m_textureSamplers[samplerName] = entry;
}

std::string MaterialResource::GetSamplerTextureResourceFilePath(
    const std::string &samplerName) const
{
    std::string filePath = "";

    std::map<std::string, SamplerEntry>::const_iterator it =
        m_textureSamplers.find(samplerName);

    if (it != m_textureSamplers.end())
    {
        filePath = it->second.textureResourceFilePath;
    }

    return filePath;
}

const std::vector<ds_render::ShaderParameter> &
MaterialResource::GetMaterialParameters() const
{
    return m_parameters;
}

void MaterialResource::AddMaterialParameter(
    const ds_render::ShaderParameter &parameter)
{
    m_parameters.push_back(parameter);
}

// void MaterialResource::AddUniformBlock(
//     const ds_render::UniformBlock &uniformBlock)
// {
//     m_uniformBlocks.push_back(uniformBlock);
// }

// const std::vector<ds_render::UniformBlock> &
// MaterialResource::GetUniformBlocks() const
// {
//     return m_uniformBlocks;
// }

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
