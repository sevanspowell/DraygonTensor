#include <cassert>

#include "engine/common/StringIntern.h"
#include "engine/system/script/ScriptComponentManager.h"

namespace ds
{
const std::string &ScriptComponentManager::GetScriptPath(Instance i) const
{
    assert(i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
           "Tried to get invalid instance");

    return StringIntern::Instance().GetString(
        m_data.component[i.index].scriptPath);
}

void ScriptComponentManager::SetScriptPath(Instance i, const std::string &path)
{
    assert(i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
           "Tried to set invalid instance");

    m_data.component[i.index].scriptPath =
        StringIntern::Instance().Intern(path);
}

bool ScriptComponentManager::IsInitialized(Instance i) const
{
    assert(i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
           "Tried to get invalid instance");

    return m_data.component[i.index].isInitialized;
}

void ScriptComponentManager::SetInitialized(Instance i, bool isInitialized)
{
    assert(i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
           "Tried to set invalid instance.");

    m_data.component[i.index].isInitialized = isInitialized;
}
}
