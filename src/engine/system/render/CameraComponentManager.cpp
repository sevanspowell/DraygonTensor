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

const ds_math::Quaternion &
CameraComponentManager::GetOrientation(ds::Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "CameraComponentManager::GetOrientation: tried to get invalid "
           "instance");

    std::cout << "GetOrientation: " << m_data.component[i.index].orientation
              << std::endl;
    return m_data.component[i.index].orientation;
}

void CameraComponentManager::SetOrientation(
    ds::Instance i, const ds_math::Quaternion &orientation)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "CameraComponentManager::SetOrientation: tried to set invalid "
           "instance");

    m_data.component[i.index].orientation = orientation;
}
}
