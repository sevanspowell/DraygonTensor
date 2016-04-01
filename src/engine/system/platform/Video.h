#pragma once

// #include "engine/system/platform/Window.h"
#include "engine/Config.h"

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
     * @param   config  const Config &, configuration loaded by the engine.
     * @return          bool, TRUE if initialization succeeds, FALSE otherwise.
     */
    bool Initialize(const ds::Config &config);

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

private:
    // Window m_window;
};
}
