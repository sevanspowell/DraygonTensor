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

#include "engine/json/JsonTypes.h"

namespace ds
{
/**
 * Functions to manipulate JsonObjects.
 */
namespace json_object
{
/**
 * Returns the number of keys in the json object.
 *
 * @param   jsonObject   const JsonObject &, json object to get size of.
 * @return               uint32_t, number of keys in the json object.
 */
uint32_t size(const JsonObject &jsonObject);

/**
 * Returns true if the given key exists in the given json object and false
 * otherwise.
 *
 * @pre   key is a NULL-terminated string.
 *
 * @param   jsonObject   const JsonObject &, json object to find key in.
 * @param   key          const char *, key to attempt to find in json object.
 * @param                bool, true if the given key exists in the map, false
 * otherwise.
 */
bool has(const JsonObject &jsonObject, const char *key);

/**
 * Returns a pointer to the first item in the json object, which can be used to
 * efficiently iterate over the elements in the map (in random order).
 *
 * @param   jsonObject   const JsonObject &, json object to get iterator to.
 * @return               const std::map<FixedString, const char
 * *>::const_iterator, pointer to first item.
 */
const std::map<FixedString, const char *>::const_iterator
begin(const JsonObject &jsonObject);

/**
 * Returns a pointer to the last item in the json object.
 *
 * @param   jsonObject   const JsonObject &, json object to get iterator to.
 * @return               const std::map<FixedString, const char
 * *>::const_iterator, pointer to last item.
 */
const std::map<FixedString, const char *>::const_iterator
end(const JsonObject &jsonObject);
}

JsonObject::JsonObject()
{
}

const char *JsonObject::operator[](const char *key) const
{
    std::map<FixedString, const char *>::const_iterator it;
    it = m_map.find(FixedString(key));

    if (it != m_map.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

const char *JsonObject::operator[](const FixedString &key) const
{
    std::map<FixedString, const char *>::const_iterator it;
    it = m_map.find(key);

    if (it != m_map.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}
}
