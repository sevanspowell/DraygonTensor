/*
 * Copyright (c) 2012-2016 Daniele Bartolini and individual contributors.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Modified by Samuel Evans-Powell
 */
#pragma once

#include <map>
#include <vector>

#include "engine/strings/FixedString.h"

namespace ds
{
namespace JsonValueType
{
enum Enum
{
    Nil,
    Bool,
    Number,
    String,
    Array,
    Object
};
}

/** Array of pointers to json-encoded data */
typedef std::vector<const char *> JsonArray;

/**
 * Map key to pointers to json-encoded data.
 */
struct JsonObject
{
    /**
     * Create a json Object.
     *
     * @param   allocator   Allocator &, memory allocator to use.
     */
    JsonObject();

    /**
     * Get json-encoded data associated with the given key, or nullptr if no
     * matching key found.
     *
     * @pre   key is a NULL-terminated string.
     *
     * @param   key   const char *, key.
     * @return        const char *, pointer to json encoded data.
     */
    const char *operator[](const char *key) const;

    /**
     * Get json-encoded data associated with the given key, or nullptr if no
     * matching key found.
     *
     * @param   key   const FixedString &, key.
     * @return        const char *, pointer to json encoded data.
     */
    const char *operator[](const FixedString &key) const;

    std::map<FixedString, const char *> m_map;
};
}
