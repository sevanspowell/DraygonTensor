#pragma once

#include "engine/entity/ComponentManager.h"
#include "engine/system/render/CameraComponent.h"

namespace ds_render
{
/**
 * Manages camera components.
 */
class CameraComponentManager : public ds::ComponentManager<CameraComponent>
{
public:
    /**
     * Get the projection matrix of the given camera component instance.
     *
     * @param   i  ds::Instance, component instance to get projection matrix of.
     * @return     const ds_math::Matrix4 &, material of given component
     * instance.
     */
    const ds_math::Matrix4 &GetProjectionMatrix(ds::Instance i) const;

    /**
     * Set the projection matrix of the given component instance.
     *
     * @param  i                      ds::Instance, component instance to set
     * projection matrix of.
     * @param  projectionMatrix       const Material &, material to set.
     */
    void SetProjectionMatrix(ds::Instance i,
                             const ds_math::Matrix4 &projectionMatrix);

    /**
     * Get the orientation of the given camera component instance.
     *
     * @param   i  ds::Instance, component instance to get orientation of.
     * @return     const ds_math::Quaternion &, orientation.
     */
    const ds_math::Quaternion &GetOrientation(ds::Instance i) const;

    /**
     * Set the orientation of the given camera component instance.
     *
     * @param   i  ds::Instance, component instance to get orientation of.
     * @param      const ds_math::Quaternion &, new orientation.
     */
    void SetOrientation(ds::Instance i, const ds_math::Quaternion &orientation);

private:
};
}
