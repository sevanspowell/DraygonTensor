#include <algorithm>
#include <cassert>

#include "engine/system/script/ScriptBindingSet.h"

namespace ds
{
void ScriptBindingSet::AddFunction(const char *functionName,
                                   SCRIPT_FN functionPtr)
{
    // Search for another function pair in the set with the same name
    std::vector<std::pair<const char *, SCRIPT_FN>>::iterator it =
        std::find_if(m_functions.begin(), m_functions.end(),
                     [&](const std::pair<const char *, SCRIPT_FN> &namePtr)
                     {
                         return strcmp(functionName, namePtr.first) == 0;
                     });

    // If none exists, add a new function pair
    if (it == m_functions.end())
    {
        m_functions.push_back(
            std::pair<const char *, SCRIPT_FN>(functionName, functionPtr));
    }
    // If one does exist, overwrite it
    else
    {
        *it = std::pair<const char *, SCRIPT_FN>(functionName, functionPtr);
    }
}

const std::pair<const char *, SCRIPT_FN> &
ScriptBindingSet::GetFunctionPair(size_t index) const
{
    assert(index < Size() && index > 0 && "ScriptBindingSet::GetFunctionPair: "
                                          "Tried to get function pair at "
                                          "invalid index.");
    return m_functions[index];
}

size_t ScriptBindingSet::Size() const
{
    return m_functions.size();
}
}
