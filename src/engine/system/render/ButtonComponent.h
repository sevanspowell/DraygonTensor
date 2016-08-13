#pragma once

#include "engine/system/render/Mesh.h"

namespace ds_render
{
struct ButtonComponent 
{
    float startX;
    float startY;
    float endX;
    float endY;
    MaterialHandle defaultMaterial;
    MaterialHandle pressedMaterial;
    MaterialHandle hoverMaterial;
};
}
