#include "engine/resource/MaterialResource.h"

namespace ds
{
std::unique_ptr<IResource> MaterialResource::CreateFromFile(std::fstream &fileIn)
{
    std::unique_ptr<IResource> materialResource(nullptr);

    // TODO: Parse file

    // TODO: Create mesh resource

    // Return resource (if any)
    return materialResource;
}

// TODO: other methods (e.g. GetVertices...)
}
