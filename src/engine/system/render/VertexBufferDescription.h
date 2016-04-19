#pragma once

#include <vector>

#include "engine/system/render/RenderCommon.h"

namespace ds_render
{
/**
 * A vertex buffer description is used to describe the type, format and layout
 * of data in a vertex buffer (as vertex buffers are just some raw data).
 */
class VertexBufferDescription
{
public:
    /** Used to describe each attribute in a vertex buffer */
    struct AttributeDescription
    {
        /** Attribute kind: Position, Normal, TextureCoordinate */
        AttributeType attributeType;
        /** Data type of the attribute elements: float, int, etc. */
        RenderDataType attributeDataType;
        /** Number of elements per attribute. */
        uint32_t numElementsPerAttribute;
        /** Stride from one vertex datum to another in buffer (bytes) */
        uint32_t stride;
        /** Offset into each vertex datum this attribute is found (bytes) */
        uint32_t offset;
        /** Is this data normalized? */
        bool normalized;
    };


    /**
     * Add an attribute description to the vertex buffer description.
     *
     * Used to describe one particular attribute (Position, Normal,
     * TextureCoordinate);
     *
     * @param  attributeDescriptor  const AttributeDescription &, description of
     * one particular attribute in the vertex buffer.
     */
    void
    AddAttributeDescription(const AttributeDescription &attributeDescriptor);

    /**
     * Get a list of attribute descriptions in this vertex buffer.
     *
     * @return  const std::vector<AttributeDescription> &, list of attribute
     * descriptions in this vertex buffer.
     */
    const std::vector<AttributeDescription> &GetAttributeDescriptions() const;

private:
    /** List of attribute descriptions */
    std::vector<AttributeDescription> m_attributes;
};
}
