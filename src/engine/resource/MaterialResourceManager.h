#pragma once

#include <vector>

#include "engine/common/HandleManager.h"
#include "engine/resource/MaterialResource.h"

namespace ds
{
/** Handle to material resource */
typedef Handle MaterialResourceHandle;

/**
 * The material resource manager is responsible for controlling the creation and
 * access of material resources.
 */
class MaterialResourceManager
{
public:
    /**
     * Get a handle to the material resource loaded from the given file path.
     * Returns FALSE if material resource failed to load and memory at provided
     * address is left untouched. Returns handle to already loaded material
     * resource if material resource already loaded.
     *
     * @post  materialResourceHandle memory left untouched if method returns
     * FALSE.
     *
     * @param   filePath                const std::string &, path to material
     * resource file to load.
     * @param   materialResourceHandle  MaterialResourceHandle *, address to
     * place MaterialResourceHandle if material resource successfully loaded.
     * @return  bool, TRUE if material resource successfully loaded, FALSE
     * otherwise.
     */
    bool LoadMaterialResourceFromFile(
        const std::string &filePath,
        MaterialResourceHandle *materialResourceHandle);

    /**
     * Get the material resource associated with the given material resource
     * handle.
     *
     * @param   materialResourceHandle  MaterialResourceHandle, material
     * resource
     * handle to get associated material resource for.
     * @return  const MaterialResource *, pointer to material resource
     * associated with handle or nullptr if handle is invalid.
     */
    const MaterialResource *
    GetMaterialResource(MaterialResourceHandle materialResourceHandle) const;

private:
    /** Store handle with managed material resource object for update purposes.
     */
    struct ManagedMaterialResource
    {
        MaterialResourceHandle handle;
        MaterialResource materialResource;
    };

    /** Material resource storage */
    std::vector<ManagedMaterialResource> m_materialResources;
    /** Handle manager */
    HandleManager m_handleManager;
};
}
