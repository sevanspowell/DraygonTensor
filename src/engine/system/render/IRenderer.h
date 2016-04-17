#pragma once

#include "engine/system/render/RenderCommon.h"
#include "engine/system/render/VertexBufferDescription.h"
#include "engine/system/render/ConstantBuffer.h"

namespace ds_render
{
/**
 * Abstract render class.
 *
 * Abstracts away implementation details of any specific graphics API.
 */
class IRenderer
{
public:
    /**
     * Allows renderer to perform any necessary initialization.
     *
     * Should be called before any other renderer methods.
     *
     * @param   viewportWidth   unsigned int, width of the viewport in pixels.
     * @param   viewportHeight  unsigned int, width of the viewport in pixels.
     * @return                  bool, TRUE if renderer initialized successfully,
     * FALSE otherwise.
     */
    virtual bool Init(unsigned int viewportWidth,
                      unsigned int viewportHeight) = 0;

    /**
     * Set the clear colour of the colour buffer.
     *
     * @param r float, red value (0 - 1).
     * @param g float, green value (0 - 1).
     * @param b float, blue value (0 - 1).
     * @param a float, alpha value (0 - 1).
     */
    virtual void SetClearColour(float r, float g, float b, float a) = 0;

    /**
     * Clear the colour, depth and/or stencil buffers of the renderer.
     *
     * Buffers passed a value of TRUE are cleared, buffers passed a value OF
     * FALSE are not.
     *
     * @param colour bool, TRUE to clear colour buffer, FALSE otherwise.
     * @param depth bool, TRUE to clear depth buffer, FALSE otherwise.
     * @param stencil bool, TRUE to clear stencil buffer, FALSE otherwise.
     */
    virtual void ClearBuffers(bool colour = true,
                              bool depth = true,
                              bool stencil = true) = 0;

    /**
     * Resize the viewport the renderer renders to.
     *
     * @param  newViewportWidth   unsigned int, new viewport width in pixels.
     * @param  newViewportHeight  unsigned int, new viewport height in pixels.
     */
    virtual void ResizeViewport(unsigned int newViewportWidth,
                                unsigned int newViewportHeight) = 0;

    /**
     * Create a vertex buffer.
     *
     * A vertex buffer is a collection of vertex data (eg. position, normal,
     * texCoords)
     *
     * @param   usage        BufferUsageType, how the buffer will be used. See
     * BufferUsageType.
     * @param   description  VertexBufferDescription, description of the vertex
     * buffer and it's attributes. See VertexBufferDescription
     * @param   numBytes     size_t, number of bytes of data in vertex buffer.
     * @param   data         const void *, vertex buffer data to construct
     * vertex buffer with.
     * @return               VertexBufferHandle, handle to vertex buffer
     * created.
     */
    virtual VertexBufferHandle
    CreateVertexBuffer(BufferUsageType usage,
                       const VertexBufferDescription &description,
                       size_t numBytes,
                       const void *data) = 0;

    /**
     * Create an index buffer.
     *
     * A index buffer is a collection of indices which are used to determine the
     * order in which the vertices in a vertes buffer are drawn.
     *
     * @pre  Each index provided is an unsigned int.
     *
     * @param   usage     BufferUsageType, how the buffer will be used. See
     * BufferUsageType.
     * @param   numBytes  size_t, number of bytes if data in the index buffer.
     * @param   data      const void *, index buffer data.
     * @return            IndexBufferHandle, handle to index buffer created.
     */
    virtual IndexBufferHandle CreateIndexBuffer(BufferUsageType usage,
                                                size_t numBytes,
                                                const void *data) = 0;

    /**
     * Compile the given shader source int a shader object of the given type.
     *
     * @param   shaderType        ShaderType, type of the shader (Vertex,
     * Fragment, etc.)
     * @param   shaderSourceSize  size_t, size of the shader source data (in
     * bytes).
     * @param   shaderSource      const char *, pointer to shader source data.
     * @return                    ShaderHandle, handle to compiled to shader
     * object.
     */
    virtual ShaderHandle CreateShaderObject(ShaderType shaderType,
                                            size_t shaderSourceSize,
                                            const char *shaderSource) = 0;

    /**
     * Create a shader program from a list of shaders.
     *
     * @param  shaders  const vector<ShaderHandle> &, list of shaders to create
     * shader program from.
     * @return          ProgramHandle, handle to created program.
     */
    virtual ProgramHandle
    CreateProgram(const std::vector<ShaderHandle> &shaders) = 0;

    /**
     * Set a shader program as the current shader program
     *
     * @param  programHandle  ProgramHandle, handle to program to set as the
     * current shader.
     * TODO: Unset program
     */
    virtual void SetProgram(ProgramHandle programHandle) = 0;

    /**
     * Update the given constant buffer name in the given program with the given
     * data.
     *
     * @param  programHandle  ProgramHandle, handle to program to update
     * constant buffer of.
     * @param  constanBufferName  const std::string &, name of the constant
     * buffer in the shader to update.
     * @param  constantBufferData  const ConstantBuffer &, constant buffer data
     * to update with.
     */
    virtual void
    UpdateConstantBuffer(ProgramHandle programHandle,
                         const std::string &constantBufferName,
                         const ConstantBuffer &constantBufferData) = 0;

    /**
     * Draw a number of vertices in a vertex buffer.
     *
     * The vertices are drawn one-by-one from the given starting vertex.
     *
     * @param  buffer          VertexBufferHandle, vertex buffer to draw from.
     * @param  primitiveType   PrimitiveType, primitives to draw with vertices.
     * @param  startingVertex  size_t, index of vertex in vertex buffer to begin
     * drawing from.
     * @param  numVertices     size_t, number of vertices to draw from vertex
     * buffer.
     */
    virtual void DrawVertices(VertexBufferHandle buffer,
                              PrimitiveType primitiveType,
                              size_t startingVertex,
                              size_t numVertices) = 0;

    /**
     * Draw a number of vertices in a vertex buffer using the index buffer to
     * choose which vertices to draw and in which order.
     *
     * @param  buffer          VertexBufferHandle, vertex buffer to draw from.
     * @param  indexBuffer     IndexBufferHandle, index buffer to determine
     * which vertices to draw and in what order.
     * @param  primitiveType   PrimitiveType, primitives to draw with vertices.
     * @param  startingIndex   size_t, index to begin drawing from.
     * @param  numIndices      size_t, number of indices to draw.
     */
    virtual void DrawVerticesIndexed(VertexBufferHandle buffer,
                                     IndexBufferHandle indexBuffer,
                                     PrimitiveType primitiveType,
                                     size_t startingIndex,
                                     size_t numIndices) = 0;

private:
};
}
