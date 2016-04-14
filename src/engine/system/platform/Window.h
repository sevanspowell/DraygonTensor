#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "engine/system/platform/GraphicsContext.h"

namespace ds_platform
{
/**
 * Class used to create windows in the engine.
 *
 * Platform video system (eg. SDL_VIDEO) must be initialized before the window
 * is created.
 *
 * The window's parameters are exposed in the public members of the class
 * (window.width, window.height, window.fullscreen etc.). Only modify these
 * directly BEFORE calling Create(). After callling Create(), only use member
 * functions to modify the window. However, the window's exposed data will be
 * kept up-to-date so you can use it to check the status of the window (eg. if
 * (window.fullscreen) {...}).
 *
 * The Create() member function attempts to create a window with the current
 * window parameters.
 */
class Window
{
public:
    enum class WindowEventType
    {
        None,
        Shown,
        Hidden,
        Exposed,
        Moved,
        Resized,
        SizeChanged,
        Minimized,
        Maximized,
        Restored,
        Enter,
        Leave,
        FocusGained,
        FocusLost,
        Close
    };

    /**
     * Sets member variables to sensible values.
     */
    Window();

    /**
     * Create a window with the requested settings.
     *
     * @return bool, TRUE if window was created successfully, FALSE otherwise.
     */
    bool Create();

    /**
     * Destroys the window.
     */
    void Destroy();

    /**
     * Swap the back and front OpenGL framebuffers.
     *
     * Drawing occurs on the back framebuffer, so the front and back buffers
     * must be swapped every frame.
     */
    void SwapBuffers();

    /** The following properties should be set before the Create() member
     * function is called */
    // Width of the window in pixels
    unsigned int width;
    // Height of the window in pixels
    unsigned int height;

    // TRUE for fullscreen, FALSE for windowed
    bool fullscreen;
    // TRUE to lock mouse in window, FALSE otherwise
    bool lockMouse;

    // Number of bits in the red channel of the window
    unsigned int redBits;
    // Number of bits in the green channel of the window
    unsigned int greenBits;
    // Number of bits in the blue channel of the window
    unsigned int blueBits;
    // Number of bits in the alpha channel of the window
    unsigned int alphaBits;

    // Number of bits in the depth buffer of the window
    unsigned int depthBits;
    // Number of bits in the stencil buffer of the window
    unsigned int stencilBits;

    // Title of the window
    std::string title;

    // Graphics context
    GraphicsContext::ContextInfo contextInfo;

private:
    /**
     * Create an SDL2 window with the given parameters.
     *
     * @param redBits     unsigned int, requested depth of the red channel.
     * @param greenBits   unsigned int, requested depth of the green channel.
     * @param blueBits    unsigned int, requested depth of the blue channel.
     * @param alphaBits   unsigned int, requested depth of the alpha channel.
     * @param depthBits   unsigned int, requested depth of the depth buffer.
     * @param stencilBits unsigned int, requested depth of the stencil buffer.
     * @param lockMouse   bool, TRUE if the mouse should be locked within the
     * window, FALSE otherwise.
     * @param fullscreen  bool, TRUE if the window should be started fullscreen,
     * FALSE otherwise.
     * @param title       const std::string &, title of the window.
     * @param width       unsigned int, width (in pixels) of the window.
     * @param height      unsigned int, height (in pixels) of the window.
     * @param context     Context::ContextInfo, information about the graphics
     * context to create, if any.
     * @param window      SDL_Window **, pass in a pointer which hold the
     * reference to the window.
     * @return            bool, TRUE if window was created successfully, FALSE
     * otherwise.
     */
    bool CreateSDL2Window(unsigned int redBits,
                          unsigned int greenBits,
                          unsigned int blueBits,
                          unsigned int alphaBits,
                          unsigned int depthBits,
                          unsigned int stencilBits,
                          bool lockMouse,
                          bool fullscreen,
                          const std::string &title,
                          unsigned int width,
                          unsigned int height,
                          GraphicsContext::ContextInfo context,
                          SDL_Window **window);

    SDL_Window *_window;
    SDL_GLContext _openGLContext;
};
}
