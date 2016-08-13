#include "engine/system/render/Material.h"
#include "math/Matrix4.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

namespace ds_render
{
ProgramHandle Material::GetProgram() const
{
    return m_program;
}

void Material::SetProgram(ProgramHandle program)
{
    m_program = program;
}

void Material::AddTexture(const std::string &samplerName,
                          TextureHandle textureHandle)
{
    // Construct shader texture
    ShaderTexture shaderTexture;
    shaderTexture.samplerName = samplerName;
    shaderTexture.textureHandle = textureHandle;

    // Add it
    m_textures.push_back(shaderTexture);
}

const std::vector<Material::ShaderTexture> &Material::GetTextures() const
{
    return m_textures;
}

const std::vector<ShaderParameter> &Material::GetMaterialParameters() const
{
    return m_parameters;
}

void Material::AddMaterialParameter(const ShaderParameter &shaderParameter)
{
    m_parameters.push_back(shaderParameter);
}

void Material::SetMaterialParameterData(
    const std::string &parameterName,
    ShaderParameter::ShaderParameterType parameterType,
    const void *data)
{
    // Find parameter with name given
    std::vector<ShaderParameter>::iterator it =
        std::find_if(m_parameters.begin(), m_parameters.end(),
                     [&](const ShaderParameter &parameter)
                     {
                         return parameter.GetName() == parameterName;
                     });

    // If found, check material parameter type
    if (it != m_parameters.end())
    {
        // If material parameter type is correct
        if (it->GetDataType() == parameterType)
        {
            switch (it->GetDataType())
            {
            case ShaderParameter::ShaderParameterType::Float:
                it->SetData(sizeof(float), data);
                break;
            case ShaderParameter::ShaderParameterType::Int:
                it->SetData(sizeof(int), data);
                break;
            case ShaderParameter::ShaderParameterType::Matrix4:
                it->SetData(sizeof(ds_math::Matrix4), data);
                break;
            case ShaderParameter::ShaderParameterType::Vector3:
                it->SetData(sizeof(ds_math::Vector3), data);
                break;
            case ShaderParameter::ShaderParameterType::Vector4:
                it->SetData(sizeof(ds_math::Vector4), data);
                break;
            }
        }
        else
        {
            std::cerr << "Material::SetMaterialParameterData: parameter type "
                         "provided does not match material parameter type."
                      << std::endl;
        }
    }
}

void Material::AddConstantBuffer(const std::string &constantBufferName,
                                 const ConstantBuffer &constantBuffer)
{
    m_constantBuffers.push_back(std::pair<std::string, ConstantBuffer>(
        constantBufferName, constantBuffer));
}

ds::MaterialResourceHandle Material::GetMaterialResourceHandle() const
{
    return m_materialResourceHandle;
}

void Material::SetMaterialResourceHandle(
    ds::MaterialResourceHandle materialResourceHandle)
{
    m_materialResourceHandle = materialResourceHandle;
}
}
