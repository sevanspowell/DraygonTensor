#include <sstream>

#include "engine/message/MessageFactory.h"
#include "engine/system/input/Input.h"

namespace ds
{
bool Input::Initialize(const Config &config)
{
    bool result = true;

    InitializeKeyNameToKeyCodeMap();

    // Iterate over config and create input contexts
    std::vector<std::string> contexts = config.GetObjectKeys("Input");

    // For each context
    for (auto contextName : contexts)
    {
        // Create context
        InputContext context(contextName);

        // Construct context config name (name of input context in config file).
        std::stringstream contextConfigName;
        contextConfigName << "Input." << contextName;

        // For each element of context (key bind)
        std::vector<std::string> keys =
            config.GetObjectKeys(contextConfigName.str());
        for (auto key : keys)
        {
            // Construct keybind config name (name of keybind in config file).
            std::stringstream keyBindConfigName;
            keyBindConfigName << contextConfigName.str() << "." << key;

            // Get key bind message string value from config
            std::string messageString;
            if (config.GetString(keyBindConfigName.str(), &messageString))
            {
                // Get key code that matches string key
                Keyboard::Key keycode = Keyboard::Key::Key_Last;
                if (GetKeyCodeForKeyName(key, &keycode))
                {
                    // Bind key code to message string
                    context.BindKey(keycode, messageString);
                }
            }
        }

        // Put contexts in appropriate storage location
        if (contextName == "Default")
        {
            m_inputContextStack.push_back(context);
        }
        else
        {
            m_inputContexts.push_back(context);
        }
    }

    return result;
}

void Input::Update(float deltaTime)
{
    ProcessEvents(&m_messagesReceived);
}

void Input::Shutdown()
{
}

void Input::PostMessages(const ds_msg::MessageStream &messages)
{
    AppendStreamBuffer(m_messagesReceived, messages);
}

ds_msg::MessageStream Input::CollectMessages()
{
    ds_msg::MessageStream tmp = m_messagesGenerated;

    m_messagesGenerated.Clear();

    return tmp;
}

bool Input::GetKeyCodeForKeyName(std::string keyName, Keyboard::Key *key) const
{
    bool result = false;

    std::unordered_map<std::string, Keyboard::Key>::const_iterator it =
        m_keyNameToKeyCodeMap.find(keyName);

    if (it != m_keyNameToKeyCodeMap.end())
    {
        *key = it->second;
        result = true;
    }

    return result;
}

void Input::InitializeKeyNameToKeyCodeMap()
{
    // Hard code key name to key code map
    m_keyNameToKeyCodeMap["a"] = Keyboard::Key::Key_a;
    m_keyNameToKeyCodeMap["b"] = Keyboard::Key::Key_b;
    m_keyNameToKeyCodeMap["c"] = Keyboard::Key::Key_c;
    m_keyNameToKeyCodeMap["d"] = Keyboard::Key::Key_d;
    m_keyNameToKeyCodeMap["e"] = Keyboard::Key::Key_e;
    m_keyNameToKeyCodeMap["f"] = Keyboard::Key::Key_f;
    m_keyNameToKeyCodeMap["g"] = Keyboard::Key::Key_g;
    m_keyNameToKeyCodeMap["h"] = Keyboard::Key::Key_h;
    m_keyNameToKeyCodeMap["i"] = Keyboard::Key::Key_i;
    m_keyNameToKeyCodeMap["j"] = Keyboard::Key::Key_j;
    m_keyNameToKeyCodeMap["k"] = Keyboard::Key::Key_k;
    m_keyNameToKeyCodeMap["l"] = Keyboard::Key::Key_l;
    m_keyNameToKeyCodeMap["m"] = Keyboard::Key::Key_m;
    m_keyNameToKeyCodeMap["n"] = Keyboard::Key::Key_n;
    m_keyNameToKeyCodeMap["o"] = Keyboard::Key::Key_o;
    m_keyNameToKeyCodeMap["p"] = Keyboard::Key::Key_p;
    m_keyNameToKeyCodeMap["q"] = Keyboard::Key::Key_q;
    m_keyNameToKeyCodeMap["r"] = Keyboard::Key::Key_r;
    m_keyNameToKeyCodeMap["s"] = Keyboard::Key::Key_s;
    m_keyNameToKeyCodeMap["t"] = Keyboard::Key::Key_t;
    m_keyNameToKeyCodeMap["u"] = Keyboard::Key::Key_u;
    m_keyNameToKeyCodeMap["v"] = Keyboard::Key::Key_v;
    m_keyNameToKeyCodeMap["w"] = Keyboard::Key::Key_w;
    m_keyNameToKeyCodeMap["x"] = Keyboard::Key::Key_x;
    m_keyNameToKeyCodeMap["y"] = Keyboard::Key::Key_y;
    m_keyNameToKeyCodeMap["z"] = Keyboard::Key::Key_z;
    m_keyNameToKeyCodeMap["`"] = Keyboard::Key::Key_BackQuote;
}

void Input::ProcessEvents(ds_msg::MessageStream *messages)
{
    while (messages->AvailableBytes() != 0)
    {
        // Extract header
        ds_msg::MessageHeader header;
        (*messages) >> header;

        switch (header.type)
        {
        case ds_msg::MessageType::KeyboardEvent:
            ds_msg::KeyboardEvent keyEvent;
            (*messages) >> keyEvent;

            if (keyEvent.state == ds::Keyboard::State::Key_Pressed)
            {
                // Get message string from current input context using key code
                std::string messageString =
                    m_inputContextStack.front().GetMessageString(keyEvent.key);

                // Send string to message factory and get message stream back
                ds_msg::MessageStream stream =
                    ds_msg::MessageFactory::ConstructMessage(messageString);

                // Add message to messages generated by this system, to be
                // broadcast later
                AppendStreamBuffer(m_messagesGenerated, stream);
            }
            break;
        default:
            // Extract message to prevent corrupting message stream
            messages->Extract(header.size);
            break;
        }
    }
}
}
