#include "engine/system/input/InputContext.h"

namespace ds
{
InputContext::InputContext(std::string contextName)
{
    m_name = contextName;
}

void InputContext::BindKey(ds_platform::Keyboard::Key key,
                           std::string messageString)
{
    m_keyToMessageStringMap[key] = messageString;
}

std::string InputContext::GetMessageString(ds_platform::Keyboard::Key key) const
{
    std::string messageString = "";

    std::map<ds_platform::Keyboard::Key, std::string>::const_iterator it =
        m_keyToMessageStringMap.find(key);
    if (it != m_keyToMessageStringMap.end())
    {
        messageString = it->second;
    }

    return messageString;
}

const std::string InputContext::GetContextName() const
{
    return m_name;
}
}
