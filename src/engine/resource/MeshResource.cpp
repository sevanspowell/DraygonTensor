#include "engine/resource/MeshResource.h"

namespace ds
{
std::unique_ptr<IResource> MeshResource::CreateFromFile(std::fstream &fileIn)
{
    std::unique_ptr<IResource> meshResource(nullptr);

    // TODO: Parse file

    // TODO: Create mesh resource

    // Return resource (if any)
    return meshResource;
}

// TODO: other methods (e.g. GetVertices...)
}
