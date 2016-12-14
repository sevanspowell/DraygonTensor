#include <algorithm>
#include <chrono>
#include <fstream>
#include <thread>

#include "engine/Engine.h"
#include "engine/system/scene/TransformComponent.h"

namespace ds
{
Engine::Engine()
{
    m_script = new Script();
    AddSystem(std::unique_ptr<ISystem>(m_script));

    m_platform = new Platform();
    AddSystem(std::unique_ptr<ISystem>(m_platform));
}

void Engine::Start()
{
    if (Init())
    {
        m_running = true;

        while (m_running)
        {
            // Calculate deltaTime
            static double prevSeconds =
                ((Platform *)m_platform)->GetTicks() / 1000.0f;
            double currSeconds = ((Platform *)m_platform)->GetTicks() / 1000.0f;
            float deltaTime = (float)(currSeconds - prevSeconds);

            if (deltaTime >= 0.001)
            {
                prevSeconds = currSeconds;
                Update(deltaTime);
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::microseconds(
                    (long long int)(1000 - 50 + deltaTime * 1000 * 1000)));
            }
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

    if (sharedPtr != nullptr)
    {
        // Does the engine already have this system?
        std::vector<std::shared_ptr<ISystem>>::iterator it =
            std::find(m_systems.begin(), m_systems.end(), sharedPtr);

        // If not, insert into engine
        if (it == m_systems.end())
        {
            m_systems.insert(it, sharedPtr);

            // Also insert into message bus
            m_messageBus.AddSystem(std::weak_ptr<ISystem>(sharedPtr));

            // Register system script bindings
            m_script->RegisterForScriptBinding(sharedPtr->GetName(),
                                             sharedPtr.get());

            sharedPtr->SetComponentStore(&m_componentStore);

            result = true;
        }
    }

    return result;
}

bool Engine::Init()
{
    bool result = true;

    const char *configFilePath = "../assets/config.json";

    std::ifstream configFile(configFilePath, std::ios::binary | std::ios::ate);
    bool didLoad = configFile.good();

    // Read file into buffer
    std::vector<char> configBuffer;
    if (didLoad)
    {
        // Reserve memory
        std::streamsize size = configFile.tellg();
        configBuffer.resize(size);

        // Read file
        configFile.seekg(0, configFile.beg);
        if (configFile.read(&configBuffer[0], size))
        {
            didLoad = true;
        }
        else
        {
            didLoad = false;
            configBuffer.resize(0);
        }
    }
    // Terminate string
    configBuffer.push_back('\0');

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

    result &= m_script->Initialize(&configBuffer[0]);

    // Initialize all systems
    for (auto &system : m_systems)
    {
    	//Skip the script system.
    	if (dynamic_cast<Script*>(system.get())) continue;

        // If any initialization fails, whole process fails.
        result &= system->Initialize(&configBuffer[0]);
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

    uint32_t screenRefreshRate = m_platform->GetRefreshRate();

    auto updateSystem = [](float deltaTime, ISystem* system, uint32_t screenRefreshRate){
    	if (system->getUpdateRate(screenRefreshRate) == 0)
		{
			system->Update(deltaTime);
		}
		else
		{
			unsigned maxUpdates = system->getMaxConsecutiveUpdates();
			float accum = system->getUpdateAccum();
			float updateDT = 1/(float)system->getUpdateRate(screenRefreshRate);
			float boundedDeltaTime = maxUpdates == 0 ? deltaTime : std::min(maxUpdates * updateDT, deltaTime);
			accum += boundedDeltaTime;

			while (accum >= updateDT)
			{
				system->Update(updateDT);
				accum -= updateDT;
			}

			system->setUpdateAccum(accum);
		}
    };

    // Update systems
    for (auto &system : m_systems)
    {
    	//Skip the script system.
    	if (dynamic_cast<Script*>(system.get())) continue;

    	updateSystem(deltaTime, system.get(), screenRefreshRate);
    }

    updateSystem(deltaTime, m_script, screenRefreshRate);
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
    AppendStreamBuffer(&m_messagesInternal, messages);
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
