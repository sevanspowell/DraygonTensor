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
 *
 * @author Samuel Evans-Powell
 *
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
     * @param   textureResourceHandle  TextureResourceHandle, texture resource
     * handle to get associated texture resource for.
     * @return                         const TextureResource *, pointer to
     * texture resource associated with handle or nullptr if handle is invalid.
     */
    const TextureResource *
    GetTextureResource(TextureResourceHandle textureResourceHandle) const;

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
