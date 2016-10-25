#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include "engine/Config.h"
#include "engine/common/Common.h"
#include "engine/json/Json.h"
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
    std::ifstream materialFile(filePath, std::ios::binary | std::ios::ate);
    bool didLoad = materialFile.good();

    // Read file into buffer
    std::vector<char> materialFileBuffer;
    if (didLoad)
    {
        // Reserve memory
        std::streamsize size = materialFile.tellg();
        materialFileBuffer.resize(size);

        // Read file
        materialFile.seekg(0, materialFile.beg);
        if (materialFile.read(&materialFileBuffer[0], size))
        {
            didLoad = true;
        }
        else
        {
            didLoad = false;
            materialFileBuffer.resize(0);
        }
    }
    // Terminate string
    materialFileBuffer.push_back('\0');

    if (didLoad)
    {
        // Create empty material resource
        materialResource = std::unique_ptr<IResource>(new MaterialResource());
        materialResource->SetResourceFilePath(filePath);

        // Get containing folder of this material resource
        std::string folder = ds_com::GetParentDirectory(filePath);

        JsonObject root;
        json::parseObject(&materialFileBuffer[0], &root);

        if (root["shader"] != nullptr)
        {
            // Get path to shader to use
            std::string relShaderPath;
            json::parseString(root["shader"], &relShaderPath);

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

        if (root["textures"] != nullptr)
        {
            JsonArray textures;
            json::parseArray(root["textures"], &textures);

            for (unsigned int iTex = 0; iTex < textures.size(); ++iTex)
            {
                JsonObject texture;
                json::parseObject(textures[iTex], &texture);

                if (texture["name"] != nullptr && texture["texture"] != nullptr)
                {
                    // Get name of texture in shader
                    std::string texName;
                    // Get path to texture resource file
                    std::string textureResourcePath;

                    json::parseString(texture["name"], &texName);
                    json::parseString(texture["texture"], &textureResourcePath);

                    std::stringstream fullTextureResourcePath;
                    fullTextureResourcePath << folder << textureResourcePath;

                    // Create new texture sampler
                    static_cast<MaterialResource *>(materialResource.get())
                        ->AddTextureSampler(texName,
                                            fullTextureResourcePath.str());
                }
                else
                {
                    std::cerr
                        << "MaterialResource::CreateFromFile: could not find "
                           "'name' or 'texture' field for array element "
                        << iTex << " in 'textures' resource: " << filePath << std::endl;
                }
            }
        }
        else
        {
            std::cerr << "MaterialResource::CreateFromFile: could not find "
                         "'textures' field in material resource: "
                      << filePath << std::endl;
        }

        if (root["parameters"] != nullptr)
        {
            JsonArray parameters;
            json::parseArray(root["parameters"], &parameters);

            for (unsigned int iParam = 0; iParam < parameters.size(); ++iParam)
            {
                JsonObject parameter;
                json::parseObject(parameters[iParam], &parameter);

                if (parameter["name"] != nullptr &&
                    parameter["type"] != nullptr &&
                    parameter["value"] != nullptr)
                {
                    std::string paramName;
                    std::string paramType;
                    json::parseString(parameter["name"], &paramName);
                    json::parseString(parameter["type"], &paramType);

                    ds_render::ShaderParameter param;

                    // Did the parameter load correctly?
                    bool loadedCorrectly = false;

                    // Setup parameter name
                    param.SetName(paramName);

                    if (paramType == "float")
                    {
                        // Set appropriate type
                        param.SetDataType(ds_render::ShaderParameter::
                                              ShaderParameterType::Float);

                        float paramValue;
                        paramValue = json::parseFloat(parameter["value"]);

                        param.SetData(sizeof(float), &paramValue);
                        loadedCorrectly = true;
                    }
                    else if (paramType == "int")
                    {
                        // Set appropriate type
                        param.SetDataType(ds_render::ShaderParameter::
                                              ShaderParameterType::Int);

                        int paramValue;
                        paramValue = json::parseInt(parameter["value"]);
                        param.SetData(sizeof(int), &paramValue);
                        loadedCorrectly = true;
                    }
                    else if (paramType == "vec3")
                    {
                        // Set appropriate type
                        param.SetDataType(ds_render::ShaderParameter::
                                              ShaderParameterType::Vector3);

                        // Get parameter data
                        JsonArray vector;
                        json::parseArray(parameter["value"], &vector);

                        if (vector.size() == 3)
                        {
                            std::vector<float> paramValue;
                            paramValue.push_back(json::parseFloat(vector[0]));
                            paramValue.push_back(json::parseFloat(vector[1]));
                            paramValue.push_back(json::parseFloat(vector[2]));

                            param.SetData(sizeof(float) * 3, &paramValue[0]);
                            loadedCorrectly = true;
                            std::cout << param.GetName()
                                      << " is a vec3 with value: "
                                      << *((ds_math::Vector3 *)param.GetData())
                                      << std::endl;
                        }
                        else
                        {
                            std::cerr << "MaterialResource::CreateFromFile: "
                                         "vec3 parameteter"
                                         "'"
                                      << paramName << "' must be of size 3! "
                                      << "In material resource: " << filePath
                                      << std::endl;
                        }
                    }
                    else if (paramType == "vec4")
                    {
                        // Set appropriate type
                        param.SetDataType(ds_render::ShaderParameter::
                                              ShaderParameterType::Vector4);

                        // Get parameter data
                        JsonArray vector;
                        json::parseArray(parameter["value"], &vector);

                        if (vector.size() == 4)
                        {
                            std::vector<float> paramValue;
                            paramValue.push_back(json::parseFloat(vector[0]));
                            paramValue.push_back(json::parseFloat(vector[1]));
                            paramValue.push_back(json::parseFloat(vector[2]));
                            paramValue.push_back(json::parseFloat(vector[3]));

                            param.SetData(sizeof(float) * 4, &paramValue[0]);
                            loadedCorrectly = true;
                            std::cout << param.GetName()
                                      << " is a vec4 with value: "
                                      << *((ds_math::Vector4 *)param.GetData())
                                      << std::endl;
                        }
                        else
                        {
                            std::cerr << "MaterialResource::CreateFromFile: "
                                         "vec4 parameteter"
                                         "'"
                                      << paramName << "' must be of size 4! "
                                      << "In material resource: " << filePath
                                      << std::endl;
                        }
                    }
                    else if (paramType == "mat4")
                    {
                        // Set appropriate type
                        param.SetDataType(ds_render::ShaderParameter::
                                              ShaderParameterType::Matrix4);

                        // Get parameter data
                        JsonArray matrix;
                        json::parseArray(parameter["value"], &matrix);

                        if (matrix.size() == 16)
                        {
                            std::vector<float> mat4;
                            for (int mElem = 0; mElem < 16; ++mElem)
                            {
                                mat4.push_back(json::parseFloat(matrix[mElem]));
                            }

                            param.SetData(sizeof(float) * 16, &mat4[0]);
                            loadedCorrectly = true;
                            std::cout << param.GetName()
                                      << " is a mat4 with value: "
                                      << *((ds_math::Matrix4 *)param.GetData())
                                      << std::endl;
                        }
                        else
                        {
                            std::cerr
                                << "MaterialResource::CreateFromFile: mat4 "
                                   "'"
                                << paramName << "' must be of size 16! "
                                << "In material resource: " << filePath
                                << std::endl;
                        }
                    }

                    if (loadedCorrectly)
                    {
                        // Add parameter to material resource
                        static_cast<MaterialResource *>(materialResource.get())
                            ->AddMaterialParameter(param);
                    }
                    else
                    {
                        std::cerr << "Material parameter '" << paramName
                                  << "' failed to load from file correctly."
                                  << std::endl;
                    }
                }
                else
                {
                    std::cerr
                        << "MaterialResource::CreateFromFile: shader parameter "
                           "in 'parameters' field of: "
                        << filePath
                        << " is missing either 'name', 'type' or 'value' field"
                        << std::endl;
                }
            }
        }
        else
        {
            std::cerr << "MaterialResource::CreateFromFile: could not find "
                         "'parameters' field in material resource: "
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
//         (m_uniforms.begin(), m_uniforms.end(),
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
