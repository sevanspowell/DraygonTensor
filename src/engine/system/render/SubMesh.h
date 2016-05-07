#pragma once

namespace ds_render
{
/**
 * A sub mesh is an index into part of a vertex buffer, defining a seperate
 * object to render from that vertex buffer.
 */
struct SubMesh
{
    /**
     * Default constructor, create empty submesh.
     */
    SubMesh()
    {
        startingIndex = 0;
        numIndices = 0;
    }

    /**
     * Create a submesh begining at the startingIndex and using numIndices to
     * draw.
     *
     * @param  startingIndex  size_t, starting index to begin drawing from.
     * @param  numIndices     size_t, number of indices to use to draw.
     */
    SubMesh(size_t startingIndex, size_t numIndices)
    {
        this->startingIndex = startingIndex;
        this->numIndices = numIndices;
    }

    size_t startingIndex;
    size_t numIndices;
};
}
