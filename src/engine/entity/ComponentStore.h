#pragma once

#include <map>
#include <memory>
#include <typeindex>
#include <typeinfo>

#include "engine/entity/ComponentManager.h"

namespace ds
{
/**
 * Container for storing component managers and editing, updating,
 * removing, adding to their components.
 */
class ComponentStore
{
public:
    /**
     * Get the component manager of the given type in the component store.
     *
     * @return   const T &, component manager.
     */
    template <typename T>
    const T *GetComponentManager() const;

    /**
     * Get the component manager of the given type in the component store.
     *
     * @return   T &, component manager.
     */
    template <typename T>
    T *GetComponentManager();

private:
    /**
     * Add a component manager to the component store.
     *
     * Added component manager will manage components of the type specified in
     * the template argument.
     *
     * Component manager is only added if no component manager of the given type
     * already exists.
     */
    template <typename T>
    void CreateComponentManager();

    std::map<std::type_index, std::unique_ptr<IComponentManager>>
        m_componentManagers;
};
}

#include "engine/entity/ComponentStore.hpp"
