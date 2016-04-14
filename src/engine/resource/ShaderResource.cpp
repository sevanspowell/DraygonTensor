#include "engine/resource/ShaderResource.h"

namespace ds
{
std::unique_ptr<IResource> ShaderResource::CreateFromFile(std::fstream &fileIn)
{
    std::unique_ptr<IResource> shaderResource(nullptr);

    // TODO: Parse file

    // TODO: Create mesh resource

    // Return resource (if any)
    return shaderResource;
}

// TODO: other methods (e.g. GetVertices...)
}
