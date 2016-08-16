#include <cassert>

#include "engine/system/render/CameraComponentManager.h"

namespace ds_render
{
const ds_math::Matrix4 &
CameraComponentManager::GetProjectionMatrix(ds::Instance i) const
{
    assert(i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
           "CameraComponentManager::GetProjectionMatrix: tried to get invalid "
           "instance");

    return m_data.component[i.index].projectionMatrix;
}

float CameraComponentManager::GetVerticalFieldOfView(ds::Instance i) const
{
    assert(
        i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
        "CameraComponentManager::GetVerticalFieldOfView: tried to get invalid "
        "instance");

    return m_data.component[i.index].verticalFov;
}

void CameraComponentManager::SetVerticalFieldOfView(ds::Instance i, float fov)
{
    assert(
        i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
        "CameraComponentManager::SetVerticalFieldOfView: tried to set invalid "
        "instance");

    m_data.component[i.index].verticalFov = fov;

    // Update projection matrix
    m_data.component[i.index].projectionMatrix =
        ds_math::Matrix4::CreatePerspectiveFieldOfView(
            m_data.component[i.index].verticalFov,
            m_data.component[i.index].aspectRatio,
            m_data.component[i.index].nearZClip,
            m_data.component[i.index].farZClip);

}

float CameraComponentManager::GetAspectRatio(ds::Instance i) const
{
    assert(
        i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
        "CameraComponentManager::GetAspectRatio: tried to get invalid "
        "instance");

    return m_data.component[i.index].aspectRatio;
}

void CameraComponentManager::SetAspectRatio(ds::Instance i, float aspectRatio)
{
    assert(
        i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
        "CameraComponentManager::SetAspectRatio: tried to set invalid "
        "instance");

    m_data.component[i.index].aspectRatio = aspectRatio;

    // Update projection matrix
    m_data.component[i.index].projectionMatrix =
        ds_math::Matrix4::CreatePerspectiveFieldOfView(
            m_data.component[i.index].verticalFov,
            m_data.component[i.index].aspectRatio,
            m_data.component[i.index].nearZClip,
            m_data.component[i.index].farZClip);
}

float CameraComponentManager::GetNearClippingPlane(ds::Instance i) const
{
    assert(
        i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
        "CameraComponentManager::GetNearClippingPlane: tried to get invalid "
        "instance");

    return m_data.component[i.index].nearZClip;
}

void CameraComponentManager::SetNearClippingPlane(ds::Instance i, float dist)
{
    assert(
        i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
        "CameraComponentManager::SetNearClippingPlane: tried to set invalid "
        "instance");

    m_data.component[i.index].nearZClip = dist;

    // Update projection matrix
    m_data.component[i.index].projectionMatrix =
        ds_math::Matrix4::CreatePerspectiveFieldOfView(
            m_data.component[i.index].verticalFov,
            m_data.component[i.index].aspectRatio,
            m_data.component[i.index].nearZClip,
            m_data.component[i.index].farZClip);
}

float CameraComponentManager::GetFarClippingPlane(ds::Instance i) const
{
    assert(
        i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
        "CameraComponentManager::GetFarClippingPlane: tried to get invalid "
        "instance");

    return m_data.component[i.index].farZClip;
}

void CameraComponentManager::SetFarClippingPlane(ds::Instance i, float dist)
{
    assert(
        i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
        "CameraComponentManager::SetFarClippingPlane: tried to set invalid "
        "instance");

    m_data.component[i.index].farZClip = dist;

    // Update projection matrix
    m_data.component[i.index].projectionMatrix =
        ds_math::Matrix4::CreatePerspectiveFieldOfView(
            m_data.component[i.index].verticalFov,
            m_data.component[i.index].aspectRatio,
            m_data.component[i.index].nearZClip,
            m_data.component[i.index].farZClip);
}
}
