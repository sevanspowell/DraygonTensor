#include <cassert>

#include "engine/system/render/RenderComponentManager.h"

namespace ds_render
{
const Mesh &RenderComponentManager::GetMesh(ds::Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "RenderComponentManager::GetMesh: tried to get invalid instance");

    return m_data.component[i.index].mesh;
}

void RenderComponentManager::SetMesh(ds::Instance i, const Mesh &mesh)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "RenderComponentManager::SetMesh: tried to set invalid instance");

    m_data.component[i.index].mesh = mesh;
}
}
