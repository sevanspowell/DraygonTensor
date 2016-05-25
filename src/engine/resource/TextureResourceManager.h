#pragma once

#include <vector>

#include "engine/common/HandleManager.h"
#include "engine/resource/TextureResource.h"

namespace ds
{
/** Handle to texture resource */
typedef Handle TextureResourceHandle;

/**
 * The texture resource manager is responsible for controlling the creation and
 * access of texture resources.
 */
class TextureResourceManager
{
public:
    /**
     * Get a handle to the texture resource loaded from the given file path.
     * Returns FALSE if texture resource failed to load and memory at given
     * address is left untouched.
     *
     * @post textureResourceHandle memory left untouched if method returns
     * FALSE.
     *
     * @param   filePath  const std::string &, path to texture resource file to
     * load.
     * @return            TextureResourceHandle, address to place
     * TextureResourceHandle loaded (if successfully loaded).
     */
    bool
    LoadTextureResourceFromFile(const std::string &filePath,
                                TextureResourceHandle *textureResourceHandle);

    /**
     * Get the texture resource associated with the given texture resource
     * handle.
     *
     * If no texture associated with given handle, will return FALSE and
     * memory at the address given will be set to nullptr.
     *
     * @param   textureResourceHandle  TextureResourceHandle, texture resource
     * handle to get associated texture resource for.
     * @param   texture        ds_render::Texture **, address in memory to
     * place Texture Resource pointer at.
     * @return                 bool, TRUE if texture resource for texture
     * resource handle found, FALSE otherwise.
     */
    bool GetTextureResource(TextureResourceHandle textureResourceHandle,
                            TextureResource **textureResource);

    /**
     * Reload all texture resources from file.
     */
    void ReloadAll();

private:
    /** Store handle with managed texture resource object for update purposes */
    struct ManagedTextureResource
    {
        TextureResourceHandle handle;
        TextureResource textureResource;
    };

    /** Texture resource storage */
    std::vector<ManagedTextureResource> m_textureResources;
    /** Handle manager */
    HandleManager m_handleManager;
};
}
