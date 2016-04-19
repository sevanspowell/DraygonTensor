#pragma once

#include "engine/system/render/Material.h"
#include "engine/system/render/Mesh.h"

namespace ds_render
{
struct RenderComponent
{
    Material material;
    Mesh mesh;
};
}
