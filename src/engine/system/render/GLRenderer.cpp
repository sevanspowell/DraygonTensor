#include <iostream>

#include "engine/system/render/GLRenderer.h"

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
        vbo, GLObjectType::VertexArrayObject));
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
                         size_t startingVertex,
                         size_t numVertices)
{
}

ds::Handle GLRenderer::StoreOpenGLObject(GLuint glObject, GLObjectType type)
{
    // Construct GLObject
    GLObject obj;
    obj.object = glObject;

    // Insert object into vector so we can get it's address and pass it to the
    // handle.
    m_openGLObjects.push_back(obj);
    // Location in vector of GLObject we just added
    size_t loc = m_openGLObjects.size() - 1;
    // Get address of GLObject and create handle from it
    ds::Handle handle =
        m_handleManager.Add((void *)&m_openGLObjects[loc], (uint32_t)type);
    // Store handle with GLObject
    m_openGLObjects[loc].handle = handle;

    // Because pushing an element back on the vector might cause the vector to
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
        // If provided handle is of a different type than the one required, fail
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
                std::cerr
                    << "GLRenderer::GetOpenGLObject: Failed to retrieve handle."
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
}
