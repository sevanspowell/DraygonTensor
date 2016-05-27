#include <cassert>

#include "engine/system/render/ButtonComponentManager.h"

namespace ds_render
{
const Material &ButtonComponentManager::GetDefaultMaterial(ds::Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::GetDefaultMaterial: tried to get invalid "
           "instance");

    return m_data.component[i.index].defaultMaterial;
}

void ButtonComponentManager::SetDefaultMaterial(ds::Instance i,
                                                const Material &material)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::SetDefaultMaterial: tried to set invalid "
           "instance");

    m_data.component[i.index].defaultMaterial = material;
}

const Material &ButtonComponentManager::GetPressedMaterial(ds::Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::GetPressedMaterial: tried to get invalid "
           "instance");

    return m_data.component[i.index].pressedMaterial;
}

void ButtonComponentManager::SetPressedMaterial(ds::Instance i,
                                                const Material &material)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::SetPressedMaterial: tried to set invalid "
           "instance");

    m_data.component[i.index].pressedMaterial = material;
}

const Material &ButtonComponentManager::GetHoverMaterial(ds::Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::GetHoverMaterial: tried to get invalid "
           "instance");

    return m_data.component[i.index].hoverMaterial;
}

void ButtonComponentManager::SetHoverMaterial(ds::Instance i,
                                              const Material &material)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "ButtonComponentManager::SetHoverMaterial: tried to set invalid "
           "instance");

    m_data.component[i.index].hoverMaterial = material;
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
