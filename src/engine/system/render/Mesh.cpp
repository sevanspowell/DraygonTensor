#include "engine/system/render/Mesh.h"

namespace ds_render
{
Mesh::Mesh()
{
    m_vertexBuffer = VertexBufferHandle();
    m_indexBuffer = IndexBufferHandle();
    m_startingIndex = 0;
    m_numIndices = 0;
}

Mesh::Mesh(VertexBufferHandle vertexBuffer,
     IndexBufferHandle indexBuffer,
     size_t startingIndex,
     size_t numIndices)
{
    m_vertexBuffer = vertexBuffer;
    m_indexBuffer = indexBuffer;
    m_startingIndex = startingIndex;
    m_numIndices = numIndices;
}

VertexBufferHandle Mesh::GetVertexBuffer() const
{
    return m_vertexBuffer;
}

void Mesh::SetVertexBuffer(VertexBufferHandle vertexBuffer)
{
    m_vertexBuffer = vertexBuffer;
}

IndexBufferHandle Mesh::GetIndexBuffer() const
{
    return m_indexBuffer;
}

void Mesh::SetIndexBuffer(IndexBufferHandle indexBuffer)
{
    m_indexBuffer = indexBuffer;
}

size_t Mesh::GetStartingIndex() const
{
    return m_startingIndex;
}

void Mesh::SetStartingIndex(size_t startingIndex)
{
    m_startingIndex = startingIndex;
}

size_t Mesh::GetNumIndices() const
{
    return m_numIndices;
}

void Mesh::SetNumIndices(size_t numIndices)
{
    m_numIndices = numIndices;
}
}
