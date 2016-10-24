#include <algorithm>
#include <fstream>

#include <SDL2/SDL.h>

#include "engine/json/Json.h"
#include "engine/message/MessageHelper.h"
#include "engine/system/platform/Video.h"

namespace ds_platform
{
bool Video::Initialize(const char *configFile)
{
    bool result = true;

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) == 0)
    {
        ds::JsonObject root;
        ds::json::parseObject(configFile, &root);

        std::string renderer = std::string("None");

        if (root["Video"] != nullptr)
        {
            ds::JsonObject video;
            ds::json::parseObject(root["Video"], &video);

            if (video["redBits"] != nullptr)
            {
                m_window.redBits = ds::json::parseInt(video["redBits"]);
            }
            if (video["greenBits"] != nullptr)
            {
                m_window.greenBits = ds::json::parseInt(video["greenBits"]);
            }
            if (video["blueBits"] != nullptr)
            {
                m_window.blueBits = ds::json::parseInt(video["blueBits"]);
            }
            if (video["alphaBits"] != nullptr)
            {
                m_window.alphaBits = ds::json::parseInt(video["alphaBits"]);
            }

            if (video["depthBits"] != nullptr)
            {
                m_window.depthBits = ds::json::parseInt(video["depthBits"]);
            }
            if (video["stencilBits"] != nullptr)
            {
                m_window.stencilBits = ds::json::parseInt(video["stencilBits"]);
            }

            if (video["fullscreen"] != nullptr)
            {
                m_window.fullscreen = ds::json::parseBool(video["fullscreen"]);
            }
            if (video["lockMouse"] != nullptr)
            {
                m_window.lockMouse = ds::json::parseBool(video["lockMouse"]);
            }

            if (video["width"] != nullptr)
            {
                m_window.width = ds::json::parseInt(video["width"]);
            }
            if (video["height"] != nullptr)
            {
                m_window.height = ds::json::parseInt(video["height"]);
            }

            if (video["title"] != nullptr)
            {
                m_window.title = "";
                ds::json::parseString(video["title"], &m_window.title);
            }

            if (video["renderer"] != nullptr)
            {
                renderer = "";
                ds::json::parseString(video["renderer"], &renderer);
                // Ignore case
                std::transform(renderer.begin(), renderer.end(),
                               renderer.begin(), ::tolower);
            }

            if (renderer == "opengl")
            {
                // Set default OpenGL context values
                m_window.contextInfo.type =
                    GraphicsContext::ContextType::OpenGL;
                m_window.contextInfo.openGL.majorVersion = 3;
                m_window.contextInfo.openGL.minorVersion = 3;
                m_window.contextInfo.openGL.profile =
                    GraphicsContext::ContextProfileOpenGL::Core;

                if (video["majorVersion"] != nullptr &&
                    video["minorVersion"] != nullptr &&
                    video["profile"] != nullptr)
                {
                    m_window.contextInfo.openGL.majorVersion =
                        ds::json::parseInt(video["majorVersion"]);
                    m_window.contextInfo.openGL.minorVersion =
                        ds::json::parseInt(video["minorVersion"]);

                    std::string profile = "";
                    ds::json::parseString(video["profile"], &profile);
                    // Ignore case
                    std::transform(profile.begin(), profile.end(),
                                   profile.begin(), ::tolower);
                    if (profile == std::string("compatability"))
                    {
                        m_window.contextInfo.openGL.profile = GraphicsContext::
                            ContextProfileOpenGL::Compatability;
                    }
                    // else, leave as default (core) profile
                }
            }
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
