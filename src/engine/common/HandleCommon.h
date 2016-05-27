#pragma once

namespace ds
{
enum HandleTypes
{
    HandleFirstType = 0x0,
    TextureResourceHandleType,
    MaterialResourceHandleType,
    TextureHandleType,
    MaterialHandleType,
    RenderTextureHandleType,
    RenderProgramHandleType,
    RenderShaderHandleType,
    RenderIndexBufferHandleType,
    RenderVertexBufferHandleType,
    RenderConstantBufferHandleType,
    HandleLastType = ~0x0
};
}
