#include "engine/resource/TextureResource.h"

namespace ds
{
std::unique_ptr<IResource> TextureResource::CreateFromFile(std::fstream &fileIn)
{
    std::unique_ptr<IResource> textureResource(nullptr);

    // TODO: Parse file

    // TODO: Create mesh resource

    // Return resource (if any)
    return textureResource;
}

// TODO: other methods (e.g. GetVertices...)
}
