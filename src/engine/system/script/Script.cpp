#include "engine/Config.h"
#include "engine/common/StringIntern.h"
#include "engine/system/script/Script.h"
#include "engine/message/MessageHelper.h"

namespace ds_lua
{
extern void LoadMathAPI(LuaEnvironment &luaEnv);
}

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
        // Load our Lua/C APIs
        ds_lua::LoadMathAPI(m_lua);
        // Loop thru and load script bindings
        for (auto namePtr : m_registeredSystems)
        {
            RegisterScriptBindings(namePtr.second);
        }

        // Register all systems with the lua environment as light userdata
        m_lua.RegisterLightUserData("Script", (void *)this);
        for (auto namePtr : m_registeredSystems)
        {
            m_lua.RegisterLightUserData(namePtr.first, (void *)namePtr.second);
        }

        std::string bootScriptPath;

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
    ds_msg::SystemInit initMsg;
    initMsg.systemName = "Script";

    ds_msg::AppendMessage(&m_messagesGenerated, ds_msg::MessageType::SystemInit,
                          sizeof(ds_msg::SystemInit), &initMsg);

    // We don't need these pointers anymore
    m_registeredSystems.clear();

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

void Script::RegisterScriptBindings(const char *systemName, ISystem *systemPtr)
{
    m_registeredSystems.push_back(
        std::pair<const char *, ISystem *>(systemName, systemPtr));
}

void Script::SpawnUnit(std::string unitFile)
{
    std::cout << "Unit spawned: " << unitFile << std::endl;
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

void Script::RegisterScriptBindings(ISystem *systemPtr)
{
    //     if (systemPtr != nullptr)
    //     {
    //         std::vector<std::pair<const char *, SCRIPT_FN>> metaMethods;
    //         std::vector<std::pair<const char *, SCRIPT_FN>> methods;

    //         ScriptBindingSet scriptBindings = systemPtr->GetScriptBindings();
    //         for (unsigned int i = 0; i < scriptBindings.size(); ++i)
    //         {
    //             const std::pair<const char *, SCRIPT_FN> &functionPair =
    //                 scriptBinding.GetFunctionPair(i);

    //             // If script binding is a meta method, add to metaMethods
    //             // Else add to methods
    //         }
    //     }
}
}
