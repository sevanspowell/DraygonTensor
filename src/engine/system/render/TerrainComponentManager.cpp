#include <cassert>

#include "engine/system/render/TerrainComponentManager.h"

namespace ds_render
{
TerrainResourceHandle
TerrainComponentManager::GetTerrainResourceHandle(ds::Instance i) const
{
    assert(i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
           "TerrainComponentManager::GetTerrainResourceHandle: tried to get "
           "invalid instance");

    return m_data.component[i.index].terrainResourceHandle;
}

void TerrainComponentManager::SetTerrainResourceHandle(
    ds::Instance i, TerrainResourceHandle terrainResourceHandle)
{
    assert(i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
           "TerrainComponentManager::SetTerrainResourceHandle: tried to set "
           "invalid instance");

    m_data.component[i.index].terrainResourceHandle = terrainResourceHandle;
}
}
