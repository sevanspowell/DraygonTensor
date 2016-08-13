#include "engine/system/render/Skybox.h"

namespace ds_render
{
Mesh Skybox::GetMesh() const
{
    return m_mesh;
}

void Skybox::SetMesh(Mesh mesh)
{
    m_mesh = mesh;
}
}
