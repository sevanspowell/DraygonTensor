#pragma once

#include "engine/entity/ComponentManager.h"

namespace ds
{
template <typename T>
void ComponentStore::CreateComponentManager()
{
    std::map<std::type_index,
             std::unique_ptr<IComponentManager>>::const_iterator it =
        m_componentManagers.find(typeid(T));
    // Only add component manager if one doesn't already exist
    if (it == m_componentManagers.end())
    {
        m_componentManagers[typeid(T)] =
            std::unique_ptr<IComponentManager>((IComponentManager *)new T());
    }
}

template <typename T>
const T *ComponentStore::GetComponentManager() const
{
    // Only creates component manager if one does not exist already
    CreateComponentManager<T>();

    std::map<std::type_index,
             std::unique_ptr<IComponentManager>>::const_iterator it =
        m_componentManagers.find(typeid(T));

    return static_cast<const T *>(it->second.get());
}

template <typename T>
T *ComponentStore::GetComponentManager()
{
    // Only creates component manager if one does not exist already
    CreateComponentManager<T>();

    std::map<std::type_index, std::unique_ptr<IComponentManager>>::iterator it =
        m_componentManagers.find(typeid(T));

    return static_cast<T *>(it->second.get());
}
}
