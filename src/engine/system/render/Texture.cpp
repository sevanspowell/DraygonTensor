#include "engine/system/render/Texture.h"

namespace ds_render
{
Texture::Texture()
{
    m_samplerType = SamplerType::None;
    m_texture = TextureHandle();
}

Texture::Texture(SamplerType samplerType, TextureHandle texture)
{
    m_samplerType = samplerType;
    m_texture = texture;
}

TextureHandle Texture::GetTextureHandle() const
{
    return m_texture;
}

void Texture::SetTextureHandle(TextureHandle textureHandle)
{
    m_texture = textureHandle;
}

SamplerType Texture::GetSamplerType() const
{
    return m_samplerType;
}

void Texture::SetSamplerType(SamplerType samplerType)
{
    m_samplerType = samplerType;
}
}
