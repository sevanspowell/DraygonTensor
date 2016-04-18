template <typename T>
unsigned int ComponentManager<T>::GetNumInstances() const
{
    /* Arrays entity and component should be the same size,
     * so just return size of one of the arrays to get the
     * number of active instances.
     */
    return m_data.entity.size();
}

template <typename T>
Instance
ComponentManager<T>::CreateComponentForEntity(Entity entity)
{
    // Index to new component instance will be put into
    unsigned int newIndex = m_data.entity.size();

    m_data.entity.push_back(entity);
    m_data.component.push_back(T());

    // Put new entry into the map (mapping index array to Entity)
    m_map[entity.id] = newIndex;

    // Return instance to caller
    return Instance::MakeInstance(newIndex);
}

template <typename T>
Instance ComponentManager<T>::GetInstanceForEntity(Entity entity) const
{
    int index = -1;
    EntityMap::const_iterator it = m_map.find(entity.id);

    if (it != m_map.end())
    {
        index = it->second;
    }

    return Instance::MakeInstance(index);
}

template <typename T>
Entity ComponentManager<T>::GetEntityForInstance(Instance i) const
{
    Entity e;

    const int index = i.index;

    if (index < GetNumInstances() && index >= 0)
    {
        e = m_data.entity[index];
    }

    return e;
}

template <typename T>
void ComponentManager<T>::OnAddressChange(const Instance &oldAddress,
                                          const Instance &newAddress)
{
}

/**
 * Be very careful with this method, if overriding component manager
 * manages a component which contains references to other components,
 * this method will not keep those references intact. In cases like
 * the above, you should override OnAddressChange to keep the references
 * intact. For an example of this, see TransformComponentManager.
 */
template <typename T>
bool ComponentManager<T>::RemoveInstance(Instance i)
{
    bool result = false;

    // Keep data array tightly packed by swapping last element
    // with element we want to remove and reducing the number
    // of active instances.
    const int index = i.index;
    const unsigned int lastIndex = GetNumInstances() - 1;

    // Make sure we are trying to delete a valid instance
    if (index < GetNumInstances() && index >= 0)
    {
        // Get the entity at the index to destroy
        Entity entityToDestroy = m_data.entity[index];
        // Get the entity at the end of the array
        Entity lastEntity = m_data.entity[lastIndex];

        // Update the references of the deleted component
        OnAddressChange(index, -1);
        // Update the references of the moved component
        OnAddressChange(lastIndex, index);

        // Move last entity's data
        m_data.entity[index] = m_data.entity[lastIndex];
        m_data.component[index] = m_data.component[lastIndex];

        // Update map entry for the swapped entity
        m_map[lastEntity.id] = index;
        // Remove the map entry for the destroyed entity
        m_map.erase(entityToDestroy.id);

        // Destroy component at end of array
        m_data.entity.pop_back();
        m_data.component.pop_back();

        result = true;
    }

    return result;
}
