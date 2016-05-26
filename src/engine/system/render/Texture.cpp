#include "engine/system/render/Texture.h"

namespace ds_render
{
Texture::Texture()
{
    this->textureResourceHandle = ds::TextureResourceHandle();
    this->textureType = TextureType::None;
    this->renderTextureHandle = RenderTextureHandle();
}

Texture::Texture(ds::TextureResourceHandle textureResourceHandle,
                 TextureType textureType,
                 RenderTextureHandle texture)
{
    this->textureResourceHandle = textureResourceHandle;
    this->textureType = textureType;
    this->renderTextureHandle = texture;
}
}
