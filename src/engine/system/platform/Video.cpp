#include <SDL2/SDL.h>

#include "engine/system/platform/Video.h"

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

        result &= m_window.Create();
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
}
