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

#include <cstdint>
#include <string>

#include "engine/json/JsonTypes.h"

namespace ds
{
/**
 * Functions to parse JSON-encoded data.
 */
namespace json
{
/**
 * Returns the data type of the JSON string.
 *
 * @param   json   const char *, JSON string.
 * @return         JsonValueType::Enum, data type of the given JSON string.
 */
JsonValueType::Enum type(const char *json);

/**
 * Parses the JSON-encoded data and puts it in a string.
 *
 * @param   json     const char *, JSON string.
 * @param   string   std::string *const, string out.
 */
void parseString(const char *json, std::string *const string);

/**
 * Parses the JSON-encoded data and returns it as a double.
 *
 * @param   json   const char *, JSON string.
 * @return         double, parsed number.
 */
double parseNumber(const char *json);

/**
 * Parses the JSON-encoded data and returns it as an int.
 *
 * @param   json   const char *, JSON string.
 * @return         int32_t, parsed int.
 */
int32_t parseInt(const char *json);

/**
 * Parses the JSON-encoded data and returns it as a float.
 *
 * @param   json   const char *, JSON string.
 * @return         float, parsed float.
 */
float parseFloat(const char *json);

/**
 * Parses the JSON-encoded data and returns it as a bool.
 *
 * @param   json   const char *, JSON string.
 * @return         bool, parsed bool.
 */
bool parseBool(const char *json);

/**
 * Parses the JSON-encoded data and puts it into the JsonArray as pointers to
 * the corresponding items in the JSON string.
 *
 * @param   json    const char *, JSON string.
 * @param   array   JsonArray *const, json array out.
 */
void parseArray(const char *json, JsonArray *const array);

/**
 * Parses the JSON-encoded data and puts it into the JsonObject as a map from
 * key to pointer to the corresponding value into the JSON string.
 *
 * @param   json     const char *, JSON string.
 * @param   object   JsonObject *const, json object out.
 */
void parseObject(const char *json, JsonObject *const object);

/**
 * Parses the JSON-encoded data from the given NULL-terminated string and puts
 * it in a JsonObject.
 *
 * @param   json     const char *, JSON string.
 * @param   object   JsonObject *const, json object out.
 */
void parse(const char *json, JsonObject *const object);
}
}
