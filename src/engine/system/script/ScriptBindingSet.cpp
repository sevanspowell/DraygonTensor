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

void ScriptBindingSet::AddMethod(const char *methodName, SCRIPT_FN methodPtr)
{
    // Search for another method pair in the set with the same name
    std::vector<std::pair<const char *, SCRIPT_FN>>::iterator it =
        std::find_if(m_methods.begin(), m_methods.end(),
                     [&](const std::pair<const char *, SCRIPT_FN> &namePtr)
                     {
                         return strcmp(methodName, namePtr.first) == 0;
                     });

    // If none exists, add a new method pair
    if (it == m_methods.end())
    {
        m_methods.push_back(
            std::pair<const char *, SCRIPT_FN>(methodName, methodPtr));
    }
    // If one does exist, overwrite it
    else
    {
        *it = std::pair<const char *, SCRIPT_FN>(methodName, methodPtr);
    }
}

const std::pair<const char *, SCRIPT_FN> &
ScriptBindingSet::GetFunctionPair(size_t index) const
{
    assert(index < GetNumFunctions() && index >= 0 &&
           "ScriptBindingSet::GetFunctionPair: Tried to get function pair at "
           "invalid index.");
    return m_functions[index];
}

const std::pair<const char *, SCRIPT_FN> &
ScriptBindingSet::GetMethodPair(size_t index) const
{
    assert(index < GetNumMethods() && index >= 0 &&
           "ScriptBindingSet::GetMethodPair: Tried to get method pair at "
           "invalid index.");
    return m_methods[index];
}

size_t ScriptBindingSet::GetNumFunctions() const
{
    return m_functions.size();
}

size_t ScriptBindingSet::GetNumMethods() const
{
    return m_methods.size();
}
}
