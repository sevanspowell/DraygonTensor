#include <iostream>

#include "engine/Engine.h"
#include "engine/system/Console.h"
#include "engine/system/script/Script.h"

int main(void)
{
    ds::Engine engine;

    engine.AddSystem(std::unique_ptr<ds::ISystem>(new ds::Console()));
    engine.AddSystem(std::unique_ptr<ds::ISystem>(new ds::Script()));
    engine.Start();

    return 0;
}
