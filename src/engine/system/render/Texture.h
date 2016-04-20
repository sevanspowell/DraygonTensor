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
     * @param  texture  TextureHandle, handle to renderer texture.
     */
    Texture(TextureHandle texture);

    /**
     * Get handle to renderer texture.
     *
     * @return  TextureHandle, handle to renderer texture.
     */
    TextureHandle GetTextureHandle() const;

    /**
     * Set the handle to renderer texture.
     *
     * @param  textureHandle  TextureHandle, handle to renderer texture.
     */
    void SetTextureHandle(TextureHandle textureHandle);

private:
    /** Handle to renderer texture */
    TextureHandle m_texture;
};
}
