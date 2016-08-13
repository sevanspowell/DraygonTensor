#include <cassert>

#include "engine/system/render/ButtonComponentManager.h"

namespace ds_render
{
MaterialHandle ButtonComponentManager::GetDefaultMaterialHandle(ds::Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::GetDefaultMaterial: tried to get invalid "
           "instance");

    return m_data.component[i.index].defaultMaterial;
}

void ButtonComponentManager::SetDefaultMaterialHandle(
    ds::Instance i, MaterialHandle materialHandle)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::SetDefaultMaterial: tried to set invalid "
           "instance");

    m_data.component[i.index].defaultMaterial = materialHandle;
}


MaterialHandle
ButtonComponentManager::GetPressedMaterialHandle(ds::Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::GetPressedMaterial: tried to get invalid "
           "instance");

    return m_data.component[i.index].pressedMaterial;
}

void ButtonComponentManager::SetPressedMaterialHandle(
    ds::Instance i, MaterialHandle materialHandle)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::SetPressedMaterial: tried to set invalid "
           "instance");

    m_data.component[i.index].pressedMaterial = materialHandle;
}

MaterialHandle
ButtonComponentManager::GetHoverMaterialHandle(ds::Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::GetHoverMaterial: tried to get invalid "
           "instance");

    return m_data.component[i.index].hoverMaterial;
}

void ButtonComponentManager::SetHoverMaterialHandle(
    ds::Instance i, MaterialHandle materialHandle)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::SetHoverMaterial: tried to set invalid "
           "instance");

    m_data.component[i.index].hoverMaterial = materialHandle;
}

float ButtonComponentManager::GetStartXCoordinate(ds::Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::GetStartXCoordinate: tried to get invalid "
           "instance");

    return m_data.component[i.index].startX;
}

void ButtonComponentManager::SetStartXCoordinate(ds::Instance i, float startX)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::SetStartXCoordinate: tried to set invalid "
           "instance");

    m_data.component[i.index].startX = startX;
}

float ButtonComponentManager::GetStartYCoordinate(ds::Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::GetStartYCoordinate: tried to get invalid "
           "instance");

    return m_data.component[i.index].startY;
}

void ButtonComponentManager::SetStartYCoordinate(ds::Instance i, float startY)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::SetStartYCoordinate: tried to set invalid "
           "instance");

    m_data.component[i.index].startY = startY;
}

float ButtonComponentManager::GetEndXCoordinate(ds::Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::GetEndXCoordinate: tried to get invalid "
           "instance");

    return m_data.component[i.index].endX;
}

void ButtonComponentManager::SetEndXCoordinate(ds::Instance i, float endX)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::SetEndXCoordinate: tried to set invalid "
           "instance");

    m_data.component[i.index].endX = endX;
}

float ButtonComponentManager::GetEndYCoordinate(ds::Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::GetEndYCoordinate: tried to get invalid "
           "instance");

    return m_data.component[i.index].endY;
}

void ButtonComponentManager::SetEndYCoordinate(ds::Instance i, float endY)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::SetEndYCoordinate: tried to set invalid "
           "instance");

    m_data.component[i.index].endY = endY;
}
}
