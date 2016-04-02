#pragma once

#include "engine/common/StreamBuffer.h"
#include "engine/common/StringIntern.h"
#include "engine/system/platform/Keyboard.h"

namespace ds_msg
{
/**
 * A message stream is a buffer full of messages of different types and sizes.
 * A message should always be inserted into the stream such that the message
 * header is found before the message data. To properly extract the messages
 * from the stream, extract a message header, get the type of the message and
 * the size of the message payload. Extract the given number of bytes into the
 * correct message structure.
 */
typedef ds_com::StreamBuffer MessageStream;

/**
 * Type of the message.
 */
enum class MessageType
{
    // On system init
    SystemInit,
    // On config load attempt
    ConfigLoad,
    // When you want to output a string to the console
    // ConsoleOut,
    // When you want the scripting system to interpret a string
    ScriptInterpret,
    // On Keyboard event
    KeyboardEvent,
    // On Quit event
    QuitEvent,
    // Toggle console on/off
    ConsoleToggle,
    // On text input
    TextInput,
};

/**
 * Message header, used to extract the message payload correctly.
 */
struct MessageHeader
{
    MessageType type;
    size_t size;
};

// Message payloads
struct SystemInit
{
    const char *systemName;
};

struct ConfigLoad
{
    const char *configFilePath;
    bool didLoad;
};

struct ScriptInterpret
{
    ds::StringIntern::StringId
        stringId; // Interned id of the string to interpret
};

struct KeyboardEvent
{
    ds_platform::Keyboard::Key key;     // Key that was pressed/released
    ds_platform::Keyboard::State state; // State of the key (pressed/released)
    bool repeat;        // TRUE if this is a key repeat, FALSE otherwise.
    uint32_t timeStamp; // Time stamp of key event
    uint32_t windowID;  // ID of window with focus (if any)
};

struct QuitEvent
{
};

struct ConsoleToggle
{
};

struct TextInput
{
    ds::StringIntern::StringId stringId; // Interned id of the string produced
                                         // by this text input event.
    uint32_t timeStamp;                  // Time stamp of text input event
    uint32_t windowID;                   // ID of window with focus (if any)
};
}
