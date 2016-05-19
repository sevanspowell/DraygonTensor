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
     * @param  samplerType  SamplerType, type of sampler that should be used to
     * sample from this texture.
     * @param  texture      TextureHandle, handle to renderer texture.
     */
    Texture(SamplerType samplerType, TextureHandle texture);

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

    /**
     * Get the type of sampler that should be used to sample from this texture.
     *
     * @return  SamplerType, type of sampler that should be used to sample from
     * this texture.
     */
    SamplerType GetSamplerType() const;

    /**
     * Set the type of sampler that should be used to sample from this texture.
     *
     * @param  samplerType  SamplerType, type of sampler that should be used to
     * sample from this texture.
     */
    void SetSamplerType(SamplerType samplerType);

private:
    /** Texture sampler type */
    SamplerType m_samplerType;
    /** Handle to renderer texture */
    TextureHandle m_texture;
};
}
