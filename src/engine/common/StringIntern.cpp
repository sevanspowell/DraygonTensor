#include <cassert>

#include "engine/common/StringIntern.h"

namespace ds
{
StringIntern &StringIntern::Instance()
{
    static StringIntern instance;

    return instance;
}

StringIntern::StringId StringIntern::Intern(std::string string)
{
    StringId id = (StringId)m_strings.size();

    m_strings.push_back(string);

    return id;
}

const std::string &StringIntern::GetString(StringIntern::StringId id) const
{
    assert((uint32_t)id < m_strings.size() && "StringIntern::GetString(): "
                                              "Attempted to get string with "
                                              "invalid StringId.");
    return (m_strings[(uint32_t)id]);
}

StringIntern::StringIntern()
{
    
}
}
