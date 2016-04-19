#include <iostream>

#include "engine/system/render/GLRenderer.h"
#include "math/Matrix4.h"

namespace ds_render
{
bool GLRenderer::Init(unsigned int viewportWidth, unsigned int viewportHeight)
{
    bool result = false;

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if (err != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err)
                  << std::endl;
    }
    else
    {
        result = true;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearDepth(1.0f);

    SetClearColour(0.2f, 0.2f, 0.2f, 1.0f);

    ResizeViewport(viewportWidth, viewportHeight);

    ClearBuffers();

    return result;
}

void GLRenderer::SetClearColour(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void GLRenderer::ClearBuffers(bool colour, bool depth, bool stencil)
{
    GLbitfield clearBuffers = 0;

    if (colour)
    {
        clearBuffers |= GL_COLOR_BUFFER_BIT;
    }
    if (depth)
    {
        clearBuffers |= GL_DEPTH_BUFFER_BIT;
    }
    if (stencil)
    {
        clearBuffers |= GL_STENCIL_BUFFER_BIT;
    }

    if (clearBuffers != 0)
    {
        glClear(clearBuffers);
    }
}

void GLRenderer::ResizeViewport(unsigned int newViewportWidth,
                                unsigned int newViewportHeight)
{
    glViewport(0, 0, newViewportWidth, newViewportHeight);
}

VertexBufferHandle
GLRenderer::CreateVertexBuffer(BufferUsageType usage,
                               const VertexBufferDescription &description,
                               size_t numBytes,
                               const void *data)
{
    // Create vbo
    GLuint vbo = 0;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, numBytes, data, ToGLBufferUsageType(usage));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create vao
    GLuint vao = 0;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // Bind vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // For each attribute, create vertex attribute pointer in vao
    const std::vector<VertexBufferDescription::AttributeDescription>
        &attributeDescriptions = description.GetAttributeDescriptions();
    for (unsigned int i = 0; i < attributeDescriptions.size(); ++i)
    {
        const VertexBufferDescription::AttributeDescription
            &attributeDescriptor = attributeDescriptions[i];

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
            i, attributeDescriptor.numElementsPerAttribute,
            ToGLDataType(attributeDescriptor.attributeDataType),
            ToGLBool(attributeDescriptor.normalized),
            attributeDescriptor.stride,
            (char *)NULL + attributeDescriptor.offset);
    }

    glBindVertexArray(0);

    // Return handle to VAO
    return ((VertexBufferHandle)StoreOpenGLObject(
        vao, GLObjectType::VertexArrayObject));
}

IndexBufferHandle GLRenderer::CreateIndexBuffer(BufferUsageType usage,
                                                size_t numBytes,
                                                const void *data)
{
    GLuint ibo = 0;

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numBytes, data,
                 ToGLBufferUsageType(usage));

    return ((IndexBufferHandle)StoreOpenGLObject(
        ibo, GLObjectType::IndexBufferObject));
}

ShaderHandle GLRenderer::CreateShaderObject(ShaderType shaderType,
                                            size_t shaderSourceSize,
                                            const char *shaderSource)
{
    // Create shader
    GLuint shader = glCreateShader(ToGLShaderType(shaderType));
    ShaderHandle handle;

    // Compile shader
    glShaderSource(shader, 1, &shaderSource, (const int *)&shaderSourceSize);
    glCompileShader(shader);

    // Check for errors
    int compileResult = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

    if (compileResult == GL_FALSE)
    {
        // If there was an error, get information
        // Get log length
        int logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        // Create a 'string' of appropriate size
        std::vector<char> shaderLog(logLength);
        // Get log
        glGetShaderInfoLog(shader, logLength, NULL, &shaderLog[0]);

        // Print log
        std::cout << "GLRenderer::CreateShaderObject: Compile Error: "
                  << &shaderLog[0];
    }
    else
    {
        // No error, create shader
        handle =
            (ShaderHandle)StoreOpenGLObject(shader, GLObjectType::ShaderObject);
    }

    return handle;
}

ProgramHandle
GLRenderer::CreateProgram(const std::vector<ShaderHandle> &shaders)
{
    // Create shader program
    GLuint program = glCreateProgram();
    ProgramHandle handle;

    // For each shader
    for (ShaderHandle shaderHandle : shaders)
    {
        // Attach shader
        GLObject glObject = *((GLObject *)m_handleManager.Get(shaderHandle));
        GLuint shaderObject = glObject.object;

        glAttachShader(program, shaderObject);
    }

    // Link shaders into shader program
    glLinkProgram(program);

    // Check for errors
    int linkResult = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkResult);

    if (linkResult == GL_FALSE)
    {
        // If there was an error, get information
        int logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

        // Create a 'string' of appropriate size
        std::vector<char> programLog(logLength);
        // Get log
        glGetProgramInfoLog(program, logLength, NULL, &programLog[0]);

        // Print log
        std::cout << "GLRenderer::CreateProgram: Link Error: "
                  << &programLog[0];
    }
    else
    {
        // No error, create program
        handle = (ProgramHandle)StoreOpenGLObject(program,
                                                  GLObjectType::ProgramObject);
    }

    return handle;
}

void GLRenderer::SetProgram(ProgramHandle programHandle)
{
    // Get program associated with handle
    GLuint program;
    if (GetOpenGLObject(programHandle, GLObjectType::ProgramObject, &program))
    {
        glUseProgram(program);
    }
    else
    {
        std::cerr << "GLRenderer::SetProgram: Failed to set program."
                  << std::endl;
    }
}

TextureHandle GLRenderer::Create2DTexture(ImageFormat format,
                                          RenderDataType imageDataType,
                                          InternalImageFormat internalFormat,
                                          bool generateMipMaps,
                                          unsigned int width,
                                          unsigned int height,
                                          const void *data)
{
    // Create OpenGL texture object
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, ToGLInternalImageFormat(internalFormat),
                 width, height, 0, ToGLImageFormat(format),
                 ToGLDataType(imageDataType), data);

    // Set texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Generate mips
    if (generateMipMaps)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    // Setup anisotropic filtering
    GLfloat maxAnisotropy = 0.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                    maxAnisotropy);

    // Unbind texture object
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create handle to texture object
    return (TextureHandle)StoreOpenGLObject(tex, GLObjectType::TextureObject);
}

void GLRenderer::BindTextureToSampler(ProgramHandle programHandle,
                                      const std::string &samplerName,
                                      TextureHandle textureHandle)
{
    // Is texture already bound to a texture slot?
    std::vector<TextureHandle>::iterator it =
        std::find(m_textureSlots.begin(), m_textureSlots.end(), textureHandle);

    // Texture not already bound
    if (it == m_textureSlots.end())
    {
        // Find an empty texture slot
        it = std::find(m_textureSlots.begin(), m_textureSlots.end(),
                       TextureHandle());

        // Insert into slot
        if (it != m_textureSlots.end())
        {
            *it = textureHandle;
        }
        // Or add it to end if no empty texture slot found
        else
        {
            // Update iterator
            it = m_textureSlots.insert(m_textureSlots.end(), textureHandle);
        }
    }

    // Calculate texture slot index
    unsigned int textureSlot = it - m_textureSlots.begin();

    // Bind GL texture
    GLuint tex = 0;
    if (GetOpenGLObject(textureHandle, GLObjectType::TextureObject, &tex))
    {
        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glBindTexture(GL_TEXTURE_2D, tex);

        GLuint program = 0;
        if (GetOpenGLObject(programHandle, GLObjectType::ProgramObject,
                            &program))
        {
            glUniform1i(glGetUniformLocation(program, samplerName.c_str()),
                        textureSlot);
        }
        else
        {
            std::cerr << "GLRenderer::BindTextureToSampler: Failed to get "
                         "program associated with program handle provided."
                      << std::endl;
        }
    }
    else
    {
        std::cerr << "GLRenderer::BindTextureToSampler: Failed to get texture "
                     "associated with texture handle provided."
                  << std::endl;
    }
}

void GLRenderer::UnbindTextureFromSampler(TextureHandle textureHandle)
{
    // Find texture slot of texture
    std::vector<TextureHandle>::iterator it =
        std::find(m_textureSlots.begin(), m_textureSlots.end(), textureHandle);
    unsigned int textureSlot = it - m_textureSlots.begin();

    // Insert into empty texture handle
    if (it != m_textureSlots.end())
    {
        *it = TextureHandle();
    }

    // Unbind OpenGL texture from sampler
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLRenderer::GetConstantBufferDescription(
    ProgramHandle programHandle,
    const std::string &constantBufferName,
    ConstantBufferDescription *constantBufferDescription)
{
    if (constantBufferDescription != nullptr)
    {
        GLuint program;
        if (GetOpenGLObject(programHandle, GLObjectType::ProgramObject,
                            &program))
        {
            // From
            // https://www.packtpub.com/books/content/opengl-40-using-uniform-blocks-and-uniform-buffer-objects
            // Get index of uniform block
            GLuint blockIndex =
                glGetUniformBlockIndex(program, constantBufferName.c_str());

            // Get size of uniform block (GL size may differ from C++ size)
            GLint blockSize;
            glGetActiveUniformBlockiv(program, blockIndex,
                                      GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

            const size_t numMembers =
                constantBufferDescription->GetNumberOfMembers();

            // Get names for each uniform block member
            GLchar **names = (GLchar **)malloc(numMembers + sizeof(GLchar *));
            // Create our own data store that matches the size and alignment of
            // data that OpenGL expects
            ConstantBufferDescription constantBufferDescriptionTemp(blockSize);
            // Copy names from provided description
            std::vector<std::string> memberNames =
                constantBufferDescription->GetMemberNames();
            for (unsigned int i = 0; i < memberNames.size(); ++i)
            {
                // Add name to constant buffer description
                constantBufferDescriptionTemp.AddMember(memberNames[i]);
                // Add name to GL array
                names[i] = (GLchar *)memberNames[i].c_str();
            }

            // Get index of each uniform block member
            GLuint *indices = (GLuint *)malloc(numMembers * sizeof(GLuint));
            glGetUniformIndices(program, numMembers, names, indices);

            // Check that indices are found
            bool shouldContinue = true;
            for (unsigned int i = 0; i < numMembers; ++i)
            {
                if (indices[i] == GL_INVALID_INDEX)
                {
                    std::cerr
                        << "GLRenderer::UpdateConstantBuffer: Found no "
                           "uniform block member with name: "
                        << names[i] << ". "
                        << "Note: If this uniform block member exists in your "
                           "shader but you aren't using it, "
                           "OpenGL might be optimizing it away."
                        << std::endl;
                    shouldContinue = false;
                }
            }

            if (shouldContinue)
            {
                // Get offset of each member in GL buffer
                GLint *offset = (GLint *)malloc(numMembers * sizeof(GLint));
                glGetActiveUniformsiv(program, numMembers, indices,
                                      GL_UNIFORM_OFFSET, offset);

                // Set member offsets
                for (unsigned int i = 0; i < numMembers; ++i)
                {
                    constantBufferDescriptionTemp.SetMemberOffset(names[i],
                                                                  offset[i]);
                }

                // Update constant buffer description
                *constantBufferDescription = constantBufferDescriptionTemp;

                free(offset);
            }

            free(indices);
            free(names);
        }
    }
}

ConstantBufferHandle GLRenderer::CreateConstantBuffer(
    const ConstantBufferDescription &constantBufferDescription)
{
    // Create OpenGL buffer object and copy data to it
    GLuint ubo;
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, constantBufferDescription.GetBufferSize(),
                 constantBufferDescription.GetDataPtr(), GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Create handle to ubo object
    ConstantBufferHandle constantBufferHandle =
        (ConstantBufferHandle)StoreOpenGLObject(
            ubo, GLObjectType::ConstantBufferObject);

    // Find an empty binding point
    std::vector<ConstantBufferHandle>::iterator it =
        std::find(m_constantBufferBindingPoints.begin(),
                  m_constantBufferBindingPoints.end(), ConstantBufferHandle());

    // Binding point found, insert into binding point
    if (it != m_constantBufferBindingPoints.end())
    {
        *it = constantBufferHandle;
    }
    // Or add it to end if no empty constant buffer binding point found
    else
    {
        // Update iterator
        it = m_constantBufferBindingPoints.insert(
            m_constantBufferBindingPoints.end(), constantBufferHandle);
    }

    // Calculate binding point index
    unsigned int bindingPointIndex = it - m_constantBufferBindingPoints.begin();

    // Bind ubo to binding point
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPointIndex, ubo);

    return constantBufferHandle;
}

// ConstantBufferHandle
// GLRenderer::CreateConstantBuffer(ProgramHandle programHandle,
//                                  const std::string &constantBufferName,
//                                  const ConstantBuffer &constantBufferData)
// {
//     ConstantBufferHandle constantBufferHandle = ConstantBufferHandle();

//     GLuint program;
//     if (GetOpenGLObject(programHandle, GLObjectType::ProgramObject,
//     &program))
//     {
//         // From
//         //
//         https://www.packtpub.com/books/content/opengl-40-using-uniform-blocks-and-uniform-buffer-objects
//         // Get index of uniform block
//         GLuint blockIndex =
//             glGetUniformBlockIndex(program, constantBufferName.c_str());

//         // Get size of uniform block (GL size may differ from C++ size)
//         GLint blockSize;
//         glGetActiveUniformBlockiv(program, blockIndex,
//                                   GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

//         // Create our own temporary data store that matches the size and
//         // alignment of data that OpenGL expects
//         GLubyte *blockBuffer = (GLubyte *)malloc(blockSize);

//         // Number of members in constant buffer
//         const size_t numMembers = constantBufferData.GetNumMembers();

//         // Get names for each uniform block member
//         GLchar **names = (GLchar **)malloc(numMembers * sizeof(GLchar *));
//         for (unsigned int i = 0; i < numMembers; ++i)
//         {
//             names[i] = (GLchar *)constantBufferData.GetMemberName(i).c_str();
//         }

//         // Get index of each uniform block member
//         GLuint *indices = (GLuint *)malloc(numMembers * sizeof(GLuint));
//         glGetUniformIndices(program, numMembers, names, indices);

//         // Check that indices are found
//         bool shouldContinue = true;
//         for (unsigned int i = 0; i < numMembers; ++i)
//         {
//             if (indices[i] == GL_INVALID_INDEX)
//             {
//                 std::cerr
//                     << "GLRenderer::UpdateConstantBuffer: Found no "
//                        "uniform block member with name: "
//                     << names[i] << ". "
//                     << "Note: If this uniform block member exists in your "
//                        "shader but you aren't using it, "
//                        "OpenGL might be optimizing it away."
//                     << std::endl;
//                 shouldContinue = false;
//             }
//         }

//         if (shouldContinue)
//         {
//             // Get offset of each member in GL buffer
//             GLint *offset = (GLint *)malloc(numMembers * sizeof(GLint));
//             glGetActiveUniformsiv(program, numMembers, indices,
//                                   GL_UNIFORM_OFFSET, offset);

//             // Get offset of each member in C++ buffer and copy to correct
//             // place
//             // in GL buffer
//             const void *data = constantBufferData.GetData();
//             for (unsigned int i = 0; i < numMembers; ++i)
//             {
//                 memcpy(blockBuffer + offset[i],
//                        (uint8_t *)data +
//                        constantBufferData.GetMemberOffset(i),
//                        constantBufferData.GetMemberSize(i));
//             }

//             // Create OpenGL buffer object, copy data to it
//             GLuint ubo;
//             glGenBuffers(1, &ubo);
//             glBindBuffer(GL_UNIFORM_BUFFER, ubo);
//             glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer,
//                          GL_DYNAMIC_DRAW);

//             // Create handle to ubo object
//             constantBufferHandle = (ConstantBufferHandle)StoreOpenGLObject(
//                 ubo, GLObjectType::ConstantBufferObject);

//             // Find an empty binding point
//             std::vector<ConstantBufferHandle>::iterator it = std::find(
//                 m_constantBufferBindingPoints.begin(),
//                 m_constantBufferBindingPoints.end(), ConstantBufferHandle());

//             // Binding point found, insert into binding point
//             if (it != m_constantBufferBindingPoints.end())
//             {
//                 *it = constantBufferHandle;
//             }
//             // Or add it to end if no empty constant buffer binding point
//             // found
//             else
//             {
//                 // Update iterator
//                 it = m_constantBufferBindingPoints.insert(
//                     m_constantBufferBindingPoints.end(),
//                     constantBufferHandle);
//             }

//             // Calculate binding point index
//             unsigned int bindingPointIndex =
//                 it - m_constantBufferBindingPoints.begin();

//             // Bind ubo to binding point
//             glBindBufferBase(GL_UNIFORM_BUFFER, bindingPointIndex, ubo);

//             free(offset);
//         }

//         // Free temporary memory
//         free(indices);
//         free(names);
//         free(blockBuffer);
//     }
//     else
//     {
//         std::cerr << "GLRenderer::CreateConstantBuffer: invalid program
//         handle"
//                   << std::endl;
//     }

//     return constantBufferHandle;
// }

void GLRenderer::BindConstantBuffer(ProgramHandle programHandle,
                                    const std::string &constantBufferName,
                                    ConstantBufferHandle constantBufferHandle)
{
    // Find constant buffer object's binding point
    std::vector<ConstantBufferHandle>::iterator it =
        std::find(m_constantBufferBindingPoints.begin(),
                  m_constantBufferBindingPoints.end(), constantBufferHandle);

    // Binding point found
    if (it != m_constantBufferBindingPoints.end())
    {
        // Calculate binding point index
        unsigned int bindingPointIndex =
            it - m_constantBufferBindingPoints.begin();

        // Get program object
        GLuint program = 0;
        if (GetOpenGLObject(programHandle, GLObjectType::ProgramObject,
                            &program))
        {
            // Get constant buffer block index
            GLuint blockIndex =
                glGetUniformBlockIndex(program, constantBufferName.c_str());
            // Bind block index to binding point index, which ubo (constant
            // buffer) is also bound to
            glUniformBlockBinding(program, blockIndex, bindingPointIndex);
        }
        else
        {
            std::cerr << "GLRenderer::BindConstantBuffer: Failed to get "
                         "program associated with program handle provided."
                      << std::endl;
        }
    }
    else
    {
        std::cerr << "GLRenderer::BindConstantBuffer: Constant buffer hasn't "
                     "been bound to a binding point!"
                  << std::endl;
    }
}

void GLRenderer::UpdateConstantBufferData(
    ConstantBufferHandle constantBufferHandle,
    const ConstantBufferDescription &constantBufferDescription)
{
    GLuint ubo = 0;
    if (GetOpenGLObject(constantBufferHandle,
                        GLObjectType::ConstantBufferObject, &ubo))
    {
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER,
                     constantBufferDescription.GetBufferSize(),
                     constantBufferDescription.GetDataPtr(), GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    else
    {
        std::cerr << "GLRenderer::UpdateConstantBufferData: Failed to get "
                     "constant buffer associated with constant buffer handle "
                     "provided."
                  << std::endl;
    }
}

void GLRenderer::DrawVertices(VertexBufferHandle buffer,
                              PrimitiveType primitiveType,
                              size_t startingVertex,
                              size_t numVertices)
{
    BindVertexBuffer(buffer);
    glDrawArrays(ToGLPrimitiveType(primitiveType), startingVertex, numVertices);
    UnbindVertexBuffer();
}

void GLRenderer::DrawVerticesIndexed(VertexBufferHandle buffer,
                                     IndexBufferHandle indexBuffer,
                                     PrimitiveType primitiveType,
                                     size_t startingIndex,
                                     size_t numIndices)
{
    BindVertexBuffer(buffer);
    BindIndexBuffer(indexBuffer);
    glDrawElements(ToGLPrimitiveType(primitiveType), numIndices,
                   GL_UNSIGNED_INT, (unsigned int *)NULL + startingIndex);
    UnbindIndexBuffer();
    UnbindVertexBuffer();
}

ds::Handle GLRenderer::StoreOpenGLObject(GLuint glObject, GLObjectType type)
{
    // Construct GLObject
    GLObject obj;
    obj.object = glObject;

    // Insert object into vector so we can get it's address and pass it to
    // the
    // handle.
    m_openGLObjects.push_back(obj);
    // Location in vector of GLObject we just added
    size_t loc = m_openGLObjects.size() - 1;
    // Get address of GLObject and create handle from it
    ds::Handle handle =
        m_handleManager.Add((void *)&m_openGLObjects[loc], (uint32_t)type);
    // Store handle with GLObject
    m_openGLObjects[loc].handle = handle;

    // Because pushing an element back on the vector might cause the vector
    // to
    // realloc, and hence the address of all GLObjects in the vector change,
    // update the address of all handles.
    for (unsigned int i = 0; i < m_openGLObjects.size(); ++i)
    {
        ds::Handle handle = m_openGLObjects[i].handle;
        m_handleManager.Update(handle, &m_openGLObjects[i]);
    }

    return handle;
}

bool GLRenderer::GetOpenGLObject(ds::Handle handle,
                                 GLObjectType type,
                                 GLuint *openGLObject) const
{
    bool result = false;
    GLObject *objectTmp = nullptr;

    if (openGLObject != nullptr)
    {
        // If provided handle is of a different type than the one required,
        // fail
        if (handle.type != (int)type)
        {
            std::cerr << "GLRenderer::GetOpenGLObject: Type of handle provided "
                         "does not match required type."
                      << std::endl;
        }
        else
        {
            // If handle exists and is valid
            if (m_handleManager.Get(handle, (void **)&objectTmp))
            {
                // Success!
                *openGLObject = objectTmp->object;
                result = true;
            }
            else
            {
                std::cerr << "GLRenderer::GetOpenGLObject: Failed to "
                             "retrieve handle."
                          << std::endl;
            }
        }
    }

    return result;
}

void GLRenderer::BindVertexBuffer(VertexBufferHandle vertexBufferHandle)
{
    GLuint vao;
    if (GetOpenGLObject(vertexBufferHandle, GLObjectType::VertexArrayObject,
                        &vao))
    {
        glBindVertexArray(vao);
    }
    else
    {
        std::cerr
            << "GLRenderer::BindVertexBuffer: Failed to bind vertex buffer."
            << std::endl;
    }
}

void GLRenderer::UnbindVertexBuffer()
{
    glBindVertexArray(0);
}

void GLRenderer::BindIndexBuffer(IndexBufferHandle indexBufferHandle)
{
    GLuint ibo;
    if (GetOpenGLObject(indexBufferHandle, GLObjectType::IndexBufferObject,
                        &ibo))
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    }
    else
    {
        std::cerr << "GLRenderer::BindIndexBuffer: Failed to bind index buffer."
                  << std::endl;
    }
}

void GLRenderer::UnbindIndexBuffer()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLenum GLRenderer::ToGLBufferUsageType(BufferUsageType usage) const
{
    GLenum type = GL_INVALID_ENUM;

    switch (usage)
    {
    case BufferUsageType::Static:
        type = GL_STATIC_DRAW;
        break;
    case BufferUsageType::Dynamic:
        type = GL_DYNAMIC_DRAW;
        break;
    default:
        break;
    }

    return type;
}

GLenum GLRenderer::ToGLDataType(RenderDataType dataType) const
{
    GLenum type = GL_INVALID_ENUM;

    switch (dataType)
    {
    case RenderDataType::Int:
        type = GL_INT;
        break;
    case RenderDataType::Float:
        type = GL_FLOAT;
        break;
    case RenderDataType::UnsignedByte:
        type = GL_UNSIGNED_BYTE;
        break;
    }

    return type;
}

GLenum GLRenderer::ToGLBool(bool boolean) const
{
    return (boolean == true ? GL_TRUE : GL_FALSE);
}

GLenum GLRenderer::ToGLShaderType(ShaderType shaderType) const
{
    GLenum type = GL_INVALID_ENUM;

    switch (shaderType)
    {
    case ShaderType::VertexShader:
        type = GL_VERTEX_SHADER;
        break;
    case ShaderType::FragmentShader:
        type = GL_FRAGMENT_SHADER;
        break;
    default:
        break;
    }

    return type;
}

GLenum GLRenderer::ToGLPrimitiveType(PrimitiveType primitiveType) const
{
    GLenum type = GL_INVALID_ENUM;

    switch (primitiveType)
    {
    case PrimitiveType::Triangles:
        type = GL_TRIANGLES;
        break;
    case PrimitiveType::TriangleStrip:
        type = GL_TRIANGLE_STRIP;
        break;
    case PrimitiveType::Lines:
        type = GL_LINES;
        break;
    case PrimitiveType::Points:
        type = GL_POINTS;
        break;
    /** TODO: More... */
    default:
        break;
    }

    return type;
}

GLenum GLRenderer::ToGLImageFormat(ImageFormat imageFormat) const
{
    GLenum format = GL_INVALID_ENUM;

    switch (imageFormat)
    {
    case ImageFormat::RGB:
        format = GL_RGB;
        break;
    case ImageFormat::RGBA:
        format = GL_RGBA;
        break;
    default:
        break;
    }

    return format;
}

GLenum GLRenderer::ToGLInternalImageFormat(
    InternalImageFormat internalImageFormat) const
{
    GLenum format = GL_INVALID_ENUM;

    switch (internalImageFormat)
    {
    case InternalImageFormat::RGB8:
        format = GL_RGB8;
        break;
    case InternalImageFormat::SRGB8:
        format = GL_SRGB8;
        break;
    case InternalImageFormat::RGBA8:
        format = GL_RGBA8;
        break;
    case InternalImageFormat::SRGBA8:
        format = GL_SRGB8_ALPHA8;
        break;
    default:
        break;
    }

    return format;
}
}
