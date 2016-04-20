/**
 * This code was only slightly modified from this source:
 * http://bitsquid.blogspot.com.au/2014/08/building-data-oriented-entity-system.html?m=1
 *
 * @author Niklas
 * @author Samuel Evans-Powell (modified)
 * @date   16/04/2016
 */
#pragma once

#include <cstdint>

namespace ds
{
/**
 * An entity is merely a unique identifier.
 *
 * This unique identifier is made up of two parts:
 *     - an index (lower, less significant X bits)
 *     - a generation (higher, more significant Y bits)
 * Each time an index is re-used it's generation is increased,
 * this allows us to check if an Entity id is valid or not.
 */
class Entity
{
public:
    /**
     * Get the index part of the Entity id.
     *
     * @return     unsigned int, index part of Entity id.
     */
    unsigned int GetIndex() const;
    /**
     * Get the generation part of the Entity id.
     *
     * @return     unsigned int, generation part of Entity id.
     */
    unsigned int GetGeneration() const;

    /** Number of bits making up index */
    static const unsigned int ENTITY_INDEX_BITS = 22;
    /**
     * Index mask.
     *
     * (1 << 22) will leave a 1 in the 23rd bit and 0s elsewhere.
     * Subtracting 1 from this will leave 1s in all 22 bits and 0s elsewhere,
     * forming a mask
     * for the index bits.
     */
    static const uint32_t ENTITY_INDEX_MASK = (1 << ENTITY_INDEX_BITS) - 1;

    /** Number of bits making up generation */
    static const unsigned int ENTITY_GENERATION_BITS = 8;
    /** Generation mask */
    static const uint32_t ENTITY_GENERATION_MASK =
        (1 << ENTITY_GENERATION_BITS) - 1;

    /** Unique identifier */
    uint32_t id;
};
}
