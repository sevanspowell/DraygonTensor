#include <iostream>

#include "engine/system/platform/Window.h"

namespace ds_platform
{
Window::Window()
{
    redBits = 8;
    greenBits = 8;
    blueBits = 8;
    alphaBits = 8;

    depthBits = 24;
    stencilBits = 8;

    fullscreen = false;
    lockMouse = false;

    width = 800;
    height = 600;

    title = "Working Title";
}

bool Window::Create()
{
    return CreateSDL2Window(redBits, greenBits, blueBits, alphaBits, depthBits,
                            stencilBits, lockMouse, fullscreen, title, width,
                            height, contextInfo, &_window);
}

void Window::Destroy()
{
    SDL_DestroyWindow(_window);
}

void Window::SwapBuffers()
{
    SDL_GL_SwapWindow(_window);
}

bool Window::CreateSDL2Window(unsigned int redBits,
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
                              GraphicsContext::ContextInfo contextInfo,
                              SDL_Window **window)
{
    bool result = false;

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, redBits);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, greenBits);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, blueBits);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, alphaBits);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depthBits);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, stencilBits);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    uint32_t flags = 0x0;

    if (lockMouse)
    {
        flags |= SDL_WINDOW_INPUT_GRABBED;
        SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1",
                                SDL_HINT_OVERRIDE);
        if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
        {
            std::cout << "Failed to lock mouse: " << SDL_GetError()
                      << std::endl;
        }
    }

    if (fullscreen)
    {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    // Pre-window creation steps
    switch (contextInfo.type)
    {
    case GraphicsContext::ContextType::OpenGL:
        flags |= SDL_WINDOW_OPENGL;
        break;
    default:
        break;
    }

    // Create window
    *window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, width, height, flags);

    if (window == nullptr)
    {
        std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
    }
    else
    {
        result = true;
    }

    // Post-window creation steps
    switch (contextInfo.type)
    {
    case GraphicsContext::ContextType::OpenGL:
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,
                            contextInfo.openGL.majorVersion);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
                            contextInfo.openGL.minorVersion);

        contextInfo.openGL.profile ==
                GraphicsContext::ContextProfileOpenGL::Core
            ? SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                                  SDL_GL_CONTEXT_PROFILE_CORE)
            : SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                                  SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

        _openGLContext = SDL_GL_CreateContext(_window);

        // Disable VSync
        // SDL_GL_SetSwapInterval(0);
        break;
    default:
        break;
    }

    std::cout << SDL_GetError() << std::endl;

    return result;
}

void Window::SetMouseLock(bool enableMouseLock)
{
    if (enableMouseLock == true)
    {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }
    else
    {
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }
}
}
