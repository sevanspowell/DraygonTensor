#include "engine/Config.h"
#include "engine/common/StringIntern.h"
#include "engine/system/script/Script.h"

namespace ds
{
Script::Script()
{
    m_bootScriptLoaded = false;
}

bool Script::Initialize(const Config &config)
{
    bool result = false;

    // Initialize lua environment
    if (m_lua.Init())
    {
        std::string bootScriptPath;

        // Push pointer to this class onto lua stack
        m_lua.PushLightUserData(this);

        // Get path to boot script
        config.GetString("Script.bootScript", &bootScriptPath);

        if (bootScriptPath.empty() == false)
        {
            // Execute boot script
            if (m_lua.ExecuteFile(bootScriptPath.c_str()))
            {
                m_bootScriptLoaded = true;

                // Call init function in boot script (no arguments)
                m_lua.CallLuaFunction("init", 0);

                result = true;
            }
        }
        // If no boot script, no need to crash (?)
        else
        {
            result = true;
        }
    }

    // Send system init message
    ds_msg::MessageHeader header;
    header.type = ds_msg::MessageType::SystemInit;
    header.size = sizeof(ds_msg::SystemInit);

    ds_msg::SystemInit initMsg;
    initMsg.systemName = "Script";

    m_messagesGenerated << header << initMsg;

    return result;
}

void Script::Update(float deltaTime)
{
    if (m_bootScriptLoaded)
    {
        m_lua.CallLuaFunction(
            "update", 1, ds_lua::LuaEnvironment::ArgumentType::ARGUMENT_FLOAT,
            deltaTime);
        m_lua.CallLuaFunction("render", 0);
    }

    ProcessEvents(&m_messagesReceived);
}

void Script::Shutdown()
{
    if (m_bootScriptLoaded)
    {
        m_lua.CallLuaFunction("shutdown", 0);
    }

    m_lua.Shutdown();
}

void Script::PostMessages(const ds_msg::MessageStream &messages)
{
    AppendStreamBuffer(m_messagesReceived, messages);
}

ds_msg::MessageStream Script::CollectMessages()
{
    ds_msg::MessageStream tmp = m_messagesGenerated;

    m_messagesGenerated.Clear();

    return tmp;
}

void Script::ProcessEvents(ds_msg::MessageStream *messages)
{
    while (messages->AvailableBytes() != 0)
    {
        // Extract header
        ds_msg::MessageHeader header;
        (*messages) >> header;

        switch (header.type)
        {
        case ds_msg::MessageType::ScriptInterpret:
            ds_msg::ScriptInterpret scriptMsg;
            (*messages) >> scriptMsg;

            m_lua.ExecuteString(
                StringIntern::Instance().GetString(scriptMsg.stringId).c_str());
            break;
        default:
            messages->Extract(header.size);
            break;
        }
    }
}
}
