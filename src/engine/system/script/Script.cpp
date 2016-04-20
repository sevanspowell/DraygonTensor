#include <sstream>

#include "engine/Config.h"
#include "engine/common/StringIntern.h"
#include "engine/system/script/Script.h"
#include "engine/message/MessageHelper.h"

namespace ds_lua
{
extern void LoadMathAPI(LuaEnvironment &luaEnv);

extern ds::ScriptBindingSet LoadScriptBindings();
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
        RegisterScriptBindingSet("Script", (ISystem *)this);
        // Loop thru and load script bindings
        for (auto namePtr : m_registeredSystems)
        {
            RegisterScriptBindingSet(namePtr.first, namePtr.second);
        }

        // Register all systems with the lua environment as light userdata
        m_lua.RegisterLightUserData("__Script", (void *)this);
        for (auto namePtr : m_registeredSystems)
        {
            std::stringstream ss;
            ss << "__" << std::string(namePtr.first);
            m_lua.RegisterLightUserData(ss.str().c_str(),
                                        (void *)namePtr.second);
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
    ProcessEvents(&m_messagesReceived);

    if (m_bootScriptLoaded)
    {
        m_lua.CallLuaFunction(
            "update", 1, ds_lua::LuaEnvironment::ArgumentType::ARGUMENT_FLOAT,
            deltaTime);
        m_lua.CallLuaFunction("render", 0);
    }
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

ScriptBindingSet Script::GetScriptBindings() const
{
    return ds_lua::LoadScriptBindings();
}

void Script::RegisterScriptBindings(const char *systemName, ISystem *systemPtr)
{
    m_registeredSystems.push_back(
        std::pair<const char *, ISystem *>(systemName, systemPtr));
}

Entity Script::SpawnPrefab(std::string prefabFile,
                           const ds_math::Vector3 &position)
{
    std::cout << "Prefab spawned: " << prefabFile << std::endl;

    // Create new Entity
    Entity entity = m_entityManager.Create();

    // Open prefab file
    std::stringstream fullPrefabFilePath;
    fullPrefabFilePath << "../assets/" << prefabFile << ".prefab";
    Config prefab;
    if (prefab.LoadFile(fullPrefabFilePath.str()))
    {
        // Get components
        std::vector<std::string> components =
            prefab.GetObjectKeys("components");
        // For each component
        for (auto component : components)
        {
            // Build config access key
            std::stringstream fullComponentKey;
            fullComponentKey << "components"
                             << "." << component;

            std::string componentData =
                prefab.StringifyObject(fullComponentKey.str());

            // Send a component created message
            ds_msg::CreateComponent createComponentMsg;
            createComponentMsg.entity = entity;
            createComponentMsg.componentType =
                StringIntern::Instance().Intern(component);
            createComponentMsg.componentData =
                StringIntern::Instance().Intern(componentData);

            ds_msg::AppendMessage(
                &m_messagesGenerated, ds_msg::MessageType::CreateComponent,
                sizeof(ds_msg::CreateComponent), &createComponentMsg);
        }

        // Finally, send a create transform component message
        ds_msg::CreateComponent transformComponentMsg =
            BuildTransformComponentCreateMessage(
                entity, position, ds_math::Quaternion(),
                ds_math::Vector3(1.0f, 1.0f, 1.0f));
        ds_msg::AppendMessage(
            &m_messagesGenerated, ds_msg::MessageType::CreateComponent,
            sizeof(ds_msg::CreateComponent), &transformComponentMsg);
    }
    else
    {
        std::cerr << "Script::SpawnPrefab: Failed to open prefab file: "
                  << fullPrefabFilePath.str() << std::endl;
        // Invalid entity handle
        m_entityManager.Destroy(entity);
    }

    return entity;
}

void Script::MoveEntity(Entity entity, const ds_math::Vector3 &deltaPosition)
{
    // Send an entity move message
    ds_msg::MoveEntity entityMoveMsg;
    entityMoveMsg.entity = entity;
    entityMoveMsg.deltaPosition = deltaPosition;

    ds_msg::AppendMessage(&m_messagesGenerated, ds_msg::MessageType::MoveEntity,
                          sizeof(ds_msg::MoveEntity), &entityMoveMsg);
}

ds_math::Matrix4 Script::GetWorldTransform(Entity entity) const
{
    ds_math::Matrix4 worldTransform;

    Instance i = m_transformManager.GetInstanceForEntity(entity);

    if (i.IsValid())
    {
        worldTransform = m_transformManager.GetWorldTransform(i);
    }

    return worldTransform;
}

ds_math::Matrix4 Script::GetLocalTransform(Entity entity) const
{
    ds_math::Matrix4 localTransform;

    Instance i = m_transformManager.GetInstanceForEntity(entity);

    if (i.IsValid())
    {
        localTransform = m_transformManager.GetLocalTransform(i);
    }

    return localTransform;
}

void Script::SetLocalTransform(Entity entity, const ds_math::Matrix4 &transform)
{
    ds_msg::SetLocalTransform setLocalMsg;
    setLocalMsg.entity = entity;
    setLocalMsg.localTransform = transform;

    ds_msg::AppendMessage(&m_messagesGenerated,
                          ds_msg::MessageType::SetLocalTransform,
                          sizeof(ds_msg::SetLocalTransform), &setLocalMsg);
}

bool Script::IsNextScriptMessage() const
{
    return (m_toScriptMessages.AvailableBytes() > 0);
}

ds_msg::MessageStream Script::GetNextScriptMessage()
{
    ds_msg::MessageStream msg;

    if (IsNextScriptMessage())
    {
        // Get message header
        ds_msg::MessageHeader header;
        m_toScriptMessages >> header;

        // Insert message header
        msg << header;

        // Get message payload
        void *msgPayload = malloc(header.size);
        m_toScriptMessages.Extract(header.size, msgPayload);

        // Insert message payload
        msg.Insert(header.size, msgPayload);

        // Free payload memory
        free(msgPayload);
    }

    return msg;
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

            // Insert header into messages to be sent to script
            m_toScriptMessages << header;
            // Insert payload into messages to be sent script
            m_toScriptMessages << scriptMsg;

            m_lua.ExecuteString(
                StringIntern::Instance().GetString(scriptMsg.stringId).c_str());
            break;
        case ds_msg::MessageType::MoveForward:
            ds_msg::MoveForward moveForwardMsg;
            (*messages) >> moveForwardMsg;

            // Insert header into messages to be sent to script
            m_toScriptMessages << header;
            // Insert payload into messages to be sent script
            m_toScriptMessages << moveForwardMsg;
            break;
        case ds_msg::MessageType::MoveBackward:
            ds_msg::MoveBackward moveBackwardMsg;
            (*messages) >> moveBackwardMsg;

            // Insert header into messages to be sent to script
            m_toScriptMessages << header;
            // Insert payload into messages to be sent script
            m_toScriptMessages << moveBackwardMsg;
            break;
        case ds_msg::MessageType::StrafeLeft:
            ds_msg::StrafeLeft strafeLeftMsg;
            (*messages) >> strafeLeftMsg;

            // Insert header into messages to be sent to script
            m_toScriptMessages << header;
            // Insert payload into messages to be sent script
            m_toScriptMessages << strafeLeftMsg;
            break;
        case ds_msg::MessageType::StrafeRight:
            ds_msg::StrafeRight strafeRightMsg;
            (*messages) >> strafeRightMsg;

            // Insert header into messages to be sent to script
            m_toScriptMessages << header;
            // Insert payload into messages to be sent script
            m_toScriptMessages << strafeRightMsg;
            break;
        case ds_msg::MessageType::CreateComponent:
        {
            ds_msg::CreateComponent createComponentMsg;
            (*messages) >> createComponentMsg;

            // Lod up component data for component
            Config componentData;
            if (componentData.LoadMemory(StringIntern::Instance().GetString(
                    createComponentMsg.componentData)))
            {
                // Get component type
                std::string componentType = StringIntern::Instance().GetString(
                    createComponentMsg.componentType);
                std::cout << "Create transform component" << std::endl;
                // Create transform component
                if (componentType == "transformComponent")
                {
                    TransformComponentManager::
                        CreateComponentForEntityFromConfig(
                            &m_transformManager, createComponentMsg.entity,
                            componentData);
                }
            }
            break;
        }
        case ds_msg::MessageType::MoveEntity:
        {
            ds_msg::MoveEntity entityMoveMsg;
            (*messages) >> entityMoveMsg;

            // Instance transform =
            //     m_transformManager.GetInstanceForEntity(entityMoveMsg.entity);

            // if (transform.IsValid())
            // {
            //     // Get current transform
            //     const ds_math::Matrix4 &currentTransform =
            //         m_transformManager.GetLocalTransform(transform);
            //     // Translate it
            //     ds_math::Matrix4 newTransform =
            //         currentTransform *
            //         ds_math::Matrix4::CreateTranslationMatrix(
            //             entityMoveMsg.deltaPosition);

            //     // Set transform of entity
            //     m_transformManager.SetLocalTransform(transform, newTransform);
            // }
            break;
        }
        case ds_msg::MessageType::SetLocalTransform:
        {
            ds_msg::SetLocalTransform setLocalMsg;
            (*messages) >> setLocalMsg;

            Instance transform =
                m_transformManager.GetInstanceForEntity(setLocalMsg.entity);

            if (transform.IsValid())
            {
                // Set transform of entity
                m_transformManager.SetLocalTransform(
                    transform, setLocalMsg.localTransform);
            }
            break;
        }
        default:
            messages->Extract(header.size);
            break;
        }
    }
}

void Script::RegisterScriptBindingSet(const char *systemName,
                                      ISystem *systemPtr)
{
    if (systemPtr != nullptr)
    {
        ScriptBindingSet scriptBindings = systemPtr->GetScriptBindings();

        // Allocate enough memory for methods and functions plus one extra for
        // NULL terminator
        luaL_Reg *methods = new luaL_Reg[scriptBindings.GetNumMethods() + 1];
        luaL_Reg *functions =
            new luaL_Reg[scriptBindings.GetNumFunctions() + 1];
        // No special functions in a ScriptBindingSet
        luaL_Reg special[] = {{NULL, NULL}};

        // Fill methods table
        for (unsigned int i = 0; i < scriptBindings.GetNumMethods(); ++i)
        {
            const std::pair<const char *, SCRIPT_FN> &methodPair =
                scriptBindings.GetMethodPair(i);

            methods[i].name = methodPair.first;
            methods[i].func = methodPair.second;
        }
        // Null-terminate methods table
        methods[scriptBindings.GetNumMethods()].name = NULL;
        methods[scriptBindings.GetNumMethods()].func = NULL;

        // Fill functions table
        for (unsigned int i = 0; i < scriptBindings.GetNumFunctions(); ++i)
        {
            const std::pair<const char *, SCRIPT_FN> &functionPair =
                scriptBindings.GetFunctionPair(i);

            functions[i].name = functionPair.first;
            functions[i].func = functionPair.second;
        }
        // Null-terminate functions table
        functions[scriptBindings.GetNumFunctions()].name = NULL;
        functions[scriptBindings.GetNumFunctions()].func = NULL;

        m_lua.RegisterClass(systemName, methods, functions, special);

        // Free method & function arrays
        delete[] methods;
        delete[] functions;
    }
}

ds_msg::CreateComponent Script::BuildTransformComponentCreateMessage(
    Entity entity,
    const ds_math::Vector3 &position,
    const ds_math::Quaternion &orientation,
    const ds_math::Vector3 &scale)
{
    ds_msg::CreateComponent transformComponent;

    // Create float arrays
    std::vector<float> positionArray;
    positionArray.push_back(position.x);
    positionArray.push_back(position.y);
    positionArray.push_back(position.z);

    std::vector<float> orientationArray;
    orientationArray.push_back(orientation.x);
    orientationArray.push_back(orientation.y);
    orientationArray.push_back(orientation.z);
    orientationArray.push_back(orientation.w);

    std::vector<float> scaleArray;
    scaleArray.push_back(scale.x);
    scaleArray.push_back(scale.y);
    scaleArray.push_back(scale.z);

    // Create description of component
    Config configDescription;
    configDescription.AddFloatArray("transformComponent.position",
                                    positionArray);
    configDescription.AddFloatArray("transformComponent.orientation",
                                    orientationArray);
    configDescription.AddFloatArray("transformComponent.scale", scaleArray);

    // Transform description into message
    transformComponent.entity = entity;
    transformComponent.componentType =
        StringIntern::Instance().Intern("transformComponent");
    transformComponent.componentData = StringIntern::Instance().Intern(
        configDescription.StringifyObject("transformComponent"));

    // Return message
    return transformComponent;
}
}
