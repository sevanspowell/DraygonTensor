#include "engine/resource/TextureResourceManager.h"
#include "engine/common/HandleCommon.h"

namespace ds
{
bool TextureResourceManager::LoadTextureResourceFromFile(
    const std::string &filePath, TextureResourceHandle *textureResourceHandle)
{
    bool result = false;

    // Load texture resource from file
    std::unique_ptr<IResource> resource =
        TextureResource::CreateFromFile(filePath);

    // If loaded successfully
    if (resource != nullptr)
    {
        std::unique_ptr<TextureResource> textureResource =
            std::unique_ptr<TextureResource>(
                static_cast<TextureResource *>(resource.release()));

        // If cast successfully
        if (textureResource != nullptr)
        {
            // Begin construction of managed texture resource
            ManagedTextureResource managedTextureResource;
            managedTextureResource.textureResource =
                *(textureResource.release());

            // Add texture resource to list
            m_textureResources.push_back(managedTextureResource);
            // Location in vector of texture resource we just added
            size_t loc = m_textureResources.size() - 1;

            // Add texture resource to handle manager
            Handle handle = m_handleManager.Add(
                (void *)&m_textureResources[loc],
                (uint32_t)HandleTypes::TextureResourceHandleType);
            // Store handle with texture resource
            m_textureResources[loc].handle = handle;

            // Because pushing adding an element to the vector might cause the
            // vector to realloc, update the address of all managed texture
            // resource
            // objects
            for (unsigned int i = 0; i < m_textureResources.size(); ++i)
            {
                // Get handle
                ds::Handle handle = m_textureResources[i].handle;
                // Update with new memory address
                m_handleManager.Update(handle, &m_textureResources[i]);
            }

            result = true;
            // If successful, return texture resource handle to caller
            *textureResourceHandle = handle;
        }
    }

    return result;
}

bool TextureResourceManager::GetTextureResource(
    TextureResourceHandle textureResourceHandle,
    TextureResource **textureResource)
{
    bool result = false;

    if (textureResource != nullptr)
    {
        // Get managed texture resource
        ManagedTextureResource *managedTextureResource = nullptr;
        result = m_handleManager.Get(textureResourceHandle,
                                     (void **)&managedTextureResource);

        // If fail
        if (result == false)
        {
            // Clear provided memory
            *textureResource = nullptr;
        }
        // Else
        else
        {
            // Get texture resource from managed texture resource
            *textureResource = &(managedTextureResource->textureResource);
        }
    }

    return result;
}

void TextureResourceManager::ReloadAll()
{
}
}
