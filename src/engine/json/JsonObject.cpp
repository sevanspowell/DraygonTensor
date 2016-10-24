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
#include "engine/json/JsonObject.h"

namespace ds
{
namespace json_object
{
inline uint32_t size(const JsonObject &jsonObject)
{
    return jsonObject.m_map.size();
}

inline bool has(const JsonObject &jsonObject, const char *key)
{
    std::map<FixedString, const char *>::const_iterator it;
    it = jsonObject.m_map.find(FixedString(key));

    if (it != jsonObject.m_map.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline const std::map<FixedString, const char *>::const_iterator
begin(const JsonObject &jsonObject)
{
    return jsonObject.m_map.begin();
}

inline const std::map<FixedString, const char *>::const_iterator
end(const JsonObject &jsonObject)
{
    return jsonObject.m_map.end();
}
}
}
