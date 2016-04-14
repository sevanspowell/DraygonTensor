#include "engine/Engine.h"

namespace ds
{
void Engine::Start()
{
    if (Init())
    {
        m_running = true;

        while (m_running)
        {
            Update(0.1f);
        }

        Shutdown();
    }
    else
    {
        Shutdown();
    }
}

bool Engine::AddSystem(std::unique_ptr<ISystem> system)
{
    bool result = false;

    // Make unique ptr shared
    std::shared_ptr<ISystem> sharedPtr =
        std::shared_ptr<ISystem>(std::move(system));

    // Does the engine already have this system?
    std::vector<std::shared_ptr<ISystem>>::const_iterator it =
        std::find(m_systems.begin(), m_systems.end(), sharedPtr);

    // If not, insert into engine
    if (it == m_systems.end())
    {
        m_systems.insert(it, sharedPtr);

        // Also insert into message bus
        m_messageBus.AddSystem(std::weak_ptr<ISystem>(sharedPtr));

        result = true;
    }

    return result;
}

bool Engine::Init()
{
    bool result = true;

    Config config;
    const char *configFilePath = "../assets/config.json";

    bool didLoad = config.LoadFile(configFilePath);

    // Send a message about whether or not the config file loaded
    ds_msg::MessageHeader header;
    header.type = ds_msg::MessageType::ConfigLoad;
    header.size = sizeof(ds_msg::ConfigLoad);

    ds_msg::ConfigLoad configLoadMsg;
    configLoadMsg.configFilePath = configFilePath;
    configLoadMsg.didLoad = didLoad;

    ds_msg::MessageStream stream;
    stream << header << configLoadMsg;
    PostMessages(stream);

    // Initialize all systems
    for (auto &system : m_systems)
    {
        // If any initialization fails, whole process fails.
        result &= system->Initialize(config);
    }

    return result;
}

void Engine::Update(float deltaTime)
{
    // Give any engine messages to the message bus
    m_messageBus.PostMessages(CollectMessages());

    // Collect any messages generated last tick (or in Init method in case of
    // first tick)
    m_messageBus.CollectAllMessages();

    // Get any generated messages and give to the engine.
    PostMessages(m_messageBus.CollectMessages());

    // Broadcast and purge messages
    m_messageBus.BroadcastAllMessages();

    // Let engine process it's messages
    ProcessMessages(&m_messagesInternal);

    // Update systems
    for (auto &system : m_systems)
    {
        system->Update(deltaTime);
    }
}

void Engine::Shutdown()
{
    // Shutdown systems in reverse order
    for (auto it = m_systems.rbegin(); it != m_systems.rend(); ++it)
    {
        (*it)->Shutdown();
    }
}

void Engine::PostMessages(const ds_msg::MessageStream &messages)
{
    AppendStreamBuffer(m_messagesInternal, messages);
}

ds_msg::MessageStream Engine::CollectMessages()
{
    ds_msg::MessageStream tmp = m_messagesInternal;

    m_messagesInternal.Clear();

    return tmp;
}

void Engine::ProcessMessages(ds_msg::MessageStream *messages)
{
    while (messages->AvailableBytes() != 0)
    {
        // Extract header
        ds_msg::MessageHeader header;
        (*messages) >> header;

        switch (header.type)
        {
        case ds_msg::MessageType::QuitEvent:
            ds_msg::QuitEvent quitEvent;
            (*messages) >> quitEvent;

            // Stop engine, clean up memory
            m_running = false;
            break;
        default:
            messages->Extract(header.size);

            break;
        }
    }
}
}
