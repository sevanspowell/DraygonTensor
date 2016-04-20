#pragma once

#include <memory>

#include "engine/entity/Entity.h"
#include "engine/entity/Instance.h"

namespace ds
{
/**
 * A component manager will manage componentes in the engine.
 *
 * Any concrete manager class will:
 * - use Instances as a method of referring to a specific component instance in
 * the manager.
 * - provide getter and setter methods for all the component's data members.
 */
class IComponentManager
{
public:
    /**
     * Virtual destructor to ensure that sub-call destructor is called.
     */
    virtual ~IComponentManager(){};

    /**
     * Return the number of components that currently exist within the component
     * manager.
     *
     * @return     unsigned int, number of component instances the component
     * manager is managing.
     */
    virtual unsigned int GetNumInstances() const = 0;

    /**
     * Create a component for the given entity and return a component instance
     * which can be used to refer to that component.
     *
     * @param   entity  Entity, entity to create component for. The component
     * will be associated with this entity.
     * @return          Instance, the new component instance created.
     */
    virtual Instance CreateComponentForEntity(Entity entity) = 0;

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
    virtual Instance GetInstanceForEntity(Entity entity) const = 0;

    /**
     * Get the entity associated with the given instance.
     *
     * @param   i  Instance, component instance to get owning entity of.
     * @return     Entity, owning entity.
     */
    virtual Entity GetEntityForInstance(Instance i) const = 0;

    /**
     * Remove a component instance from the manager.
     *
     * @param   i  Instance, the component instance to remove.
     * @return     bool, TRUE if the remove was successful, FALSE otherwise.
     */
    virtual bool RemoveInstance(Instance i) = 0;
};
}
