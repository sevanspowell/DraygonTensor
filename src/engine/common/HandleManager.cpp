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
#include <cassert>

#include "engine/common/HandleManager.h"

namespace ds
{
HandleManager::HandleEntry::HandleEntry()
    : nextFreeIndex(0), counter(1), active(0), endOfList(0), entry(nullptr)
{
}

HandleManager::HandleEntry::HandleEntry(uint32_t nextFreeIndex)
    : nextFreeIndex(nextFreeIndex),
      counter(1),
      active(0),
      endOfList(0),
      entry(nullptr)
{
}

HandleManager::HandleManager()
{
    Reset();
}

void HandleManager::Reset()
{
    m_activeEntryCount = 0;
    m_firstFreeEntry = 0;

    for (uint32_t i = 0; i < MAX_ENTRIES - 1; ++i)
    {
        // Initialize each entry with the next free entry
        m_entries[i] = HandleEntry(i + 1);
    }

    m_entries[MAX_ENTRIES - 1] = HandleEntry();
    m_entries[MAX_ENTRIES - 1].endOfList = true;
}

Handle HandleManager::Add(void *p, uint32_t type)
{
    assert(m_activeEntryCount < MAX_ENTRIES - 1 &&
           "HandleManager::Add: Entry list full.");
    assert(type >= 0 && type <= 31 && "HandleManager::Add: Invalid type.");

    const int newIndex = m_firstFreeEntry;
    assert(newIndex < MAX_ENTRIES &&
           "HandleManager::Add: First free entry is invalid.");
    assert(m_entries[newIndex].active == false &&
           "HandleManager::Add: First free entry is already being used.");
    assert(!m_entries[newIndex].endOfList &&
           "HandleManager::Add: First free entry is end of list.");

    m_firstFreeEntry = m_entries[newIndex].nextFreeIndex;
    // Next free index no longer used
    m_entries[newIndex].nextFreeIndex = 0;
    m_entries[newIndex].counter = m_entries[newIndex].counter + 1;
    // Wrap around counter properly
    if (m_entries[newIndex].counter == 0)
    {
        m_entries[newIndex].counter = 1;
    }
    // Entry is now being used
    m_entries[newIndex].active = true;
    m_entries[newIndex].entry = p;

    ++m_activeEntryCount;

    return Handle(newIndex, m_entries[newIndex].counter, type);
}

void HandleManager::Update(Handle handle, void *p)
{
    const int index = handle.index;
    assert(m_entries[index].counter == handle.counter &&
           "HandleManager::Update: Handle is out of date.");
    assert(m_entries[index].active == true &&
           "HandleManager::Update: Handle no longer active.");

    m_entries[index].entry = p;
}

void HandleManager::Remove(const Handle handle)
{
    const uint32_t index = handle.index;
    assert(
        m_entries[index].counter == handle.counter &&
        "HandleManager::Remove: Entry does not exist or Handle out of date.");
    assert(m_entries[index].active == true &&
           "HandleManager::Remove: Handle no longer active.");

    // Store first free entry index
    m_entries[index].nextFreeIndex = m_firstFreeEntry;
    m_entries[index].active = 0;
    // Set first free entry to this entry
    m_firstFreeEntry = index;

    --m_activeEntryCount;
}

void *HandleManager::Get(Handle handle) const
{
    void *p = nullptr;

    if (!Get(handle, &p))
    {
        p = nullptr;
    }

    return p;
}

bool HandleManager::Get(const Handle handle, void **out) const
{
    const int index = handle.index;
    if (m_entries[index].counter != handle.counter ||
        m_entries[index].active == false)
    {
        return false;
    }

    *out = m_entries[index].entry;
    return true;
}

int HandleManager::GetCount() const
{
    return m_activeEntryCount;
}
}
