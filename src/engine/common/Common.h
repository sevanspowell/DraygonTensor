#pragma once

#include <vector>
#include <string>

namespace ds_com
{
/**
 * Take a given string and split it into tokens, using the given seperator
 * character to split string. Tokens are returned in order, with the first
 * element of the vector matching the first token found and so on.
 *
 * @param   separator   char, character to use to split the string. The
 *                      separator is not included in the returned tokens.
 * @param   string      const std::string &, string to split into tokens.
 * @return              std::vector<std::string>, tokens formed from splitting
 *                      string.
 */
std::vector<std::string> TokenizeString(char separator,
                                        const std::string &string);

/**
 * Get the parent directory of a given file.
 *
 * @param   filePath  const std::string &, path to file to get parent directory
 * of.
 * @return            std::string, parent directory of file path.
 */
std::string GetParentDirectory(const std::string &filePath);
}
