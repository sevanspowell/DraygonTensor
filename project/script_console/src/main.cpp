#include <iostream>

#include "engine/Engine.h"
#include "engine/system/Console.h"
#include "engine/system/script/Script.h"
#include "engine/system/platform/Platform.h"
#include "engine/system/input/Input.h"

int main(int argc, char **argv)
{
    ds::Engine engine;

    engine.AddSystem(std::unique_ptr<ds::ISystem>(new ds::Console()));
    engine.AddSystem(std::unique_ptr<ds::ISystem>(new ds::Script()));
    engine.AddSystem(std::unique_ptr<ds::ISystem>(new ds::Platform()));
    engine.AddSystem(std::unique_ptr<ds::ISystem>(new ds::Input()));
    engine.Start();

    return 0;
}
