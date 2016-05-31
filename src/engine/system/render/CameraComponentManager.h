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
     * Get the vertical field of view of the given camera component instance.
     *
     * @param   i  ds::Instance, camera component instance to get the vertical
     * field of view of.
     * @return     float, vertical field of view of camera component.
     */
    float GetVerticalFieldOfView(ds::Instance i) const;

    /**
     * Set the vertical field of view of the given camera component instance.
     *
     * @param  i    ds::Instance, camera component instance to set the vertical
     * field of view of.
     * @param  fov  float, value for vertical field of view of camera to set.
     */
    void SetVerticalFieldOfView(ds::Instance i, float fov);

    /**
     * Get the aspect ratio of the given camera component instance.
     *
     * @param   i  ds::Instance, camera component instance to get the aspect
     * ratio of.
     * @return     float, aspect ratio of camera component.
     */
    float GetAspectRatio(ds::Instance i) const;

    /**
     * Set the aspect ratio of the given camera component instance.
     *
     * @param  i            ds::Instance, camera component instance to set the
     * aspect ratio of.
     * @param  aspectRatio  float, value for aspect ratio of camera to set.
     */
    void SetAspectRatio(ds::Instance i, float aspectRatio);

    /**
     * Get the near clipping plane distance of the given camera component
     * instance.
     *
     * @param   i  ds::Instance, camera component instance to get the near
     * clipping plane distance of.
     * @param      float, value of given camera component's near clipping plane
     * distance.
     */
    float GetNearClippingPlane(ds::Instance i) const;

    /**
     * Set the near clipping plane distance of the given camera component
     * instance.
     *
     * @param  i     ds::Instance, camera component instance to set the near
     * clipping plane of.
     * @param  dist  float, value for near clipping plane distance of camera to
     * set.
     */
    void SetNearClippingPlane(ds::Instance i, float dist);

    /**
     * Get the far clipping plane distance of the given camera component
     * instance.
     *
     * @param   i  ds::Instance, camera component instance to get the far
     * clipping plane distance of.
     * @param      float, value of given camera component's far clipping plane
     * distance.
     */
    float GetFarClippingPlane(ds::Instance i) const;

    /**
     * Set the far clipping plane distance of the given camera component
     * instance.
     *
     * @param  i     ds::Instance, camera component instance to set the far
     * clipping plane of.
     * @param  dist  float, value for far clipping plane distance of camera to
     * set.
     */
    void SetFarClippingPlane(ds::Instance i, float dist);

private:
};
}
