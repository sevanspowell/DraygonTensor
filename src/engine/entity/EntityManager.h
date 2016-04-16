/**
 * This code was only slightly modified from this source:
 * http://bitsquid.blogspot.com.au/2014/08/building-data-oriented-entity-system.html?m=1
 *
 * @author Niklas
 * @author Samuel Evans-Powell (modified)
 * @date   16/04/2016
 */
#pragma once

#include <queue>
#include <vector>

#include "engine/entity/Entity.h"

namespace ds
{
/**
 * Manages Entities in the system.
 *
 * Re-uses expired entity IDs.
 * Prevents duplication of entity IDs.
 * Facilitates clean removal of entity by removing it from all systems. -
 * Observer pattern
 */
class EntityManager
{
public:
    /**
     * Entity id's are a weak reference. This
     * method checks to see if the id is valid.
     *
     * Index slots are re-used and a generation
     * field is used to keep track of which index
     * slots are valid. This methods checks that
     * the generation field matches the current known
     * generation value for that index. If they
     * don't match, the Entity is no longer valid.
     *
     * @param  e   Entity, entity id to check for validity.
     * @return     bool, TRUE if entity id is valid, FALSE otherwise.
     */
    bool IsValid(Entity e) const;

    /**
     * Destroy an Entity.
     *
     * Release the Entity's id so it can be re-used
     * and invalidate any current references to the Entity.
     *
     * @param  e   Entity, entity to destroy.
     */
    void Destroy(Entity e);

    /**
     * Create a new Entity.
     *
     * @return     Entity, created entity.
     */
    Entity Create();

private:
    /**
     * Construct an Entity from an index and a generation value.
     *
     * @pre    index is smaller than the maximum value possible given the
     * number
     *         of bits assigned to it.
     * @pre    generation is smaller than the maximum value possible given the
     * number
     *         of bits assigned to it.
     *
     * @param  index       unsigned int, index value to give Entity.
     * @param  generation  unsigned int, generation value to give Entity.
     * @return             Entity, constructed from index and value.
     */
    Entity MakeEntity(unsigned int index, unsigned int generation) const;

    std::queue<unsigned int> m_freeIndices;
    std::vector<unsigned char> m_generation;
};
}
