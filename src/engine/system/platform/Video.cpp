#include <algorithm>

#include <SDL2/SDL.h>

#include "engine/system/platform/Video.h"
#include "engine/message/MessageHelper.h"

namespace ds_platform
{
bool Video::Initialize(const ds::Config &config)
{
    bool result = true;

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) == 0)
    {
        config.GetUnsignedInt("Video.redBits", &m_window.redBits);
        config.GetUnsignedInt("Video.greenBits", &m_window.greenBits);
        config.GetUnsignedInt("Video.blueBits", &m_window.blueBits);
        config.GetUnsignedInt("Video.alphaBits", &m_window.alphaBits);

        config.GetUnsignedInt("Video.depthBits", &m_window.depthBits);
        config.GetUnsignedInt("Video.stencilBits", &m_window.stencilBits);

        config.GetBool("Video.fullscreen", &m_window.fullscreen);
        config.GetBool("Video.lockMouse", &m_window.lockMouse);

        config.GetUnsignedInt("Video.width", &m_window.width);
        config.GetUnsignedInt("Video.height", &m_window.height);

        config.GetString("Video.title", &m_window.title);

        // Get renderer type
        std::string renderer = std::string("None");
        config.GetString("Video.renderer", &renderer);
        // Ignore case of renderer string
        std::transform(renderer.begin(), renderer.end(), renderer.begin(),
                       ::tolower);

        if (renderer == "opengl")
        {
            // Set default OpenGL context values
            m_window.contextInfo.type = GraphicsContext::ContextType::OpenGL;
            m_window.contextInfo.openGL.majorVersion = 3;
            m_window.contextInfo.openGL.minorVersion = 3;
            m_window.contextInfo.openGL.profile =
                GraphicsContext::ContextProfileOpenGL::Core;

            // Get values from config if they exist
            config.GetUnsignedInt("Video.majorVersion",
                                  &m_window.contextInfo.openGL.majorVersion);
            config.GetUnsignedInt("Video.minorVersion",
                                  &m_window.contextInfo.openGL.minorVersion);
            std::string profile = std::string("core");
            config.GetString("Video.profile", &profile);
            // Ignore case of profile string
            std::transform(profile.begin(), profile.end(), profile.begin(),
                           ::tolower);
            if (profile == std::string("compatability"))
            {
                m_window.contextInfo.openGL.profile =
                    GraphicsContext::ContextProfileOpenGL::Compatability;
            }
            // else, leave as default (core) profile
        }

        result &= m_window.Create();

        // If we successfully created a graphics context
        if (result == true && renderer != "none")
        {
            // Tell everyone that a graphics context has been created
            ds_msg::GraphicsContextCreated gfxContext;
            gfxContext.contextInfo = m_window.contextInfo;

            ds_msg::AppendMessage(&m_messagesGenerated,
                                  ds_msg::MessageType::GraphicsContextCreated,
                                  sizeof(ds_msg::GraphicsContextCreated),
                                  &gfxContext);

            // Tell everyone current size of the window
            ds_msg::WindowResize windowResize;
            windowResize.newWidth = m_window.width;
            windowResize.newHeight = m_window.height;

            ds_msg::AppendMessage(&m_messagesGenerated,
                                  ds_msg::MessageType::WindowResize,
                                  sizeof(ds_msg::WindowResize), &windowResize);
        }
    }

    return result;
}

void Video::Update()
{
    m_window.SwapBuffers();
}

void Video::Shutdown()
{
    m_window.Destroy();
}

ds_msg::MessageStream Video::CollectMessages()
{
    ds_msg::MessageStream tmp = m_messagesGenerated;

    m_messagesGenerated.Clear();

    return tmp;
}

void Video::SetMouseLock(bool enableMouseLock)
{
    m_window.SetMouseLock(enableMouseLock);
}

unsigned int Video::GetWindowWidth() const
{
    return m_window.width;
}

unsigned int Video::GetWindowHeight() const
{
    return m_window.height;
}

void Video::SetWindowWidth(unsigned int windowWidth)
{
    m_window.width = windowWidth;
}

void Video::SetWindowHeight(unsigned int windowHeight)
{
    m_window.height = windowHeight;
}
}
