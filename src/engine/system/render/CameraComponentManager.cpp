#include <cassert>

#include "engine/system/render/CameraComponentManager.h"

namespace ds_render
{
const ds_math::Matrix4 &
CameraComponentManager::GetProjectionMatrix(ds::Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "CameraComponentManager::GetProjectionMatrix: tried to get invalid "
           "instance");

    return m_data.component[i.index].projectionMatrix;
}

void CameraComponentManager::SetProjectionMatrix(
    ds::Instance i, const ds_math::Matrix4 &projectionMatrix)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "CameraComponentManager::SetProjectionMatrix: tried to set invalid "
           "instance");

    m_data.component[i.index].projectionMatrix = projectionMatrix;
}
}
