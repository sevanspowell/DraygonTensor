#pragma once

#include "engine/entity/ComponentManager.h"
#include "engine/system/render/TerrainComponent.h"

namespace ds_render
{
/**
 * Manages render components.
 */
class TerrainComponentManager : public ds::ComponentManager<TerrainComponent>
{
public:
    /**
     * Get the terrain resource handle associated with a given terrain instance.
     *
     * @param   i  ds::Instance, component instance to get terrain resource
     *             handle of.
     * @return     TerrainResourceHandle, handle to some terrain resource data.
     */
    TerrainResourceHandle GetTerrainResourceHandle(ds::Instance i) const;

    /**
     * Set the terrain resource handle of the given terrain instance.
     *
     * @param  i                      ds::Instance, component instance to set
     *                                terrain resource handle of.
     * @param  terrainResourceHandle  TerrainResourceHandle, handle to terrain
     *                                resource to associate with the given
     *                                component instance.
     */
    void SetTerrainResourceHandle(ds::Instance i,
                                  TerrainResourceHandle terrainResourceHandle);

private:
};
}
