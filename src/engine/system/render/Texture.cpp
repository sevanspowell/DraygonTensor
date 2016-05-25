#include "engine/system/render/Texture.h"

namespace ds_render
{
Texture::Texture()
{
    m_textureType = TextureType::None;
    m_texture = RenderTextureHandle();
}

Texture::Texture(TextureType textureType, RenderTextureHandle texture)
{
    m_textureType = textureType;
    m_texture = texture;
}

RenderTextureHandle Texture::GetRenderTextureHandle() const
{
    return m_texture;
}

void Texture::SetRenderTextureHandle(RenderTextureHandle textureHandle)
{
    m_texture = textureHandle;
}

TextureType Texture::GetTextureType() const
{
    return m_textureType;
}

void Texture::SetTextureType(TextureType textureType)
{
    m_textureType = textureType;
}
}
