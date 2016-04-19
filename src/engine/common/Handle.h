/*
* Copyright (c) 2008, Power of Two Games LLC
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Power of Two Games LLC nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY POWER OF TWO GAMES LLC ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL POWER OF TWO GAMES LLC BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**
 *  @modified   Samuel Evans-Powell on 14/03/2016
 */
#pragma once

#include <cstdint>

namespace ds 
{
/**
 * Handle class, provides a way of referencing
 * data that can change memory locations.
 *
 * Exists with a HandleManager.
 *
 * Handles are merely a 32-bit uint split into
 * three different sections:
 *     - index: provides a direct (ie. fast) index into
 *              an array in the HandleManager which maps
 *              the Handle to a data pointer.
 *     - counter:  Handles need to be able to be re-used,
 *                 so how do we tell the difference between
 *                 an old and a new handle? Use a generation
 *                 counter.
 *     - type:     Provides a way of determining the type
 *                 of the data being pointed to by the Handle.
 */
struct Handle
{
    /**
     * Default constructor.
     *
     * Give Handle sensible initial values.
     */
    Handle() : index(0), counter(0), type(0)
    {
    }

    /**
     * Constructor.
     *
     * Create Handle with given initial values.
     *
     * @param  index       uint32_t, index into HandleManager.
     * @param  counter     uint32_t, generation counter.
     * @param  type        uint32_t, type Handle is pointing to.
     */
    Handle(uint32_t index, uint32_t counter, uint32_t type)
        : index(index), counter(counter), type(type)
    {
    }

    /**
     * Convert Handle to single uint32, because
     * members are stored as three seperate uint32s.
     *
     * @return     uint32_t, Handle compressed as single uint32.
     */
    inline operator uint32_t() const;

    // Bitfield (http://en.cppreference.com/w/cpp/language/bit_field)
    uint32_t index : 12;
    uint32_t counter : 15;
    uint32_t type : 5;
};

Handle::operator uint32_t() const
{
    return type << 27 | counter << 12 | index;
}
}
