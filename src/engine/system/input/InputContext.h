#pragma once

#include <string>
#include <map>

#include "engine/system/platform/Keyboard.h"

namespace ds
{
/**
 * An InputContext is a set of keybinds. Only one InputContext may be enabled at
 * any one time. Each key is bound to a message string which is used to
 * construct the message structure by passing it to the MessageFactory.
 *
 * While the player is walking around, one InputContext is active, mapping
 * their WASD keys to move messages etc. When the player enters a vehicle, a
 * different InputContext is pushed to the InputContext stack. Now when the
 * player presses the WASD keys, vehicle move messages are produced instead.
 * When the user exits the vehicle, the vehicle input context is popped off the
 * stack, leaving the default input context.
 */
class InputContext
{
public:
    /**
     * Construct an input context with the given name.
     *
     * @param  contextName  std::string, name to give context.
     */
    InputContext(std::string contextName);

    /**
     * Add a key bind to the InputContext.
     *
     * Binds key to message string, which is passed to the message factory to
     * create a message.
     *
     * Note: will overwrite previous key bind if it already exists.
     *
     * @param  key            ds_platform::Keyboard::Key, key to bind message
     * string to
     * @param  messageString  std::string, message string.
     */
    void BindKey(ds_platform::Keyboard::Key key, std::string messageString);

    /**
     * Gets the message string bound to the given key.
     *
     * Returns an empty string if no keybind for that key exists.
     *
     * @param   key  ds_platform::Keyboard::Key, key to get message string for.
     * @return       std::string, message string.
     */
    std::string GetMessageString(ds_platform::Keyboard::Key key) const;

    /**
     * Get the input context's name.
     *
     * @return  std::string, context name.
     */
    const std::string GetContextName() const;

private:
    // Map key code to message string (message string is passed to message
    // factory to create message)
    std::map<ds_platform::Keyboard::Key, std::string> m_keyToMessageStringMap;

    // Name of context
    std::string m_name;
};
}
