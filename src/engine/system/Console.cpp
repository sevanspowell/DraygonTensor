#include "engine/common/StringIntern.h"
#include "engine/system/Console.h"
#include "engine/message/MessageHelper.h"

namespace ds
{
bool Console::Initialize(const Config &config)
{
    bool result = true;

    m_isConsoleOpen = false;

    // Send system init message
    ds_msg::SystemInit initMsg;
    initMsg.systemName = "Console";

    ds_msg::AppendMessage(&m_messagesGenerated, ds_msg::MessageType::SystemInit,
                          sizeof(ds_msg::SystemInit), &initMsg);

    return result;
}

void Console::Update(float deltaTime)
{
    ProcessEvents(&m_messagesReceived);

    // Flush console output
    Flush();

    // TESTING ONLY
    // ds_msg::MessageHeader header;
    // header.type = ds_msg::MessageType::ScriptInterpret;
    // header.size = sizeof(ds_msg::ScriptInterpret);

    // ds_msg::ScriptInterpret scriptMsg;
    // scriptMsg.string = "print(\"Hello World\")";
    // m_messagesGenerated << header << scriptMsg;
}

void Console::Shutdown()
{
}

void Console::PostMessages(const ds_msg::MessageStream &messages)
{
    AppendStreamBuffer(&m_messagesReceived, messages);
}

ds_msg::MessageStream Console::CollectMessages()
{
    ds_msg::MessageStream tmp = m_messagesGenerated;

    m_messagesGenerated.Clear();

    return tmp;
}

void Console::Flush()
{
    std::cout << m_buffer.str();

    // Clean buffer
    m_buffer.str(std::string());
}

void Console::ProcessEvents(ds_msg::MessageStream *messages)
{
    while (messages->AvailableBytes() != 0)
    {
        // Extract header
        ds_msg::MessageHeader header;
        (*messages) >> header;

        switch (header.type)
        {
        case ds_msg::MessageType::SystemInit:
            ds_msg::SystemInit initMsg;
            (*messages) >> initMsg;

            m_buffer << "Console out: " << initMsg.systemName
                     << " system initialized." << std::endl;
            break;
        case ds_msg::MessageType::ConfigLoad:
            ds_msg::ConfigLoad configLoadMsg;
            (*messages) >> configLoadMsg;

            if (configLoadMsg.didLoad)
            {
                m_buffer << "Console out: "
                         << "Loaded config file: \""
                         << configLoadMsg.configFilePath << "\"" << std::endl;
            }
            else
            {
                m_buffer << "Console out: "
                         << "Failed to load config file: \""
                         << configLoadMsg.configFilePath << "\"" << std::endl;
            }
            break;
        case ds_msg::MessageType::ScriptInterpret:
            ds_msg::ScriptInterpret scriptMsg;
            (*messages) >> scriptMsg;

            m_buffer << "Console out: "
                     << "Executed script command: "
                     << StringIntern::Instance().GetString(scriptMsg.stringId)
                     << std::endl;
            break;
        case ds_msg::MessageType::KeyboardEvent:
            ds_msg::KeyboardEvent keyEvent;
            (*messages) >> keyEvent;

            if (keyEvent.state == ds_platform::Keyboard::State::Key_Pressed)
            {
                // m_buffer << "Console out: "
                //          << "Key pressed: '"
                //          << ds_platform::Keyboard::PrintKey(keyEvent.key)
                //          << "'." << std::endl;

                if (keyEvent.key == ds_platform::Keyboard::Key::Key_Backspace &&
                    m_inputText.length() > 0)
                {
                    m_inputText.pop_back();
                }
                else if (keyEvent.key ==
                             ds_platform::Keyboard::Key::Key_Return &&
                         m_isConsoleOpen)
                {
                    // Send message to Script system to execute Lua code
                    // inputted into console.
                    ds_msg::ScriptInterpret scriptMsg;
                    scriptMsg.stringId =
                        StringIntern::Instance().Intern(m_inputText);

                    ds_msg::AppendMessage(&m_messagesGenerated,
                                          ds_msg::MessageType::ScriptInterpret,
                                          sizeof(ds_msg::ScriptInterpret),
                                          &scriptMsg);

                    // Clear input text
                    m_inputText.clear();
                }
            }
            else if (keyEvent.state ==
                     ds_platform::Keyboard::State::Key_Released)
            {
                // m_buffer << "Console out: "
                //          << "Key released: '"
                //          << ds_platform::Keyboard::PrintKey(keyEvent.key)
                //          << "'." << std::endl;
            }
            break;
        case ds_msg::MessageType::QuitEvent:
            ds_msg::QuitEvent quitEvent;
            (*messages) >> quitEvent;

            m_buffer << "Console out: "
                     << "Quit event." << std::endl;
            break;
        case ds_msg::MessageType::ConsoleToggle:
            ds_msg::ConsoleToggle consoleToggle;
            (*messages) >> consoleToggle;

            m_buffer << "Console out:"
                     << "Console toggled." << std::endl;

            if (m_inputText.length() > 0)
            {
                // Lop off console toggle key character from input text
                m_inputText.pop_back();
            }

            // Open or close console
            m_isConsoleOpen = !m_isConsoleOpen;
            break;
        case ds_msg::MessageType::TextInput:
            ds_msg::TextInput textInput;
            (*messages) >> textInput;

            m_buffer << "Console out: "
                     << "Text input: "
                     << StringIntern::Instance().GetString(textInput.stringId)
                     << std::endl;

            // If console is open, accept text input.
            if (m_isConsoleOpen == true)
            {
                m_inputText +=
                    StringIntern::Instance().GetString(textInput.stringId);
                std::cout << m_inputText << std::endl;
            }
            break;
        case ds_msg::MessageType::GraphicsContextCreated:
            ds_msg::GraphicsContextCreated gfxContext;
            (*messages) >> gfxContext;

            if (gfxContext.contextInfo.type ==
                ds_platform::GraphicsContext::ContextType::OpenGL)
            {
                m_buffer << "Console out: ";
                m_buffer << "OpenGL context created version "
                         << gfxContext.contextInfo.openGL.majorVersion << "."
                         << gfxContext.contextInfo.openGL.minorVersion;
                if (gfxContext.contextInfo.openGL.profile ==
                    ds_platform::GraphicsContext::ContextProfileOpenGL::
                        Compatability)
                {
                    m_buffer << " (compatability profile)." << std::endl;
                }
                else
                {
                    m_buffer << " (core profile)." << std::endl;
                }
            }
            break;
        case ds_msg::MessageType::CreateComponent:
            ds_msg::CreateComponent createComponentMsg;
            (*messages) >> createComponentMsg;

            // Print console msg
            m_buffer << "Console out: Component created: Entity: "
                     << createComponentMsg.entity.id << " ComponentType: "
                     << StringIntern::Instance().GetString(
                            createComponentMsg.componentType)
                     << std::endl;
            break;
        case ds_msg::MessageType::SetLocalTranslation:
        {
            ds_msg::SetLocalTranslation setLocalTranslationMsg;
            (*messages) >> setLocalTranslationMsg;

            // m_buffer << "Console out: Set local translation of Entity "
            //          << setLocalTranslationMsg.entity.id
            //          << " to: " << setLocalTranslationMsg.localTranslation
            //          << std::endl;
            break;
        }
        case ds_msg::MessageType::SetLocalOrientation:
        {
            ds_msg::SetLocalOrientation setLocalOrientationMsg;
            (*messages) >> setLocalOrientationMsg;

            // m_buffer << "Console out: Set local orientation of Entity "
            //          << setLocalOrientationMsg.entity.id
            //          << " to: " << setLocalOrientationMsg.localOrientation
            //          << std::endl;
            break;
        }
        case ds_msg::MessageType::SetLocalScale:
        {
            ds_msg::SetLocalScale setLocalScaleMsg;
            (*messages) >> setLocalScaleMsg;

            // m_buffer << "Console out: Set local scale of Entity "
            //          << setLocalScaleMsg.entity.id
            //          << " to: " << setLocalScaleMsg.localScale << std::endl;
            break;
        }
        case ds_msg::MessageType::SetAnimationIndex:
        {
            ds_msg::SetAnimationIndex setAnimationMsg;
            (*messages) >> setAnimationMsg;

            m_buffer << "Console out: animation of entity id "
                     << setAnimationMsg.entity.id << " changed to index "
                     << setAnimationMsg.animationIndex << std::endl;
            break;
        }
        case ds_msg::MessageType::SetSkyboxMaterial:
        {
            ds_msg::SetSkyboxMaterial setSkyboxMaterialMsg;
            (*messages) >> setSkyboxMaterialMsg;

            m_buffer << "Console out: set skybox material to: "
                     << StringIntern::Instance().GetString(
                            setSkyboxMaterialMsg.skyboxMaterialPath)
                     << std::endl;
            break;
        }
        case ds_msg::MessageType::MouseMotion:
        {
            ds_msg::MouseMotion mouseMotionEvent;
            (*messages) >> mouseMotionEvent;

            // m_buffer << "Console out: mouse now at position "
            //          << mouseMotionEvent.x << ", " << mouseMotionEvent.y
            //          << " with buttons pressed Left: "
            //          << mouseMotionEvent.button.left
            //          << " Middle: " << mouseMotionEvent.button.middle
            //          << " Right: " << mouseMotionEvent.button.right << "."
            //          << std::endl;
            break;
        }
        case ds_msg::MessageType::MouseButton:
        {
            ds_msg::MouseButton mouseButtonEvent;
            (*messages) >> mouseButtonEvent;

            // m_buffer << "Console out: mouse button state changed, current "
            //             "state: Left: "
            //          << mouseButtonEvent.button.left
            //          << " Middle: " << mouseButtonEvent.button.middle
            //          << " Right: " << mouseButtonEvent.button.right << " at
            //          ("
            //          << mouseButtonEvent.x << ", " << mouseButtonEvent.y <<
            //          ")."
            //          << std::endl;
            break;
        }
        case ds_msg::MessageType::CreatePanel:
        {
            ds_msg::CreatePanel createPanelMsg;
            (*messages) >> createPanelMsg;

            m_buffer << "Console out: GUI panel created from ("
                     << createPanelMsg.startX << ", " << createPanelMsg.startY
                     << ") to (" << createPanelMsg.endX << ", "
                     << createPanelMsg.endY << ") with material "
                     << StringIntern::Instance().GetString(
                            createPanelMsg.materialPath)
                     << std::endl;
            break;
        }
        case ds_msg::MessageType::CreateButton:
        {
            ds_msg::CreateButton createButtonMsg;
            (*messages) >> createButtonMsg;

            m_buffer << "Console out: GUI button created from ("
                     << createButtonMsg.startX << ", " << createButtonMsg.startY
                     << ") to (" << createButtonMsg.endX << ", "
                     << createButtonMsg.endY << ") with materials: "
                     << StringIntern::Instance().GetString(
                            createButtonMsg.defaultMaterialPath)
                     << ", "
                     << StringIntern::Instance().GetString(
                            createButtonMsg.pressedMaterialPath)
                     << ", "
                     << StringIntern::Instance().GetString(
                            createButtonMsg.pressedMaterialPath)
                     << std::endl;
            break;
        }
        case ds_msg::MessageType::ButtonFired:
        {
            ds_msg::ButtonFired buttonFireMsg;
            (*messages) >> buttonFireMsg;

            m_buffer << "Console out: button entity id: "
                     << buttonFireMsg.entity.id << " fired." << std::endl;
            break;
        }
        case ds_msg::MessageType::PhysicsCollision:
        {
            ds_msg::PhysicsCollision collisionMsg;
            (*messages) >> collisionMsg;

            // m_buffer << "Console out: Collision between entity A: "
            //          << collisionMsg.entityA.id
            //          << " and entity B: " << collisionMsg.entityB.id
            //          << ". Point on A (world): " <<
            //          collisionMsg.pointWorldOnA
            //          << ", point on B (world): " <<
            //          collisionMsg.pointWorldOnB
            //          << ", normal on B (world): " <<
            //          collisionMsg.normalWorldOnB
            //          << std::endl;
            break;
        }
        case ds_msg::MessageType::DestroyEntity:
        {
            ds_msg::DestroyEntity destroyEntityMsg;
            (*messages) >> destroyEntityMsg;

            m_buffer << "Console out: entity id: " << destroyEntityMsg.entity.id
                     << " flagged for destruction." << std::endl;
            break;
        }
        case ds_msg::MessageType::SetMaterialParameterFloat:
        {
            ds_msg::SetMaterialParameterFloat setParameterMsg;
            (*messages) >> setParameterMsg;

            m_buffer << "Console out: set float material parameter '"
                     << StringIntern::Instance().GetString(
                            setParameterMsg.parameter)
                     << "' from material resource '"
                     << StringIntern::Instance().GetString(
                            setParameterMsg.materialResourceFilePath)
                     << "' to value: " << setParameterMsg.data << std::endl;
            break;
        }
        case ds_msg::MessageType::SetMaterialParameterInt:
        {
            ds_msg::SetMaterialParameterInt setParameterMsg;
            (*messages) >> setParameterMsg;

            m_buffer << "Console out: set int material parameter '"
                     << StringIntern::Instance().GetString(
                            setParameterMsg.parameter)
                     << "' from material resource '"
                     << StringIntern::Instance().GetString(
                            setParameterMsg.materialResourceFilePath)
                     << "' to value: " << setParameterMsg.data << std::endl;
            break;
        }
        case ds_msg::MessageType::SetMaterialParameterMatrix4:
        {
            ds_msg::SetMaterialParameterMatrix4 setParameterMsg;
            (*messages) >> setParameterMsg;

            m_buffer << "Console out: set Matrix4 material parameter '"
                     << StringIntern::Instance().GetString(
                            setParameterMsg.parameter)
                     << "' from material resource '"
                     << StringIntern::Instance().GetString(
                            setParameterMsg.materialResourceFilePath)
                     << "' to value: " << setParameterMsg.data << std::endl;
            break;
        }
        case ds_msg::MessageType::SetMaterialParameterVector4:
        {
            ds_msg::SetMaterialParameterVector4 setParameterMsg;
            (*messages) >> setParameterMsg;

            m_buffer << "Console out: set Vector4 material parameter '"
                     << StringIntern::Instance().GetString(
                            setParameterMsg.parameter)
                     << "' from material resource '"
                     << StringIntern::Instance().GetString(
                            setParameterMsg.materialResourceFilePath)
                     << "' to value: " << setParameterMsg.data << std::endl;
            break;
        }
        case ds_msg::MessageType::SetMaterialParameterVector3:
        {
            ds_msg::SetMaterialParameterVector3 setParameterMsg;
            (*messages) >> setParameterMsg;

            m_buffer << "Console out: set Vector3 material parameter '"
                     << StringIntern::Instance().GetString(
                            setParameterMsg.parameter)
                     << "' from material resource '"
                     << StringIntern::Instance().GetString(
                            setParameterMsg.materialResourceFilePath)
                     << "' to value: " << setParameterMsg.data << std::endl;
            break;
        }
        default:
            // Always extract the payload
            messages->Extract(header.size);

            // Console should handle all message types, so crash if not handled.
            assert(false && "Console::ProcessEvents: Unhandled message type!");
            break;
        }
    }
}
}
