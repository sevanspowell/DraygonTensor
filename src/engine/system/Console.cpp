#include "engine/common/StringIntern.h"
#include "engine/system/Console.h"

namespace ds
{
bool Console::Initialize(const Config &config)
{
    bool result = true;

    m_isConsoleOpen = false;

    // Send system init message
    ds_msg::MessageHeader header;
    header.type = ds_msg::MessageType::SystemInit;
    header.size = sizeof(ds_msg::SystemInit);

    ds_msg::SystemInit initMsg;
    initMsg.systemName = "Console";

    m_messagesGenerated << header << initMsg;

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
    AppendStreamBuffer(m_messagesReceived, messages);
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
                m_buffer << "Console out: "
                         << "Key pressed: '"
                         << ds_platform::Keyboard::PrintKey(keyEvent.key)
                         << "'." << std::endl;

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

                    ds_msg::MessageHeader header;
                    header.type = ds_msg::MessageType::ScriptInterpret;
                    header.size = sizeof(ds_msg::ScriptInterpret);

                    m_messagesGenerated << header << scriptMsg;

                    // Clear input text
                    m_inputText.clear();
                }
            }
            else if (keyEvent.state ==
                     ds_platform::Keyboard::State::Key_Released)
            {
                m_buffer << "Console out: "
                         << "Key released: '"
                         << ds_platform::Keyboard::PrintKey(keyEvent.key)
                         << "'." << std::endl;
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
        default:
            messages->Extract(header.size);

            // Console should handle all message types, so crash if not handled.
            assert(false && "Console::ProcessEvents: Unhandled message type!");
            break;
        }
    }
}
}
