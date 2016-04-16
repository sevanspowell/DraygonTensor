/**
 * This code was only slightly modified from this source:
 * http://bitsquid.blogspot.com.au/2014/08/building-data-oriented-entity-system.html?m=1
 *
 * @author Niklas
 * @author Samuel Evans-Powell (modified)
 * @date   16/04/2016
 */
#pragma once

namespace ds
{
/**
 * The index of an Entity does not map directly to an index
 * into the array of components. Instead, a layer of indirection
 * is used.
 *
 * This instance type essentially represents the real index into
 * the component array. It is a way of referring to a specific
 * component instance.
 */
struct Instance
{
    Instance(int index = -1)
    {
        this->index = index;
    }

    /**
     * Convenience method, create an Instance from an array index.
     *
     * @param  index   unsigned int, index into array.
     * @return         Instance, Instance handle.
     */
    static Instance MakeInstance(int index)
    {
        Instance instance;
        instance.index = index;

        return instance;
    }

    /**
     * Convenience method, create an invalid Instance.
     *
     * @return     Instance, Instance handle of an invalid Instance.
     */
    static Instance MakeInvalidInstance()
    {
        Instance instance = Instance(-1);

        return instance;
    }

    /**
     * Equivalence operator.
     *
     * Compare two Instances and return TRUE if they are equal or FALSE
     * otherwise.
     *
     * @param  other   const Instance reference, other Instance to compare to
     * this Instance.
     * @return         bool, TRUE if two Instances are equal, FALSE otherwise.
     */
    bool operator==(const Instance &other) const
    {
        return (index == other.index);
    }

    bool IsValid() const
    {
        if (index == -1)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    int index;
};
}
