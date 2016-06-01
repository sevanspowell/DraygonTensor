#pragma once

#include "engine/system/render/RenderCommon.h"
#include "engine/system/render/VertexBufferDescription.h"
#include "engine/system/render/ConstantBufferDescription.h"
#include "engine/system/render/ShaderParameter.h"

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
     * Enable or disable writing to the depth buffer.
     *
     * @param  enableDisableDepthMask  bool, TRUE to enable depth buffer
     * writing, FALSE to disable it.
     */
    virtual void SetDepthWriting(bool enableDisableDepthWriting) = 0;

    /**
     * Enable or disable blending in the colour buffer.
     *
     * @param  enableBlending  bool, TRUE to enable colour buffer blending,
     * FALSE to disable it.
     */
    virtual void SetBlending(bool enableBlending) = 0;

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
     * Create a two-dimensional texture.
     *
     * @param  format           ImageFormat, composition of each element in
     * data. Number of colour components in the texture.
     * @param  imageDataType    RenderDataType, data type of pixel data.
     * @param  internalFormat   InternalImageFormat, request the renderer to
     * store the image data in a specific format.
     * @param  generateMipMaps  bool, TRUE to generate mipmaps, FALSE to not.
     * @param  width            unsigned int, width of the image in pixels.
     * @param  height           unsigend int, height of the image in pixels.
     * @param  data             const void *, pointer to image data.
     */
    virtual RenderTextureHandle
    Create2DTexture(ImageFormat format,
                    RenderDataType imageDataType,
                    InternalImageFormat internalFormat,
                    bool generateMipMaps,
                    unsigned int width,
                    unsigned int height,
                    const void *data) = 0;

    /**
     * Create a cubemap texture.
     *
     * @param  format           ImageFormat, composition of each element in
     * data. Number of colour components in the textures.
     * @param  imageDataType    RenderDataType, data type of pixel data for all
     * images.
     * @param  internalFormat   InternalImageFormat, request the renderer to
     * store the image data in a specific format.
     * @param  width            unsigned int, width of all images in pixels.
     * @param  height           unsigend int, height of all images in pixels.
     * @param  dataFrontImage   const void *, pointer to front image data.
     * @param  dataBackImage    const void *, pointer to back image data.
     * @param  dataLeftImage    const void *, pointer to left image data.
     * @param  dataRightImage   const void *, pointer to right image data.
     * @param  dataTopImage     const void *, pointer to top image data.
     * @param  dataBottomImage  const void *, pointer to bottom image data.
     */
    virtual RenderTextureHandle
    CreateCubemapTexture(ImageFormat format,
                         RenderDataType imageDataType,
                         InternalImageFormat internalFormat,
                         unsigned int width,
                         unsigned int height,
                         const void *dataFrontImage,
                         const void *dataBackImage,
                         const void *dataLeftImage,
                         const void *dataRightImage,
                         const void *dataTopImage,
                         const void *dataBottomImage) = 0;

    /**
     * Bind a texture to a sampler in the shader.
     *
     * @param  programHandle  ProgramHandle, shader program containing
     * sampler.
     * @param  samplerName    const std::string &, name of the sampler in
     * the
     * shader
     * @param  textureHandle  RenderTextureHandle, texture to bind to sampler.
     */
    virtual void BindTextureToSampler(ProgramHandle programHandle,
                                      const std::string &samplerName,
                                      const TextureType &samplerType,
                                      RenderTextureHandle textureHandle) = 0;

    /**
     * Unbind texture from sampler.
     *
     * @param  samplerType    const TextureType &, type of sampler to unbind
     * texture from.
     * @param  textureHandle  RenderTextureHandle, texture to unbind.
     */
    virtual void
    UnbindTextureFromSampler(const TextureType &samplerType,
                             RenderTextureHandle textureHandle) = 0;

    /**
     * Memory layout of constant buffer in renderer may not match that of C/C++.
     * Use this method to get a description of the memory layout. Fill the
     * description with data.
     *
     * @param   programHandle              ProgramHandle, handle to program
     * containing constant buffer.
     * @param   constantBufferName         const std::string &, name of constant
     * buffer to get description of.
     * @param   constantBufferDescription  ConstantBufferDescription *,
     * description of constant buffer. Caller should fill in names of members.
     * Renderer will fill out member name offsets.
     */
    virtual void GetConstantBufferDescription(
        ProgramHandle programHandle,
        const std::string &constantBufferName,
        ConstantBufferDescription *constantBufferDescription) = 0;

    /**
     * Create a constant buffer with the given constant buffer description.
     *
     * @param   constantBufferDescription  const ConstantBufferDescription &,
     * constant buffer description to create constant buffer with.
     * @return                             ConstantBufferHandle, handle to
     * created constant buffer object.
     */
    virtual ConstantBufferHandle CreateConstantBuffer(
        const ConstantBufferDescription &constantBufferDescription) = 0;

    /**
     * Bind data associated with the given constant buffer object to the named
     * constant buffer in the given program.
     *
     * Does not need to be called every frame, can be called once at
     * initialization.
     *
     * @param  programHandle             ProgramHandle, handle to program
     * containing constant buffer.
     * @param  constantBufferName        const std::string &, name of the
     * constant buffer in the given shader program.
     * @param  constantBufferHandle      ConstantBufferHandle, handle to
     * constant buffer to use to fill constant buffer data in given shader
     * program.
     */
    virtual void
    BindConstantBuffer(ProgramHandle programHandle,
                       const std::string &constantBufferName,
                       ConstantBufferHandle constantBufferHandle) = 0;

    /**
     * Update the data associated with the given constant buffer.
     *
     * @param  constantBufferHandle  ConstantBufferHandle, handle to constant
     * buffer to update data of.
     * @param  constantBufferDescription  const ConstantBufferDescription &, new
     * constant buffer data.
     */
    virtual void UpdateConstantBufferData(
        ConstantBufferHandle constantBufferHandle,
        const ConstantBufferDescription &constantBufferDescription) = 0;

    /**
     * Draw a number of vertices in a vertex buffer.
     *
     * The vertices are drawn one-by-one from the given starting vertex.
     *
     * @param  buffer          VertexBufferHandle, vertex buffer to draw
     * from.
     * @param  primitiveType   PrimitiveType, primitives to draw with
     * vertices.
     * @param  startingVertex  size_t, index of vertex in vertex buffer to
     * begin
     * drawing from.
     * @param  numVertices     size_t, number of vertices to draw from
     * vertex
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

    /**
     * Update the value of the specified parameter in the given program.
     *
     * @param  programHandle  ProgramHandle, handle to program to update
     * parameter of.
     * @param  parameterName  const std::string &, name of program parameter to
     * update.
     * @param  parameterType  ShaderParameter::ShaderParameterType, data type of
     * the parameter.
     * @param  parameterData  const void *, program parameter data to update
     * with.
     */
    virtual void
    UpdateProgramParameter(ProgramHandle programHandle,
                           const std::string &parameterName,
                           ShaderParameter::ShaderParameterType parameterType,
                           const void *parameterData) = 0;

private:
};
}
