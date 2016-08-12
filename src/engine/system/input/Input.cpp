#include <sstream>

#include "engine/message/MessageFactory.h"
#include "engine/system/input/Input.h"

#include <SDL2/SDL.h>

namespace ds_lua
{
extern ds::ScriptBindingSet LoadInputScriptBindings();

extern const char *inputSystemLuaName;
}

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
                ds_platform::Keyboard::Key keycode =
                    ds_platform::Keyboard::Key::Key_Last;
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

    m_messagesReceived.Clear();
}

void Input::Shutdown()
{
}

void Input::PostMessages(const ds_msg::MessageStream &messages)
{
    AppendStreamBuffer(&m_messagesReceived, messages);
}

ds_msg::MessageStream Input::CollectMessages()
{
    ds_msg::MessageStream tmp = m_messagesGenerated;

    m_messagesGenerated.Clear();

    return tmp;
}

const char *Input::GetName() const
{
    return ds_lua::inputSystemLuaName;
}

ScriptBindingSet Input::GetScriptBindings() const
{
    return ds_lua::LoadInputScriptBindings();
}

bool Input::IsKeyPressed(const std::string &keyName) const
{
    bool isPressed = false;

    ds_platform::Keyboard::Key key;
    // Get key code for name
    if (GetKeyCodeForKeyName(keyName, &key))
    {
        // Get scan code for key code
        SDL_Scancode scanCode = SDL_GetScancodeFromKey((SDL_Keycode)key);

        // Lookup in array
        const unsigned char *keys = SDL_GetKeyboardState(NULL);

        isPressed = keys[scanCode];
    }

    return isPressed;
}

void Input::GetMouseDeltaXY(int *xDelta, int *yDelta) const
{
    SDL_GetRelativeMouseState(xDelta, yDelta);
}

bool Input::GetKeyCodeForKeyName(std::string keyName,
                                 ds_platform::Keyboard::Key *key) const
{
    bool result = false;

    std::unordered_map<std::string, ds_platform::Keyboard::Key>::const_iterator
        it = m_keyNameToKeyCodeMap.find(keyName);

    if (it != m_keyNameToKeyCodeMap.end())
    {
        *key = it->second;
        result = true;
    }

    return result;
}

void Input::InitializeKeyNameToKeyCodeMap()
{
    using namespace ds_platform;

    // Hard code key name to key code map
    // Use Keyboard::PrintKey to get string representing key
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Backspace)] =
        Keyboard::Key::Key_Backspace;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Tab)] =
        Keyboard::Key::Key_Tab;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Return)] =
        Keyboard::Key::Key_Return;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Escape)] =
        Keyboard::Key::Key_Escape;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Space)] =
        Keyboard::Key::Key_Space;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Exclamation)] =
        Keyboard::Key::Key_Exclamation;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_DblQuote)] =
        Keyboard::Key::Key_DblQuote;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Hash)] =
        Keyboard::Key::Key_Hash;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Dollar)] =
        Keyboard::Key::Key_Dollar;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Percent)] =
        Keyboard::Key::Key_Percent;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Ampersand)] =
        Keyboard::Key::Key_Ampersand;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Quote)] =
        Keyboard::Key::Key_Quote;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_LeftParen)] =
        Keyboard::Key::Key_LeftParen;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_RightParen)] =
        Keyboard::Key::Key_RightParen;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Asterisk)] =
        Keyboard::Key::Key_Asterisk;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Plus)] =
        Keyboard::Key::Key_Plus;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Comma)] =
        Keyboard::Key::Key_Comma;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Minus)] =
        Keyboard::Key::Key_Minus;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Period)] =
        Keyboard::Key::Key_Period;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Slash)] =
        Keyboard::Key::Key_Slash;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_0)] =
        Keyboard::Key::Key_0;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_1)] =
        Keyboard::Key::Key_1;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_2)] =
        Keyboard::Key::Key_2;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_3)] =
        Keyboard::Key::Key_3;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_4)] =
        Keyboard::Key::Key_4;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_5)] =
        Keyboard::Key::Key_5;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_6)] =
        Keyboard::Key::Key_6;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_7)] =
        Keyboard::Key::Key_7;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_8)] =
        Keyboard::Key::Key_8;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_9)] =
        Keyboard::Key::Key_9;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Colon)] =
        Keyboard::Key::Key_Colon;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_SemiColon)] =
        Keyboard::Key::Key_SemiColon;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Less)] =
        Keyboard::Key::Key_Less;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Greater)] =
        Keyboard::Key::Key_Greater;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Question)] =
        Keyboard::Key::Key_Question;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_At)] =
        Keyboard::Key::Key_At;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_LeftBracket)] =
        Keyboard::Key::Key_LeftBracket;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_RightBracket)] =
        Keyboard::Key::Key_RightBracket;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Caret)] =
        Keyboard::Key::Key_Caret;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_UnderScore)] =
        Keyboard::Key::Key_UnderScore;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_BackQuote)] =
        Keyboard::Key::Key_BackQuote;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Delete)] =
        Keyboard::Key::Key_Delete;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_CapsLock)] =
        Keyboard::Key::Key_CapsLock;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_F1)] =
        Keyboard::Key::Key_F1;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_F2)] =
        Keyboard::Key::Key_F2;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_F3)] =
        Keyboard::Key::Key_F3;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_F4)] =
        Keyboard::Key::Key_F4;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_F5)] =
        Keyboard::Key::Key_F5;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_F6)] =
        Keyboard::Key::Key_F6;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_F7)] =
        Keyboard::Key::Key_F7;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_F8)] =
        Keyboard::Key::Key_F8;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_F9)] =
        Keyboard::Key::Key_F9;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_F10)] =
        Keyboard::Key::Key_F10;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_F11)] =
        Keyboard::Key::Key_F11;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_F12)] =
        Keyboard::Key::Key_F12;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_PrintScreen)] =
        Keyboard::Key::Key_PrintScreen;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_ScrollLock)] =
        Keyboard::Key::Key_ScrollLock;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Pause)] =
        Keyboard::Key::Key_Pause;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Insert)] =
        Keyboard::Key::Key_Insert;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_Home)] =
        Keyboard::Key::Key_Home;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_PageUp)] =
        Keyboard::Key::Key_PageUp;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_End)] =
        Keyboard::Key::Key_End;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_PageDown)] =
        Keyboard::Key::Key_PageDown;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_RightArrow)] =
        Keyboard::Key::Key_RightArrow;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_LeftArrow)] =
        Keyboard::Key::Key_LeftArrow;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_DownArrow)] =
        Keyboard::Key::Key_DownArrow;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_UpArrow)] =
        Keyboard::Key::Key_UpArrow;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_LCtrl)] =
        Keyboard::Key::Key_LCtrl;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_LShift)] =
        Keyboard::Key::Key_LShift;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_LAlt)] =
        Keyboard::Key::Key_LAlt;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_RCtrl)] =
        Keyboard::Key::Key_RCtrl;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_RShift)] =
        Keyboard::Key::Key_RShift;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_RAlt)] =
        Keyboard::Key::Key_RAlt;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_a)] =
        Keyboard::Key::Key_a;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_b)] =
        Keyboard::Key::Key_b;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_c)] =
        Keyboard::Key::Key_c;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_d)] =
        Keyboard::Key::Key_d;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_e)] =
        Keyboard::Key::Key_e;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_f)] =
        Keyboard::Key::Key_f;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_g)] =
        Keyboard::Key::Key_g;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_h)] =
        Keyboard::Key::Key_h;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_i)] =
        Keyboard::Key::Key_i;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_j)] =
        Keyboard::Key::Key_j;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_k)] =
        Keyboard::Key::Key_k;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_l)] =
        Keyboard::Key::Key_l;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_m)] =
        Keyboard::Key::Key_m;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_n)] =
        Keyboard::Key::Key_n;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_o)] =
        Keyboard::Key::Key_o;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_p)] =
        Keyboard::Key::Key_p;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_q)] =
        Keyboard::Key::Key_q;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_r)] =
        Keyboard::Key::Key_r;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_s)] =
        Keyboard::Key::Key_s;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_t)] =
        Keyboard::Key::Key_t;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_u)] =
        Keyboard::Key::Key_u;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_v)] =
        Keyboard::Key::Key_v;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_w)] =
        Keyboard::Key::Key_w;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_x)] =
        Keyboard::Key::Key_x;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_y)] =
        Keyboard::Key::Key_y;
    m_keyNameToKeyCodeMap[Keyboard::PrintKey(Keyboard::Key::Key_z)] =
        Keyboard::Key::Key_z;
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

            if (keyEvent.state == ds_platform::Keyboard::State::Key_Pressed)
            {
                // Get message string from current input context using key code
                std::string messageString =
                    m_inputContextStack.front().GetMessageString(keyEvent.key);

                // Send string to message factory and get message stream back
                ds_msg::MessageStream stream =
                    ds_msg::MessageFactory::ConstructMessage(messageString);

                // Add message to messages generated by this system, to be
                // broadcast later
                AppendStreamBuffer(&m_messagesGenerated, stream);
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
