#include "engine/system/render/VertexBufferDescription.h"

namespace ds_render
{
void VertexBufferDescription::AddAttributeDescription(
    const VertexBufferDescription::AttributeDescription &attributeDescriptor)
{
    m_attributes.push_back(attributeDescriptor);
}

const std::vector<VertexBufferDescription::AttributeDescription> &
VertexBufferDescription::GetAttributeDescriptions() const
{
    return m_attributes;
}
}
