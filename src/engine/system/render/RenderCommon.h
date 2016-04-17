#pragma once

#include "engine/common/Handle.h"

namespace ds_render
{
/**
 * Possible render data types.
 */
enum class RenderDataType
{
    Int,
    Float,
};

/**
 * Possible buffer usage types. Essentially how frequently the buffer needs to
 * be updated.
 */
enum class BufferUsageType
{
    // Buffer doesn't need to be updated/infrequently
    Static,
    // Buffer is frequently updated
    Dynamic
};

/**
 * Enumeration of the possible vertex attribute types.
 */
enum class AttributeType
{
    Position,
    Normal,
    TexturCoordinate
};

/** Shader source type */
enum class ShaderType
{
    VertexShader,
    FragmentShader
};

/** Handle to a vertex buffer */
typedef ds::Handle VertexBufferHandle;
/** Handle to a index buffer */
typedef ds::Handle IndexBufferHandle;
/** Handle to a shader object */
typedef ds::Handle ShaderHandle;
/** Handle to a shader program */
typedef ds::Handle ProgramHandle;
}
