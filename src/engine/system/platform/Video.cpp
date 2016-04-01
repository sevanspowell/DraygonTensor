#include <SDL2/SDL.h>

#include "engine/system/platform/Video.h"

namespace ds_platform
{
bool Video::Initialize(const ds::Config &config)
{
    bool result = false;

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) == 0)
    {
        result = true;
    }

    return result;
}

void Video::Update()
{
} 

void Video::Shutdown()
{
}
}
