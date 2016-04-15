#include "engine/resource/MaterialResource.h"

namespace ds
{
std::unique_ptr<IResource> MaterialResource::CreateFromFile(std::string filePath)
{
    std::unique_ptr<IResource> materialResource(nullptr);

    // TODO: Parse file

    // TODO: Create mesh resource

    // Return resource (if any)
    return materialResource;
}

const std::string &MaterialResource::GetResourceFilePath() const
{
    return m_filePath;
}

void MaterialResource::SetResourceFilePath(const std::string &filePath)
{
    m_filePath = filePath;
}

// TODO: other methods (e.g. GetVertices...)
}
