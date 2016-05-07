#include <cassert>

#include "engine/system/render/Mesh.h"

namespace ds_render
{
Mesh::Mesh()
{
    m_vertexBuffer = VertexBufferHandle();
    m_indexBuffer = IndexBufferHandle();
}

Mesh::Mesh(VertexBufferHandle vertexBuffer, IndexBufferHandle indexBuffer)
{
    m_vertexBuffer = vertexBuffer;
    m_indexBuffer = indexBuffer;
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

// size_t Mesh::GetStartingIndex() const
// {
//     return m_startingIndex;
// }

// void Mesh::SetStartingIndex(size_t startingIndex)
// {
//     m_startingIndex = startingIndex;
// }

// size_t Mesh::GetNumIndices() const
// {
//     return m_numIndices;
// }

// void Mesh::SetNumIndices(size_t numIndices)
// {
//     m_numIndices = numIndices;
// }

void Mesh::AddSubMesh(const SubMesh &subMesh)
{
    m_subMeshes.push_back(subMesh);
}

size_t Mesh::GetNumSubMeshes() const
{
    return m_subMeshes.size();
}

const SubMesh &Mesh::GetSubMesh(size_t index) const
{
    assert(index >= 0 && index < m_subMeshes.size() &&
           "Mesh::GetSubMesh: tried to get submesh out of range.");

    return m_subMeshes[index];
}
}
