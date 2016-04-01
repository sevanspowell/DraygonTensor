#include <iostream>

#include "engine/Engine.h"
#include "engine/system/Console.h"

int main(void)
{
    ds::Engine engine;

    engine.AddSystem(std::unique_ptr<ds::ISystem>(new ds::Console()));
    engine.Start();

    return 0;
}
