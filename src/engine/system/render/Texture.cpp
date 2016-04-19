#include "engine/system/render/Texture.h"

namespace ds_render
{
Texture::Texture()
{
    m_texture = TextureHandle();
}

Texture::Texture(TextureHandle texture)
{
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
}
