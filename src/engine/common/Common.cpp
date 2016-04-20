#include "engine/common/Common.h"

#include <sstream>

namespace ds_com
{
std::vector<std::string> TokenizeString(char separator,
                                        const std::string &string)
{
    std::istringstream fullString(string);
    std::vector<std::string> tokens;

    std::string token;
    // Get all characters up to separator, discard separator and place in token
    while (std::getline(fullString, token, separator))
    {
        // Ensure token is not empty
        if (token.size() > 0)
        {
            tokens.push_back(token);
        }
    }

    return tokens;
}

std::string GetParentDirectory(const std::string &filePath)
{
    return (filePath.substr(0, filePath.find_last_of("/\\") + 1)); 
}
}
