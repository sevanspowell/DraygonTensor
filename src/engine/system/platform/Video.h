#pragma once

#include "engine/Config.h"
#include "engine/message/Message.h"
#include "engine/system/platform/Window.h"

namespace ds_platform
{
class Video
{
public:
    /**
     * Initialize the video system.
     *
     * Creates a window with the default settings.
     *
     * @param   config   const char *, configuration loaded by the engine.
     * @return           bool, TRUE if initialization succeeds, FALSE otherwise.
     */
    bool Initialize(const char *configFile);

    /**
     * Update the video system.
     *
     * Swaps the graphics buffers, clears the screen and updates the renderer.
     */
    void Update();

    /**
     * Shutdown the video system.
     */
    void Shutdown();

    /**
     * Collect messages generated by the video instance.
     *
     * @return  ds_msg::MessageStream, stream of messages generated by this
     * system.
     */
    ds_msg::MessageStream CollectMessages();

    /**
     * Enable/Disable mouse lock.
     *
     * @param  enableMouseLock  bool, TRUE to enable mouse lock, FALSE to
     * disable it.
     */
    void SetMouseLock(bool enableMouseLock);

    /**
     * Get the width of the window managed by the video system.
     *
     * @return  unsigned int, width of the window managed by the video system.
     */
    unsigned int GetWindowWidth() const;

    /**
     * Get the height of the window managed by the video system.
     *
     * @return  unsigned int, height of the window managed by the video system.
     */
    unsigned int GetWindowHeight() const;

    /**
     * Set the width of the window managed by the video system.
     *
     * Note that this will not actually change the size of the rendered window,
     * only the cached size.
     *
     * @param  unsigned int  windowWidth, value for the width of the window to
     * set.
     */
    void SetWindowWidth(unsigned int windowWidth);

    /**
     * Set the height of the window managed by the video system.
     *
     * Note that this will not actually change the size of the rendered window,
     * only the cached size.
     *
     * @param  unsigned int  windowHeight, value for the height of the window to
     * set.
     */
    void SetWindowHeight(unsigned int windowHeight);

private:
    Window m_window;

    ds_msg::MessageStream m_messagesGenerated;
};
}
