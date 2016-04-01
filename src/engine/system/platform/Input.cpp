#include <iostream>

#include <SDL2/SDL.h>

#include "engine/system/platform/Input.h"

namespace ds_platform
{
Input::Input()
{
}

void Input::ToggleTextInput() const
{
    if (SDL_IsTextInputActive())
    {
        SDL_StopTextInput();
    }
    else
    {
        SDL_StartTextInput();
    }
}
}
