#include "engine/system/render/Material.h"

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
                          const Texture &texture)
{
    // Construct shader texture
    ShaderTexture shaderTexture;
    shaderTexture.samplerName = samplerName;
    shaderTexture.texture = texture;

    // Add it
    m_textures.push_back(shaderTexture);
}

const std::vector<Material::ShaderTexture> &Material::GetTextures()
{
    return m_textures;
}

void Material::AddConstantBuffer(const std::string &constantBufferName,
                                 const ConstantBuffer &constantBuffer)
{
    m_constantBuffers.push_back(std::pair<std::string, ConstantBuffer>(
        constantBufferName, constantBuffer));
}
}
