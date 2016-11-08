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
#include <cassert>
#include <cstdio>

#include "engine/json/Json.h"
#include "engine/strings/StringUtils.h"

namespace ds
{
namespace json
{
static const char *next(const char *json, const char c = 0)
{
    assert(json != nullptr);

    if (!(c != 0 && c == *json))
    {
        fprintf(stderr, "Expected '%c' got '%c'", c, *json);
    }
    assert(c != 0 && c == *json);

    return ++json;
}

static const char *skipString(const char *json)
{
    assert(json != nullptr);

    bool escaped = false;

    while (*++json)
    {
        if (*json == '"' && !escaped)
        {
            ++json;
            return json;
        }
        else if (*json == '\\')
        {
            escaped = true;
        }
        else
        {
            escaped = false;
        }
    }

    return json;
}

static const char *skipValue(const char *json)
{
    assert(json != nullptr);

    switch (*json)
    {
    case '"':
        json = skipString(json);
        break;
    case '[':
        json = skipBlock(json, '[', ']');
        break;
    case '{':
        json = skipBlock(json, '{', '}');
        break;
    default:
        for (; *json != ',' && *json != '}' && *json != ']'; ++json)
        {
        }
        break;
    }

    return json;
}

JsonValueType::Enum type(const char *json)
{
    assert(json != nullptr);

    switch (*json)
    {
    case '"':
        return JsonValueType::String;
    case '{':
        return JsonValueType::Object;
    case '[':
        return JsonValueType::Array;
    case '-':
        return JsonValueType::Number;
    default:
        return (isdigit(*json))
                   ? JsonValueType::Number
                   : (*json == 'n' ? JsonValueType::Nil : JsonValueType::Bool);
    }
}

void parseString(const char *json, std::string *const string)
{
    assert(json != nullptr);
    assert(string != nullptr);

    if (*json == '"')
    {
        while (*++json)
        {
            if (*json == '"')
            {
                // Handle end of string
                ++json;
                return;
            }
            else if (*json == '\\')
            {
                // Handle escape characters
                ++json;

                switch (*json)
                {
                case '"':
                    *string += '"';
                    break;
                case '\\':
                    *string += '\\';
                    break;
                case '/':
                    *string += '/';
                    break;
                case 'b':
                    *string += '\b';
                    break;
                case 'f':
                    *string += '\f';
                    break;
                case 'n':
                    *string += '\n';
                    break;
                case 'r':
                    *string += '\r';
                    break;
                case 't':
                    *string += '\t';
                    break;
                default:
                    assert(false && "Bad escape character.");
                    break;
                }
            }
            else
            {
                // Handle contents of string
                *string += *json;
            }
        }
    }

    // No closing '"' character
    assert(false && "Bad string - no closing '\"' character.");
}

double parseNumber(const char *json)
{
    assert(json != nullptr);

    std::vector<char> number;

    if (*json == '-')
    {
        number.push_back('-');
        ++json;
    }
    while (isdigit(*json))
    {
        number.push_back(*json);
        ++json;
    }

    if (*json == '.')
    {
        number.push_back('.');
        while (*++json && isdigit(*json))
        {
            number.push_back(*json);
        }
    }

    // Exponents
    if (*json == 'e' || *json == 'E')
    {
        number.push_back(*json);
        ++json;

        if (*json == '-' || *json == '+')
        {
            number.push_back(*json);
            ++json;
        }
        while (isdigit(*json))
        {
            number.push_back(*json);
            ++json;
        }
    }

    number.push_back('\0');

    double val;
    int ok = sscanf(&number[0], "%lf", &val);
    if (!(ok == 1))
    {
        fprintf(stderr, "Failed to parse number: %s", &number[0]);
    }
    assert(ok == 1);

    return val;
}

int32_t parseInt(const char *json)
{
    return ((int32_t)json::parseNumber(json));
}

float parseFloat(const char *json)
{
    return ((float)json::parseNumber(json));
}

bool parseBool(const char *json)
{
    assert(json != nullptr);

    switch (*json)
    {
    case 't':
        // Iterate thru json string looking for these values, fail if any one is
        // not found.
        json = next(json, 't');
        json = next(json, 'r');
        json = next(json, 'u');
        json = next(json, 'e');
        return true;
    case 'f':
        json = next(json, 'f');
        json = next(json, 'a');
        json = next(json, 'l');
        json = next(json, 's');
        json = next(json, 'e');
        return false;
    default:
        assert(false && "Bad boolean.");
        return false;
    }
}

void parseArray(const char *json, JsonArray *const array)
{
    assert(json != nullptr);
    assert(array != nullptr);

    if (*json == '[')
    {
        json = skipSpaces(++json);

        if (*json == ']')
        {
            return;
        }

        while (*json)
        {
            (*array).push_back(json);

            json = skipValue(json);
            json = skipSpaces(json);

            if (*json == ']')
            {
                return;
            }

            json = next(json, ',');
            json = skipSpaces(json);
        }
    }

    assert(false && "Bad array");
}

void parseObject(const char *json, JsonObject *const object)
{
    assert(json != nullptr);
    assert(object != nullptr);

    if (*json == '{')
    {
        json = skipSpaces(++json);

        if (*json == '}')
        {
            return;
        }

        while (*json)
        {
            const char *keyBegin = *json == '"' ? (json + 1) : json;

            std::string key;
            json::parseString(json, &key);

            FixedString fsKey(keyBegin, key.length());

            json = skipString(json);
            json = skipSpaces(json);
            json = next(json, ':');
            json = skipSpaces(json);

            object->m_map.insert(
                std::pair<FixedString, const char *>(fsKey, json));

            json = skipValue(json);
            json = skipSpaces(json);

            if (*json == '}')
            {
                return;
            }

            json = next(json, ',');
            json = skipSpaces(json);
        }
    }

    assert(false && "Bad json object.");
}

void parse(const char *json, JsonObject *const object)
{
    assert(json != nullptr);
    assert(object != nullptr);

    parseObject(json, object);
}
}
}
