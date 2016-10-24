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
 * Modified by Samuel Evans-Powell.
 */
#include <algorithm>
#include <stdint.h>

#include "engine/strings/FixedString.h"
#include "engine/strings/StringUtils.h"

namespace ds 
{
FixedString::FixedString() : m_length(0), m_data(nullptr)
{
}

FixedString::FixedString(const char *str) : m_length(strlen32(str)), m_data(str)
{
}

FixedString::FixedString(const char *str, uint32_t length)
    : m_length(length), m_data(str)
{
}

FixedString &FixedString::operator=(const char *str)
{
    m_length = strlen32(str);
    m_data = str;
    return *this;
}

uint32_t FixedString::length() const
{
    return m_length;
}

const char *FixedString::data() const
{
    return m_data;
}

bool operator==(const FixedString &str1, const char *str2)
{
    const uint32_t length = strlen32(str2);
    return (str1.m_length == length) &&
           (strncmp(str1.m_data, str2, length) == 0);
}

bool operator==(const FixedString &str1, const FixedString &str2)
{
    return (str1.m_length == str2.m_length) &&
           (strncmp(str1.m_data, str2.m_data, str1.m_length) == 0);
}

bool operator<(const FixedString &str1, const FixedString &str2)
{
    const uint32_t length = std::max(str1.m_length, str2.m_length);
    return strncmp(str1.m_data, str2.m_data, length) < 0;
}
}
