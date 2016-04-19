/**
 *  This code was only slightly modified from this source:
 *  http://bitsquid.blogspot.com.au/2014/08/building-data-oriented-entity-system.html?m=1
 *
 *  @author Niklas
 *  @author Samuel Evans-Powell (modified)
 *  @date   16/04/2016
 */
#include <cassert>

#include "engine/entity/EntityManager.h"
#include "engine/entity/Instance.h"

namespace ds
{
/**
 *  Minimum indices to allow in queue before we start
 *  drawing indices from the queue.
 */
static const int MINIMUM_INDICES_QUEUE = 1024;

bool EntityManager::IsValid(Entity e) const
{
    bool result = false;

    // Only try to check Entities that could possibly
    // have an entry in out generation array.
    if (e.GetIndex() < m_generation.size())
    {
        // Entity is only valid if the Entity's generation
        // matches our recorded valid generation.
        result = m_generation[e.GetIndex()] == e.GetGeneration();
    }

    return result;
}

void EntityManager::Destroy(Entity e)
{
    // Only try to destroy valid Entities.
    if (IsValid(e))
    {
        const unsigned int index = e.GetIndex();

        // Free index to be re-used
        m_freeIndices.push(index);
        // Increment the generation value for that index,
        // invalidating any previous references to that Entity.
        m_generation[index]++;
    }
}

Entity EntityManager::Create()
{
    unsigned int index = 0;
    unsigned int generation = 0;

    // If the queue hasn't filled up, create new index.
    if (m_freeIndices.size() < MINIMUM_INDICES_QUEUE)
    {
        m_generation.push_back(0);
        index = m_generation.size() - 1;
    }
    // If the queue has filled up, re-use index from queue.
    else
    {
        index = m_freeIndices.front();
        m_freeIndices.pop();
        generation = m_generation[index];
    }

    return MakeEntity(index, generation);
}

Entity EntityManager::MakeEntity(unsigned int index,
                                 unsigned int generation) const
{
    // Ensure index is not above maximum amount of Entities.
    assert(index < (Entity::ENTITY_INDEX_MASK + 1) &&
           "Tried to create an Entity with too high an index.");
    // Ensure generation is not about maximum generation.
    assert(generation < (Entity::ENTITY_GENERATION_MASK + 1) &&
           "Tried to create an Entity with too high a generation.");

    Entity e;
    e.id = (generation << Entity::ENTITY_INDEX_BITS) + index;

    return e;
}
}
