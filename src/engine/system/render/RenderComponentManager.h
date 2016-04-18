#pragma once

#include "engine/entity/ComponentManager.h"
#include "engine/system/render/RenderComponent.h"

namespace ds_render
{
/**
 * Manages render components.
 */
class RenderComponentManager : public ds::ComponentManager<RenderComponent>
{
public:
    /**
     * Get the material of the given component instance.
     *
     * @param   i  ds::Instance, component instance to get material of.
     * @return     const Material &, material of given component instance.
     */
    const Material &GetMaterial(ds::Instance i) const;

    /**
     * Set the material of the given component instance.
     *
     * @param  i         ds::Instance, component instance to set material of.
     * @param  material  const Material &, material to set.
     */
    void SetMaterial(ds::Instance i, const Material &material);

    /**
     * Get the mesh of the given component instance.
     *
     * @param   i  ds::Instance, component instance to get mesh of.
     * @return     const Mesh &, mesh of given component instance.
     */
    const Mesh &GetMesh(ds::Instance i) const;

    /**
     * Set the mesh of the given component instance.
     *
     * @param   i  ds::Instance, component instance to set the mesh of.
     * @param      const Mesh &, mesh to set.
     */
    void SetMesh(ds::Instance i, const Mesh &mesh);

private:
};
}
