#include <iostream>

#include <SDL2/SDL.h>

#include "engine/Engine.h"
#include "engine/system/Console.h"
#include "engine/system/platform/Platform.h"

int main(void)
{
    // ds::Engine engine;

    // engine.AddSystem(std::unique_ptr<ds::ISystem>(new ds::Console()));
    // engine.AddSystem(std::unique_ptr<ds::ISystem>(new ds::Platform()));
    // engine.Start();
    bool done = false;

    std::string inputText = "";

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_StartTextInput();
    while (!done)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    done = true;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
                    {
                        inputText.pop_back();
                    }
                    if (event.key.keysym.sym == SDLK_RETURN)
                    {
                        inputText = std::string();
                    }
                    break;
                case SDL_TEXTINPUT:
                    std::cout << "Reached" << std::endl;
                    inputText += event.text.text;
                    break;
            }
        }
        std::cout << inputText << std::endl;
    }

    SDL_Quit();

    return 0;
}
