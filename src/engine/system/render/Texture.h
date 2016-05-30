#pragma once

#include "engine/resource/TextureResourceManager.h"
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
     * @param  textureResourceHandle  TextureResourceHandle, handle to texture
     * resource used to create this texture.
     * @param  textureType            TextureType, type of sampler that should
     * be used to sample from this texture.
     * @param  renderTextureHandle    RenderTextureHandle, handle to renderer
     * texture.
     */
    Texture(ds::TextureResourceHandle textureResourceHandle,
            TextureType textureType,
            RenderTextureHandle renderTextureHandle);

    /** Handle to texture resource used to create this texture */
    ds::TextureResourceHandle textureResourceHandle;
    /** Texture sampler type */
    TextureType textureType;
    /** Handle to renderer texture */
    RenderTextureHandle renderTextureHandle;
};
}
