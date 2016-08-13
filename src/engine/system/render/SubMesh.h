#pragma once

#include "engine/system/render/Material.h"

namespace ds_render
{
/**
 * A sub mesh is an index into part of a vertex buffer, defining a seperate
 * object to render from that vertex buffer.
 *
 * Each submesh
 */
struct SubMesh
{
    /**
     * Default constructor, create empty submesh with an empty material.
     */
    SubMesh()
        : startingIndex(0), numIndices(0), materialHandle(MaterialHandle())
    {
    }

    /**
     * Create a submesh begining at the startingIndex and using numIndices to
     * draw as well as the given material.
     *
     * @param  startingIndex   size_t, starting index to begin drawing from.
     * @param  numIndices      size_t, number of indices to use to draw.
     * @param  materialHandle  MaterialHandle, handle to material to draw with.
     */
    SubMesh(size_t startingIndex,
            size_t numIndices,
            MaterialHandle materialHandle)
    {
        this->startingIndex = startingIndex;
        this->numIndices = numIndices;
        this->materialHandle = materialHandle;
    }

    size_t startingIndex;
    size_t numIndices;
    MaterialHandle materialHandle;
};
}
