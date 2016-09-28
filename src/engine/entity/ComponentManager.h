#pragma once

#include <unordered_map>
#include <vector>

#include "engine/entity/IComponentManager.h"

namespace ds
{
/**
 * Templated on component type (a struct). Manages components of that type.
 */
template <typename T>
class ComponentManager : public IComponentManager
{
public:
    /**
     * Return the number of components that currently exist within the component
     * manager.
     *
     * @return     unsigned int, number of component instances the component
     * manager is managing.
     */
    virtual unsigned int GetNumInstances() const;

    /**
     * Create a component for the given entity and return a component instance
     * which can be used to refer to that component.
     *
     * @param   entity     Entity, entity to create component for. The component
     * will be associated with this entity.
     * @param   component  const T &, component data to associate with entity.
     * @return             Instance, the new component instance created.
     */
    virtual Instance CreateComponentForEntity(Entity entity);

    /**
     * Get the component instance for the given entity.
     *
     * Returned instance index will be -1 if given entity does not have a
     * component of this type.
     *
     * @param   entity  Entity, get component instance belonging to this entity.
     * @return          Instance, the component instance belonging to that
     * entity.
     */
    virtual Instance GetInstanceForEntity(Entity entity) const;

    /**
     * Get the entity associated with the given instance.
     *
     * @param   i  Instance, component instance to get owning entity of.
     * @return     Entity, owning entity.
     */
    virtual Entity GetEntityForInstance(Instance i) const;

    /**
     * Remove a component instance from the manager.
     *
     * @param   i  Instance, the component instance to remove.
     * @return     bool, TRUE if the remove was successful, FALSE otherwise.
     */
    virtual bool RemoveInstance(Instance i);

    /**
     * Get the component for the given component instance.
     *
     * @param   i   Instance, component instance to get component for.
     * @return      T, component associated with the given component instance.
     */
    virtual T GetComponentForInstance(Instance i) const;

    /**
     * Set the component to be associated with the given component instance.
     *
     * @param   i           Instance, component instance to set component for.
     * @param   component   T, component to set.
     */
    virtual void SetComponentForInstance(Instance i, T component);

protected:
    /**
     * Think of the Instance identifier as a pointer address. When the
     * component is swapped around in memory, it's address changes and this
     * method is called to allow the programmer to keep any references
     * intact. If an object is referencing the moved object and the
     * programmer does not update this reference, the object's reference
     * becomes invalid.
     *
     * See TransformComponentManager for an example of the correct usage
     * of this method.
     *
     * Called before any data is actually moved, so oldAddress can still be
     * used to reference about to be moved component.
     *
     * @post    Once overriden, all references to "old" should be replaced
     *          with reference to "new".
     *
     * @param   oldAddress     const Instance &, old address.
     * @param   newAddress     const Instance &, new address.
     */
    virtual void OnAddressChange(const Instance &oldAddress,
                                 const Instance &newAddress);

    /** Map entity to index into vector of instance data */
    typedef std::unordered_map<uint32_t, size_t> EntityMap;

    /**
     * Parallel arrays, mapping entity id to data belonging to that instance.
     */
    struct InstanceData
    {
        std::vector<Entity> entity;
        std::vector<T> component;
    };

    /** Collection of entities and components */
    InstanceData m_data;
    /** Map entity to index into vector of instance data */
    EntityMap m_map;
};

#include "engine/entity/ComponentManager.hpp"
}
