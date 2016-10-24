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

namespace ds 
{
/**
 * String of fixed length.
 */
struct FixedString {

    /**
     * Construct an empty string.
     */
    FixedString();

    /**
     * Construct from a NULL-terminated character array.
     *
     * @pre   Character array is NULL-terminated.
     *
     * @param   str   const char *, NULL-terminated character array.
     */
    FixedString(const char *str);

    /**
     * Construct from a character array.
     */
    FixedString(const char *str, uint32_t length);

    /**
     * Create a FixedString from a NULL-terminated character array.
     *
     * @pre   Character array is NULL-terminated.
     *
     * @param   str   const char *, NULL-terminated character array.
     * @return        FixedString &, created FixedString.
     */
    FixedString &operator=(const char *str);

    /**
     * Return the number of characters in the FixedString.
     *
     * @return   uint32_t, number of characters in the FixedString.
     */
    uint32_t length() const;

    /**
     * Return a pointer to the characters in this string.
     *
     * @return   const char *, pointer to characters in this string.
     */
    const char *data() const;

    uint32_t m_length;
    const char *m_data;
};

/**
 * Returns true if both strings are the same (Same number of characters in same
 * order).
 *
 * @param   str1   const FixedString &, string one.
 * @param   str2   const char *, string two.
 * @return         bool, true if both strings are the same.
 */
bool operator==(const FixedString &str1, const char *str2);

/**
 * Returns true if both strings are the same (Same number of characters in same
 * order).
 *
 * @param   str1   const FixedString &, string one.
 * @param   str2   const FixedString &, string two.
 * @return         bool, true if both strings are the same.
 */
bool operator==(const FixedString &str1, const FixedString &str2);

/**
 * Returns true if the first string is lexographically 'less' than the second
 * string and false if they are 'equal' or if the first string is
 * lexographically
 * 'greater' than the second string.
 *
 * @param   str1   const FixedString &, string one.
 * @param   str2   const FixedString &, string two.
 * @return         bool, true if the first string is lexographically 'less' than
 * the second string, false if they are 'equal' or if the first string is
 * lexographically 'greater' than the second string.
 */
bool operator<(const FixedString &str1, const FixedString &str2);
}
