#include <iostream>
#include <sstream>

#include "engine/Config.h"
#include "engine/common/StringIntern.h"
#include "engine/system/script/Script.h"
#include "engine/message/MessageHelper.h"

namespace ds_lua
{
extern void LoadMathAPI(LuaEnvironment &luaEnv);

extern ds::ScriptBindingSet LoadScriptBindings();

extern const char *scriptSystemLuaName;
}

namespace ds
{
Script::Script()
{
    m_bootScriptLoaded = false;
    m_isFirstUpdate = true;
}

bool Script::Initialize(const Config &config)
{
    bool result = false;

    // Initialize lua environment
    if (m_lua.Init())
    {
        // Load our Lua/C APIs
        ds_lua::LoadMathAPI(m_lua);
        // RegisterScriptBindingSet("Script", (ISystem *)this);
        // Loop thru and load script bindings
        for (auto namePtr : m_registeredSystems)
        {
            assert(namePtr.second != nullptr);
            namePtr.second->RegisterScriptBindings(m_lua.GetState());
        }

        // Register all systems with the lua environment as light userdata
        // m_lua.RegisterLightUserData("__Script", (void *)this);
        // for (auto namePtr : m_registeredSystems)
        // {
        //     std::stringstream ss;
        //     ss << std::string(namePtr.first);
        //     std::cout << ss.str().c_str() << std::endl;
        //     m_lua.RegisterLightUserData(ss.str().c_str(),
        //                                 (void *)namePtr.second);
        // }

        std::string bootScriptPath;

        // Get path to boot script
        config.GetString("Script.bootScript", &bootScriptPath);

        if (bootScriptPath.empty() == false)
        {
            // Execute boot script
            if (m_lua.ExecuteFile(bootScriptPath.c_str()))
            {
                m_bootScriptLoaded = true;

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
    if (m_bootScriptLoaded && m_isFirstUpdate)
    {
        // Call init function in boot script (no arguments)
        m_lua.CallLuaFunction("init", 0);

        m_isFirstUpdate = false;
    }

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
    AppendStreamBuffer(&m_messagesReceived, messages);
}

ds_msg::MessageStream Script::CollectMessages()
{
    ds_msg::MessageStream tmp = m_messagesGenerated;

    m_messagesGenerated.Clear();

    return tmp;
}

const char *Script::GetName() const
{
    return ds_lua::scriptSystemLuaName;
}

void Script::RegisterScriptBindings(lua_State *L)
{
    luabridge::getGlobalNamespace(L)
        .beginClass<Script>("Script")
        .addFunction("GetName", &Script::GetName)
        .endClass();

    luabridge::push(L, (Script *)this);
    lua_setglobal(L, "Script");


}

void Script::RegisterForScriptBinding(const char *systemName,
                                      ISystem *systemPtr)
{
    m_registeredSystems.push_back(
        std::pair<const char *, ISystem *>(systemName, systemPtr));
}

Entity Script::SpawnPrefab(std::string prefabFile,
                           const ds_math::Vector3 &position)
{
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

ds_math::Vector3 Script::GetWorldTranslation(Entity entity) const
{
    ds_math::Vector3 translation;

    Instance i = m_transformManager.GetInstanceForEntity(entity);

    if (i.IsValid())
    {
        translation = m_transformManager.GetWorldTranslation(i);
    }

    return translation;
}

ds_math::Vector3 Script::GetLocalTranslation(Entity entity) const
{
    ds_math::Vector3 translation;

    Instance i = m_transformManager.GetInstanceForEntity(entity);

    if (i.IsValid())
    {
        translation = m_transformManager.GetLocalTranslation(i);
    }

    return translation;
}

void Script::SetLocalTranslation(Entity entity,
                                 const ds_math::Vector3 &translation)
{
    ds_msg::SetLocalTranslation setTranslationMsg;
    setTranslationMsg.entity = entity;
    setTranslationMsg.localTranslation = translation;

    ds_msg::AppendMessage(
        &m_messagesGenerated, ds_msg::MessageType::SetLocalTranslation,
        sizeof(ds_msg::SetLocalTranslation), &setTranslationMsg);
}

ds_math::Vector3 Script::GetWorldScale(Entity entity) const
{
    ds_math::Vector3 scale;

    Instance i = m_transformManager.GetInstanceForEntity(entity);

    if (i.IsValid())
    {
        scale = m_transformManager.GetWorldScale(i);
    }

    return scale;
}

ds_math::Vector3 Script::GetLocalScale(Entity entity) const
{
    ds_math::Vector3 scale;

    Instance i = m_transformManager.GetInstanceForEntity(entity);

    if (i.IsValid())
    {
        scale = m_transformManager.GetLocalScale(i);
    }

    return scale;
}

void Script::SetLocalScale(Entity entity, const ds_math::Vector3 &scale)
{
    ds_msg::SetLocalScale setScaleMsg;
    setScaleMsg.entity = entity;
    setScaleMsg.localScale = scale;

    ds_msg::AppendMessage(&m_messagesGenerated,
                          ds_msg::MessageType::SetLocalScale,
                          sizeof(ds_msg::SetLocalScale), &setScaleMsg);
}

ds_math::Quaternion Script::GetWorldOrientation(Entity entity) const
{
    ds_math::Quaternion orientation;

    Instance i = m_transformManager.GetInstanceForEntity(entity);

    if (i.IsValid())
    {
        orientation = m_transformManager.GetWorldOrientation(i);
    }

    return orientation;
}

ds_math::Quaternion Script::GetLocalOrientation(Entity entity) const
{
    ds_math::Quaternion orientation;

    Instance i = m_transformManager.GetInstanceForEntity(entity);

    if (i.IsValid())
    {
        orientation = m_transformManager.GetLocalOrientation(i);
    }

    return orientation;
}

void Script::SetLocalOrientation(Entity entity,
                                 const ds_math::Quaternion &orientation)
{
    ds_msg::SetLocalOrientation setOrientationMsg;
    setOrientationMsg.entity = entity;
    setOrientationMsg.localOrientation = orientation;

    ds_msg::AppendMessage(
        &m_messagesGenerated, ds_msg::MessageType::SetLocalOrientation,
        sizeof(ds_msg::SetLocalOrientation), &setOrientationMsg);
}

void Script::SetAnimationIndex(Entity entity, int animationIndex)
{
    ds_msg::SetAnimationIndex setAnimationMsg;
    setAnimationMsg.entity = entity;
    setAnimationMsg.animationIndex = animationIndex;

    ds_msg::AppendMessage(&m_messagesGenerated,
                          ds_msg::MessageType::SetAnimationIndex,
                          sizeof(ds_msg::SetAnimationIndex), &setAnimationMsg);
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

void Script::LookAt(Entity entity, const ds_math::Vector3 &target)
{
}

void Script::SetSkyboxMaterial(const std::string &skyboxMaterialPath)
{
    ds_msg::SetSkyboxMaterial setSkyboxMaterialMsg;
    setSkyboxMaterialMsg.skyboxMaterialPath =
        StringIntern::Instance().Intern(skyboxMaterialPath);

    ds_msg::AppendMessage(
        &m_messagesGenerated, ds_msg::MessageType::SetSkyboxMaterial,
        sizeof(ds_msg::SetSkyboxMaterial), &setSkyboxMaterialMsg);
}

Entity Script::CreateGUIPanel(float startX,
                              float startY,
                              float endX,
                              float endY,
                              const std::string &materialPath)
{
    ds_msg::CreatePanel createPanelMsg;
    createPanelMsg.entity = m_entityManager.Create();
    createPanelMsg.startX = startX;
    createPanelMsg.startY = startY;
    createPanelMsg.endX = endX;
    createPanelMsg.endY = endY;
    createPanelMsg.materialPath = StringIntern::Instance().Intern(materialPath);

    ds_msg::AppendMessage(&m_messagesGenerated,
                          ds_msg::MessageType::CreatePanel,
                          sizeof(ds_msg::CreatePanel), &createPanelMsg);

    return createPanelMsg.entity;
}

Entity Script::CreateGUIButton(float startX,
                               float startY,
                               float endX,
                               float endY,
                               const std::string &defaultMaterialPath,
                               const std::string &pressedMaterialPath,
                               const std::string &hoverMaterialPath)
{
    ds_msg::CreateButton createButtonMsg;
    createButtonMsg.entity = m_entityManager.Create();
    createButtonMsg.startX = startX;
    createButtonMsg.startY = startY;
    createButtonMsg.endX = endX;
    createButtonMsg.endY = endY;
    createButtonMsg.defaultMaterialPath =
        StringIntern::Instance().Intern(defaultMaterialPath);
    createButtonMsg.pressedMaterialPath =
        StringIntern::Instance().Intern(pressedMaterialPath);
    createButtonMsg.hoverMaterialPath =
        StringIntern::Instance().Intern(hoverMaterialPath);

    ds_msg::AppendMessage(&m_messagesGenerated,
                          ds_msg::MessageType::CreateButton,
                          sizeof(ds_msg::CreateButton), &createButtonMsg);

    return createButtonMsg.entity;
}

void Script::DestroyEntity(Entity entity)
{
    // Send destroy entity message
    ds_msg::DestroyEntity destroyEntityMsg;
    destroyEntityMsg.entity = entity;

    ds_msg::AppendMessage(&m_messagesGenerated,
                          ds_msg::MessageType::DestroyEntity,
                          sizeof(ds_msg::DestroyEntity), &destroyEntityMsg);
}

void Script::SetMaterialParameterFloat(
    const std::string &materialResourceFilePath,
    const std::string &materialParameterName,
    float data)
{
    std::stringstream fullPath;
    fullPath << "../assets/" << materialResourceFilePath;

    // Send material parameter message
    ds_msg::SetMaterialParameterFloat setParameterMsg;
    setParameterMsg.materialResourceFilePath =
        StringIntern::Instance().Intern(fullPath.str());
    setParameterMsg.parameter =
        StringIntern::Instance().Intern(materialParameterName);
    setParameterMsg.data = data;

    ds_msg::AppendMessage(
        &m_messagesGenerated, ds_msg::MessageType::SetMaterialParameterFloat,
        sizeof(ds_msg::SetMaterialParameterFloat), &setParameterMsg);
}

void Script::SetMaterialParameterInt(
    const std::string &materialResourceFilePath,
    const std::string &materialParameterName,
    int data)
{
    std::stringstream fullPath;
    fullPath << "../assets/" << materialResourceFilePath;

    // Send material parameter message
    ds_msg::SetMaterialParameterInt setParameterMsg;
    setParameterMsg.materialResourceFilePath =
        StringIntern::Instance().Intern(fullPath.str());
    setParameterMsg.parameter =
        StringIntern::Instance().Intern(materialParameterName);
    setParameterMsg.data = data;

    ds_msg::AppendMessage(
        &m_messagesGenerated, ds_msg::MessageType::SetMaterialParameterInt,
        sizeof(ds_msg::SetMaterialParameterInt), &setParameterMsg);
}

void Script::SetMaterialParameterMatrix4(
    const std::string &materialResourceFilePath,
    const std::string &materialParameterName,
    const ds_math::Matrix4 &data)
{
    std::stringstream fullPath;
    fullPath << "../assets/" << materialResourceFilePath;

    // Send material parameter message
    ds_msg::SetMaterialParameterMatrix4 setParameterMsg;
    setParameterMsg.materialResourceFilePath =
        StringIntern::Instance().Intern(fullPath.str());
    setParameterMsg.parameter =
        StringIntern::Instance().Intern(materialParameterName);
    setParameterMsg.data = data;

    ds_msg::AppendMessage(
        &m_messagesGenerated, ds_msg::MessageType::SetMaterialParameterMatrix4,
        sizeof(ds_msg::SetMaterialParameterMatrix4), &setParameterMsg);
}

void Script::SetMaterialParameterVector3(
    const std::string &materialResourceFilePath,
    const std::string &materialParameterName,
    const ds_math::Vector3 &data)
{
    std::stringstream fullPath;
    fullPath << "../assets/" << materialResourceFilePath;

    // Send material parameter message
    ds_msg::SetMaterialParameterVector3 setParameterMsg;
    setParameterMsg.materialResourceFilePath =
        StringIntern::Instance().Intern(fullPath.str());
    setParameterMsg.parameter =
        StringIntern::Instance().Intern(materialParameterName);
    setParameterMsg.data = data;

    ds_msg::AppendMessage(
        &m_messagesGenerated, ds_msg::MessageType::SetMaterialParameterVector3,
        sizeof(ds_msg::SetMaterialParameterVector3), &setParameterMsg);
}

void Script::SetMaterialParameterVector4(
    const std::string &materialResourceFilePath,
    const std::string &materialParameterName,
    const ds_math::Vector4 &data)
{
    std::stringstream fullPath;
    fullPath << "../assets/" << materialResourceFilePath;

    // Send material parameter message
    ds_msg::SetMaterialParameterVector4 setParameterMsg;
    setParameterMsg.materialResourceFilePath =
        StringIntern::Instance().Intern(fullPath.str());
    setParameterMsg.parameter =
        StringIntern::Instance().Intern(materialParameterName);
    setParameterMsg.data = data;

    ds_msg::AppendMessage(
        &m_messagesGenerated, ds_msg::MessageType::SetMaterialParameterVector4,
        sizeof(ds_msg::SetMaterialParameterVector4), &setParameterMsg);
}

void Script::SetMouseLock(bool enableMouseLock)
{
    ds_msg::SetMouseLock setMouseLockMsg;
    setMouseLockMsg.enableMouseLock = enableMouseLock;

    ds_msg::AppendMessage(&m_messagesGenerated,
                          ds_msg::MessageType::SetMouseLock,
                          sizeof(ds_msg::SetMouseLock), &setMouseLockMsg);
}

void Script::Quit()
{
    ds_msg::QuitEvent quitEvent;

    ds_msg::AppendMessage(&m_messagesGenerated, ds_msg::MessageType::QuitEvent,
                          sizeof(ds_msg::QuitEvent), &quitEvent);
}

void Script::SetPause(bool shouldPause)
{
    ds_msg::PauseEvent pauseEvent;
    pauseEvent.shouldPause = shouldPause;

    ds_msg::AppendMessage(&m_messagesGenerated, ds_msg::MessageType::PauseEvent,
                          sizeof(ds_msg::PauseEvent), &pauseEvent);
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
        {
            ds_msg::ScriptInterpret scriptMsg;
            (*messages) >> scriptMsg;

            // Insert header into messages to be sent to script
            m_toScriptMessages << header;
            // Insert payload into messages to be sent to script
            m_toScriptMessages << scriptMsg;

            m_lua.ExecuteString(
                StringIntern::Instance().GetString(scriptMsg.stringId).c_str());
            break;
        }
        case ds_msg::MessageType::ButtonFired:
        {
            ds_msg::ButtonFired buttonFiredMsg;
            (*messages) >> buttonFiredMsg;

            // Insert header into messages to be sent to script
            m_toScriptMessages << header;
            // Insert payload into messages to be sent to script
            m_toScriptMessages << buttonFiredMsg;
            break;
        }
        case ds_msg::MessageType::CreateComponent:
        {
            ds_msg::CreateComponent createComponentMsg;
            (*messages) >> createComponentMsg;

            // Load up component data for component
            Config componentData;
            if (componentData.LoadMemory(StringIntern::Instance().GetString(
                    createComponentMsg.componentData)))
            {
                // Get component type
                std::string componentType = StringIntern::Instance().GetString(
                    createComponentMsg.componentType);
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
        case ds_msg::MessageType::SetLocalTranslation:
        {
            ds_msg::SetLocalTranslation setTranslationMsg;
            (*messages) >> setTranslationMsg;

            // Get component instance of entity to move
            Instance transform = m_transformManager.GetInstanceForEntity(
                setTranslationMsg.entity);

            // If has transform component
            if (transform.IsValid())
            {
                // Set translation of entity
                m_transformManager.SetLocalTranslation(
                    transform, setTranslationMsg.localTranslation);
            }

            break;
        }
        case ds_msg::MessageType::SetLocalOrientation:
        {
            ds_msg::SetLocalOrientation setOrientationMsg;
            (*messages) >> setOrientationMsg;

            // Get component instance of entity to rotate
            Instance transform = m_transformManager.GetInstanceForEntity(
                setOrientationMsg.entity);

            // If has transform component
            if (transform.IsValid())
            {
                // Set orientation of entity
                m_transformManager.SetLocalOrientation(
                    transform, setOrientationMsg.localOrientation);
            }

            break;
        }
        case ds_msg::MessageType::SetLocalScale:
        {
            ds_msg::SetLocalScale setScaleMsg;
            (*messages) >> setScaleMsg;

            // Get component instance of entity to scale
            Instance transform =
                m_transformManager.GetInstanceForEntity(setScaleMsg.entity);

            // If has transform component
            if (transform.IsValid())
            {
                // Set scale of entity
                m_transformManager.SetLocalScale(transform,
                                                 setScaleMsg.localScale);
            }

            break;
        }
        case ds_msg::MessageType::PhysicsCollision:
        {
            ds_msg::PhysicsCollision collisionMsg;
            (*messages) >> collisionMsg;

            // Insert header into messages to be sent to script
            m_toScriptMessages << header;
            // Insert payload into messages to be sent to script
            m_toScriptMessages << collisionMsg;

            break;
        }
        case ds_msg::MessageType::DestroyEntity:
        {
            ds_msg::DestroyEntity destroyEntityMsg;
            (*messages) >> destroyEntityMsg;

            // Remove entity from entity manager
            m_entityManager.Destroy(destroyEntityMsg.entity);
            break;
        }
        case ds_msg::MessageType::KeyboardEvent:
        {
            ds_msg::KeyboardEvent keyboardMsg;
            (*messages) >> keyboardMsg;

            // Insert header into messages to be sent to script
            m_toScriptMessages << header;
            // Insert payload into messages to be sent to script
            m_toScriptMessages << keyboardMsg;

            break;
        }
        default:
            messages->Extract(header.size);
            break;
        }
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
