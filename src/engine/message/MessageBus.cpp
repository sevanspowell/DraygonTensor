#include <algorithm>

#include "engine/message/MessageBus.h"

namespace ds
{
bool MessageBus::AddSystem(std::weak_ptr<ISystem> system)
{
    bool result = false;

    // Does the message bus already have this system?
    std::vector<std::weak_ptr<ISystem>>::const_iterator it =
        std::find_if(m_systems.begin(), m_systems.end(),
                     [&](const std::weak_ptr<ISystem> &busSystem)
                     {
                         return busSystem.lock() == system.lock();
                     });

    // If not, insert into engine
    if (it == m_systems.end())
    {
        m_systems.insert(it, system);
        result = true;
    }

    return result;
}

void MessageBus::CollectAllMessages()
{
    for (auto &system : m_systems)
    {
        // Convert weak pointer to shared pointer temporarily
        std::shared_ptr<ISystem> systemPtr = system.lock();

        if (systemPtr)
        {
            AppendStreamBuffer(&m_messageStoreTemp,
                               systemPtr->CollectMessages());
        }
    }
}

void MessageBus::BroadcastAllMessages()
{
    for (auto &system : m_systems)
    {
        // Convert weak pointer to shared pointer temporarily
        std::shared_ptr<ISystem> systemPtr = system.lock();

        if (systemPtr)
        {
            systemPtr->PostMessages(m_messageStoreTemp);
        }
    }

    m_messageStoreTemp.Clear();
}

void MessageBus::PostMessages(const ds_msg::MessageStream &messages)
{
    AppendStreamBuffer(&m_messageStoreTemp, messages);
}

ds_msg::MessageStream MessageBus::CollectMessages()
{
    return m_messageStoreTemp;
}
}
