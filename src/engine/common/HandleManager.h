/*
* Copyright (c) 2008, Power of Two Games LLC
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*    * Redistributions of source code must retain the above copyright
*      notice, this list of conditions and the following disclaimer.
*    * Redistributions in binary form must reproduce the above copyright
*      notice, this list of conditions and the following disclaimer in the
*      documentation and/or other materials provided with the distribution.
*    * Neither the name of Power of Two Games LLC nor the
*      names of its contributors may be used to endorse or promote products
*      derived from this software without specific prior written permission.
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
 * @modified   Samuel Evans-Powell, 14/03/2016
 */
#pragma once

#include <iostream>
#include <memory>

#include "Handle.h"

namespace ds 
{
/**
 * The HandleManager class maps Handles to HandleEntry
 * objects. These HandleEntry objects provide a pointer
 * to the data the Handle should refer to.
 */
class HandleManager
{
public:
    enum
    {
        MAX_ENTRIES = 4096
    };

    /**
     * Default constructor.
     *
     * Creates HandleManager, clears all HandleEntrys
     * ready for use.
     */
    HandleManager();

    /**
     * Resets the HandleManager to a state as if it was just constructed.
     *
     */
    void Reset();
    /**
     * Add new data to the HandleManager and return a Handle
     * that can be used to refer to that data.
     *
     * @pre  HandleManager must not be full.
     * @pre  Type must be between 0 and 31 inclusive.
     *
     * @param  p       void *, pointer to data that Handle should refer to.
     * @param  type    uint32_t, type of the data being pointed to. Should be
     *                 an integer value between: 0 and (2^(number of bits used
     * to represent type in Handle) - 1).
     * @return         Handle, constructed handle that can be used to refer to
     * given data.
     */
    Handle Add(void *p, uint32_t type);
    /**
     * Provides a way of updating the data a Handle refers to.
     *
     * Example usage: If the pointer to the data used to construct the Handle
     * moves, it needs to be updated in order for the Handle to continue to
     * refer to valid data.
     *
     * @pre  Handle must exist and be active in HandleManager.
     *
     * @param     handle  Handle, handle to update.
     * @param     p       void *, new data pointer to use.
     */
    void Update(Handle handle, void *p);
    /**
     * Remove a Handle from the system. This allows the handle to
     * be reused and invalidates any use of the old Handle.
     *
     * @pre  Handle must exist and be active in HandleManager.
     *
     * @param     handle  Handle, handle to remove.
     */
    void Remove(Handle handle);

    /**
     * Get the data associated with a handle.
     *
     * Will return nullptr if handle is invalid.
     *
     * @param     handle  Handle, handle data to get.
     */
    void *Get(Handle handle) const;
    /**
     * Get the data associated with a handle.
     *
     * Will return false if handle is invalid.
     *
     * @param     handle  Handle, handle to data to get.
     * @param     out     void **, pointer to void pointer. Where data will be
     * placed if valid Handle is given.
     * @return            bool, true if given Handle points to valid data,
     * false otherwise.
     */
    bool Get(Handle handle, void **out) const;
    /**
     * Get the data associated with a handle cast to the
     * given type.
     *
     * Will return false if handle is invalid.
     *
     * @param  handle  Handle, handle to data to get.
     * @param  out     T *, address to type. Where data will
     *                 be placed if valid Handle is given.
     * @return         bool, true if given Handle points to valid data, false
     * otherwise.
     */
    template <typename T>
    bool GetAs(Handle handle, T *out) const;

    /**
     * Return the number of active handle entries in the manager.
     *
     * @return     int, number of active handle entries in the manager.
     */
    int GetCount() const;

private:
    /*
     * Make copy constructors private.
     */
    HandleManager(const HandleManager &handleManager);
    HandleManager &operator=(const HandleManager &handleManager);

    /**
     * Each HandleEntry is initialized so that it holds a pointer to
     * the next free HandleEntry. The HandleEntry at "MAX_ENTRIES" is
     * set to indicate that it is at the end of the list.
     */
    struct HandleEntry
    {
        /**
         * HandleEntry default constructor.
         */
        HandleEntry();
        /**
         * Handle entry constructor.
         *
         * @param   nextFreeIndex  uint32_t, next free HandleEntry in list.
         */
        explicit HandleEntry(uint32_t nextFreeIndex);

        // Bitfield
        // Index to next free HandleEntry
        uint32_t nextFreeIndex : 12;
        // Generation counter
        uint32_t counter : 15;
        // Is this entry being used?
        uint32_t active : 1;
        // Is this entry the end of the list?
        uint32_t endOfList : 1;
        // Data ptr
        void *entry;
    };

    HandleEntry m_entries[MAX_ENTRIES];

    // Number of active entries
    int m_activeEntryCount;
    // Index to first free entry in list
    uint32_t m_firstFreeEntry;
};

template <typename T>
inline bool HandleManager::GetAs(Handle handle, T *out) const
{
    void *outAsVoid;

    const bool result = Get(handle, &outAsVoid);
    *out = *(reinterpret_cast<T *>(outAsVoid));

    return result;
}
}
