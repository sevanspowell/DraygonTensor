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
#pragma once

#include <string.h>
#include <stdint.h>
#include <ctype.h>

namespace ds 
{
inline uint32_t strlen32(const char *str)
{
    return (uint32_t)strlen(str);
}

inline const char *skipSpaces(const char *str)
{
    while (isspace(*str)) {
        ++str;
    }

    return str;
}

inline const char *skipBlock(const char *str, char a, char b)
{
    uint32_t num = 0;

    for (char ch = *str++; ch != '\0'; ch = *str++) {
        if (ch == a) {
            ++num;
        } else if (ch == b) {
            if (--num == 0) {
                return str;
            }
        }
    }

    return nullptr;
}

inline const char *skipDelim(const char *str, char delim)
{
    while (*str != '\0' && *str != delim) {
        ++str;
    }

    if (*str == delim) {
        ++str;
    }

    return str;
}

// Written by Jack Handy: jakkhandy@hotmail.com
inline int wildcmp(const char *wild, const char *str)
{
    const char *cp = NULL, *mp = NULL;

    while (*str && *wild != '*') {
        if (*wild != *str && *wild != '?')
            return 0;
        ++wild;
        ++str;
    }

    while (*str) {
        if (*wild == '*') {
            if (!*++wild)
                return 1;
            mp = wild;
            cp = str + 1;
        } else if (*wild == *str || *wild == '?') {
            ++wild;
            ++str;
        } else {
            wild = mp;
            str = cp++;
        }
    }

    while (*wild == '*')
        ++wild;

    return !*wild;
}
}
