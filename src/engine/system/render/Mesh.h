#pragma once

#include <cstddef>

#include "engine/system/render/RenderCommon.h"
#include "engine/system/render/SubMesh.h"

namespace ds_render
{
/**
 * The mesh class is an abstraction of the way a renderer represents a mesh
 * (using vertex and index buffer handles).
 */
class Mesh
{
public:
    /**
     * Mesh default constructor.
     */
    Mesh();

    /**
     * Mesh constructor.
     *
     * @param  vertexBuffer   VertexBufferHandle, vertex buffer of mesh.
     * @param  indexBuffer    IndexBufferHandle, index buffer of mesh.
     */
    Mesh(VertexBufferHandle vertexBuffer,
         IndexBufferHandle indexBuffer,
         size_t startingIndex,
         size_t numIndices);

    /**
     * Get handle to the vertex buffer of the mesh.
     *
     * @return  VertexBufferHandle, vertex buffer of the mesh.
     */
    VertexBufferHandle GetVertexBuffer() const;

    /**
     * Set the vertex buffer of the mesh.
     *
     * @param  vertexBuffer  VertexBufferHandle, handle to vertex buffer of the
     * mesh.
     */
    void SetVertexBuffer(VertexBufferHandle vertexBuffer);

    /**
     * Get handle to the index buffer of the mesh.
     *
     * @return  IndexBufferHandle, index buffer of the mesh.
     */
    IndexBufferHandle GetIndexBuffer() const;

    /**
     * Set the index buffer of the mesh.
     *
     * @param  indexBuffer  IndexBufferHandle, handle to index buffer of the
     * mesh.
     */
    void SetIndexBuffer(IndexBufferHandle indexBuffer);

    /**
     * Get where (the position) in the mesh index buffer to begin drawing from.
     *
     * @return  size_t, index in index buffer to begin drawing from.
     */
    size_t GetStartingIndex() const;

    /**
     * Set where (the position) in the mesh index buffer to begin drawing from.
     *
     * @param  startingIndex  size_t, index in the index buffer to begin drawing
     * from.
     */
    void SetStartingIndex(size_t startingIndex);

    /**
     * Get the number of indices in the mesh index buffer to draw.
     *
     * @return  size_t, number of indices in the mesh index buffer to draw.
     */
    size_t GetNumIndices() const;

    /**
     * Set the number of indices in the mesh index buffer to draw.
     *
     * @param  numIndices  size_t, number of indices in the mesh index buffer to
     * draw.
     */
    void SetNumIndices(size_t numIndices);

private:
    /** Vertex buffer of mesh */
    VertexBufferHandle m_vertexBuffer;
    /** Index buffer of mesh */
    IndexBufferHandle m_indexBuffer;
    // /** Sub meshes of mesh */
    // std::vector<SubMesh> m_subMeshes;
    size_t m_startingIndex;
    size_t m_numIndices;
};
}
