/**
 *  This code was only slightly modified from this source:
 *  http://bitsquid.blogspot.com.au/2014/08/building-data-oriented-entity-system.html?m=1
 *
 *  @author Niklas
 *  @author Samuel Evans-Powell (modified)
 *  @date   16/04/2016
 */
#include "engine/entity/Entity.h"

namespace ds
{
    unsigned int Entity::GetIndex() const
    {
        // Bitwise AND id with index mask to get index bits only
        return (id & ENTITY_INDEX_MASK);
    }

    unsigned int Entity::GetGeneration() const
    {
        // Because generation bits are in upper 8 bits,
        // shift them down and apply the mask to get 
        // generation bits by themselves.
        return ((id >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK);    
    }
}
