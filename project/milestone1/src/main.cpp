#include "engine/Engine.h"
#include "engine/system/input/Input.h"
#include "engine/system/render/Render.h"
#include "engine/system/physics/Physics.h"

int main(int argc, char **argv)
{
    ds::Engine engine;

    // Add all systems to engine
    engine.AddSystem(std::unique_ptr<ds::ISystem>(new ds::Input()));
    engine.AddSystem(std::unique_ptr<ds::ISystem>(new ds::Render()));

    engine.Start();

    return 0;
}
