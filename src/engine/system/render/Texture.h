#pragma once

#include "engine/system/render/RenderCommon.h"

namespace ds_render
{
/**
 * Encapsulation of the renderer's texture handle.
 */
class Texture
{
public:
    /**
     * Texture default contructor.
     */
    Texture();

    /**
     * Texture constructor.
     *
     * @param  textureType  TextureType, type of sampler that should be used to
     * sample from this texture.
     * @param  texture      RenderTextureHandle, handle to renderer texture.
     */
    Texture(TextureType textureType, RenderTextureHandle texture);

    /**
     * Get handle to renderer texture.
     *
     * @return  RenderTextureHandle, handle to renderer texture.
     */
    RenderTextureHandle GetRenderTextureHandle() const;

    /**
     * Set the handle to renderer texture.
     *
     * @param  textureHandle  RenderTextureHandle, handle to renderer texture.
     */
    void SetRenderTextureHandle(RenderTextureHandle textureHandle);

    /**
     * Get the type of sampler that should be used to sample from this texture.
     *
     * @return  TextureType, type of sampler that should be used to sample from
     * this texture.
     */
    TextureType GetTextureType() const;

    /**
     * Set the type of sampler that should be used to sample from this texture.
     *
     * @param  textureType  TextureType, type of sampler that should be used to
     * sample from this texture.
     */
    void SetTextureType(TextureType textureType);

private:
    /** Texture sampler type */
    TextureType m_textureType;
    /** Handle to renderer texture */
    RenderTextureHandle m_texture;
};
}
