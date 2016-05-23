#pragma once

#include "engine/entity/Entity.h"
#include "engine/common/StreamBuffer.h"
#include "engine/common/StringIntern.h"
#include "engine/system/platform/GraphicsContext.h"
#include "engine/system/platform/Keyboard.h"
#include "engine/system/platform/Mouse.h"
#include "math/Matrix4.h"
#include "math/Vector3.h"

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
    // On graphics context creation
    GraphicsContextCreated,
    // On component creation
    CreateComponent,
    // Set local translation of an entity
    SetLocalTranslation,
    // Set local orientation of an entity
    SetLocalOrientation,
    // Set local scale of an entity
    SetLocalScale,
    // Set an entity's animation
    SetAnimationIndex,
    // Set skybox material
    SetSkyboxMaterial,
    // On mouse move event
    MouseMotion,
    // On mouse button click/release event
    MouseButton,
    // Create GUI panel message
    CreatePanel,
    // Create GUI button message
    CreateButton,
    // On button fire (button click)
    ButtonFired
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

struct GraphicsContextCreated
{
    ds_platform::GraphicsContext::ContextInfo
        contextInfo; // Information on the graphics context created
};

struct CreateComponent
{
    ds::Entity entity; // Entity to create component for
    ds::StringIntern::StringId
        componentType; // Type of component to create  as a string
    ds::StringIntern::StringId componentData; // Component config string.
};

// struct SetLocalTransform
// {
//     ds::Entity entity;               // Entity to set local transform of
//     ds_math::Matrix4 localTransform; // New local transform
// };

struct SetLocalTranslation
{
    ds::Entity entity;
    ds_math::Vector3 localTranslation;
};

struct SetLocalOrientation
{
    ds::Entity entity;
    ds_math::Quaternion localOrientation;
};

struct SetLocalScale
{
    ds::Entity entity;
    ds_math::Vector3 localScale;
};

struct SetAnimationIndex
{
    ds::Entity entity;  // Entity to set the animation index of
    int animationIndex; // New animation index
};

struct SetSkyboxMaterial
{
    ds::StringIntern::StringId skyboxMaterialPath; // Path to skybox material
};

struct MouseMotion
{
    ds_platform::Mouse::ButtonState button; // State of the mouse buttons
    float x;            // x-coordinate of cursor, relative to window
    float y;            // y-coordinate of cursor, relative to window
    float xRel;         // relative motion of cursor in the x direction
    float yRel;         // relative motion of cursor in the y direction
    uint32_t timeStamp; // Time stamp of mouse motion event
    uint32_t windowID;  // ID of window with focus (if any)
};

struct MouseButton
{
    ds_platform::Mouse::ButtonState button; // State of the mouse buttons
    float x;        // x-coordinate of cursor, relative to window
    float y;        // y-coordinate of cursor, relative to window
    uint8_t clicks; // Number of clicks (1 for single-click, 2 for double-click,
                    // etc.)
    uint32_t timeStamp; // Time stamp of mouse motion event;
    uint32_t windowID;  // ID of window with focus (if any)
};

struct CreatePanel
{
    ds::Entity entity;
    float startX;
    float startY;
    float endX;
    float endY;
    ds::StringIntern::StringId materialPath;
};

struct CreateButton
{
    ds::Entity entity;
    float startX;
    float startY;
    float endX;
    float endY;
    ds::StringIntern::StringId defaultMaterialPath;
    ds::StringIntern::StringId pressedMaterialPath;
    ds::StringIntern::StringId hoverMaterialPath;
};

struct ButtonFired
{
    ds::Entity entity;
};
}
