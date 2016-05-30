#include "engine/resource/MaterialResourceManager.h"
#include "engine/common/HandleCommon.h"

namespace ds
{
bool MaterialResourceManager::LoadMaterialResourceFromFile(
    const std::string &filePath, MaterialResourceHandle *materialResourceHandle)
{
    bool result = false;

    // Have we already loaded a material from that filepath?
    std::vector<ManagedMaterialResource>::const_iterator it = std::find_if(
        m_materialResources.begin(), m_materialResources.end(),
        [&](const ManagedMaterialResource &managedMaterial)
        {
            return managedMaterial.materialResource.GetResourceFilePath() ==
                   filePath;
        });

    // If so, return it
    if (it != m_materialResources.end())
    {
        *materialResourceHandle = it->handle;
        result = true;
    }
    // If not, load it
    else
    {
        // Load material resource from file
        std::unique_ptr<IResource> resource =
            MaterialResource::CreateFromFile(filePath);

        // If loaded successfully
        if (resource != nullptr)
        {
            std::unique_ptr<MaterialResource> materialResource =
                std::unique_ptr<MaterialResource>(
                    static_cast<MaterialResource *>(resource.release()));

            // If cast successfully
            if (materialResource != nullptr)
            {
                // Begin construction of managed material resource
                ManagedMaterialResource managedMaterialResource;
                managedMaterialResource.materialResource =
                    *(materialResource.release());

                // Add material resource to list
                m_materialResources.push_back(managedMaterialResource);
                // Location in vector of material resource we just added
                size_t loc = m_materialResources.size() - 1;

                // Add material resource to handle manager
                Handle handle = m_handleManager.Add(
                    (void *)&m_materialResources[loc],
                    (uint32_t)HandleTypes::MaterialResourceHandleType);
                // Store handle with material resource
                m_materialResources[loc].handle = handle;

                // Because pushing adding an element to the vector might cause
                // the
                // vector to realloc, update the address of all managed material
                // resource objects
                for (unsigned int i = 0; i < m_materialResources.size(); ++i)
                {
                    // Get handle
                    ds::Handle handle = m_materialResources[i].handle;
                    // Update with new memory address
                    m_handleManager.Update(handle, &m_materialResources[i]);
                }

                result = true;
                // If successful, return material resource handle to caller
                *materialResourceHandle = handle;
            }
        }
    }

    return result;
}

const MaterialResource *MaterialResourceManager::GetMaterialResource(
    MaterialResourceHandle materialResourceHandle) const
{
    const MaterialResource *materialResource = nullptr;

    // Get managed material resource
    ManagedMaterialResource *managedMaterialResource = nullptr;
    bool result = m_handleManager.Get(materialResourceHandle,
                                      (void **)&managedMaterialResource);

    // If successful
    if (result == true)
    {
        // Get material resource from managed material resource
        materialResource = &managedMaterialResource->materialResource;
    }

    return materialResource;
}
}
