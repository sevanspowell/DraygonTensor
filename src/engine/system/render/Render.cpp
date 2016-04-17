#include <fstream>
#include <sstream>

#include <stb_image.h>

#include "engine/resource/MaterialResource.h"
#include "engine/resource/MeshResource.h"
#include "engine/resource/ShaderResource.h"
#include "engine/resource/TextureResource.h"
#include "engine/system/render/GLRenderer.h"
#include "engine/system/render/Render.h"
#include "math/Matrix4.h"
#include "math/Vector4.h"

namespace ds
{
bool Render::Initialize(const Config &config)
{
    bool result = true;

    // Register creators
    m_factory.RegisterCreator<MaterialResource>(
        MaterialResource::CreateFromFile);
    m_factory.RegisterCreator<MeshResource>(MeshResource::CreateFromFile);
    m_factory.RegisterCreator<ShaderResource>(ShaderResource::CreateFromFile);
    m_factory.RegisterCreator<TextureResource>(TextureResource::CreateFromFile);

    m_renderer =
        std::unique_ptr<ds_render::IRenderer>(new ds_render::GLRenderer());

    // TODO: Handle resize messages to change this
    unsigned int viewportWidth = 800;
    unsigned int viewportHeight = 600;

    m_renderer->Init(viewportWidth, viewportHeight);

    // Create vertex buffer data store
    ds_com::StreamBuffer vertexBufferStore;

    // Create position data
    vertexBufferStore << ds_math::Vector3(0.0f, 0.5f, 0.0f);
    vertexBufferStore << ds_math::Vector3(0.5f, -0.5f, 0.0f);
    vertexBufferStore << ds_math::Vector3(-0.5f, -0.5f, 0.0f);

    // Describe position data
    ds_render::VertexBufferDescription::AttributeDescription
        positionAttributeDescriptor;
    positionAttributeDescriptor.attributeType =
        ds_render::AttributeType::Position;
    positionAttributeDescriptor.attributeDataType =
        ds_render::RenderDataType::Float;
    positionAttributeDescriptor.numElementsPerAttribute = 3;
    positionAttributeDescriptor.stride = 0;
    positionAttributeDescriptor.offset = 0;
    positionAttributeDescriptor.normalized = false;

    // Create texCoord data
    vertexBufferStore << 0.5f;
    vertexBufferStore << 0.0f;
    vertexBufferStore << 1.0f;
    vertexBufferStore << 1.0f;
    vertexBufferStore << 0.0f;
    vertexBufferStore << 1.0f;

    // Describe texCoord data
    ds_render::VertexBufferDescription::AttributeDescription
        texCoordAttributeDescriptor;
    texCoordAttributeDescriptor.attributeType =
        ds_render::AttributeType::TextureCoordinate;
    texCoordAttributeDescriptor.attributeDataType =
        ds_render::RenderDataType::Float;
    texCoordAttributeDescriptor.numElementsPerAttribute = 2;
    texCoordAttributeDescriptor.stride = 0;
    texCoordAttributeDescriptor.offset = 3 * sizeof(ds_math::Vector3);
    texCoordAttributeDescriptor.normalized = false;

    // Add position and texcoord attribute descriptions to vertex buffer
    // descriptor
    ds_render::VertexBufferDescription vertexBufferDescriptor;
    vertexBufferDescriptor.AddAttributeDescription(positionAttributeDescriptor);
    vertexBufferDescriptor.AddAttributeDescription(texCoordAttributeDescriptor);

    // Create vertex buffer
    m_vb = m_renderer->CreateVertexBuffer(
        ds_render::BufferUsageType::Static, vertexBufferDescriptor,
        vertexBufferStore.AvailableBytes(), vertexBufferStore.GetDataPtr());

    // Create index data
    std::vector<unsigned int> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    // Create index buffer
    m_ib = m_renderer->CreateIndexBuffer(ds_render::BufferUsageType::Static,
                                         sizeof(unsigned int) * 3, &indices[0]);

    // Create shader program
    ds_render::ShaderHandle vs =
        m_renderer->CreateShaderObject(ds_render::ShaderType::VertexShader,
                                       strlen(m_vertexShader), m_vertexShader);
    ds_render::ShaderHandle fs = m_renderer->CreateShaderObject(
        ds_render::ShaderType::FragmentShader, strlen(m_fragmentShader),
        m_fragmentShader);

    std::vector<ds_render::ShaderHandle> shaders;
    shaders.push_back(vs);
    shaders.push_back(fs);
    m_program = m_renderer->CreateProgram(shaders);

    // Create shader data
    struct Scene
    {
        ds_math::Matrix4 modelMatrix;
        ds_math::Matrix4 viewMatrix;
        ds_math::Matrix4 projectionMatrix;
    };
    Scene sceneConstants;
    sceneConstants.modelMatrix = ds_math::Matrix4(1.0f);
    sceneConstants.viewMatrix = ds_math::Matrix4(1.0f);
    sceneConstants.projectionMatrix = ds_math::Matrix4(1.0f);

    // Describe shader data
    ds_render::ConstantBuffer cBuffer;
    cBuffer.SetData(sizeof(Scene), &sceneConstants);
    cBuffer.DescribeBufferMember("Scene.modelMatrix", 0,
                                 sizeof(ds_math::Matrix4));
    cBuffer.DescribeBufferMember("Scene.viewMatrix", sizeof(ds_math::Matrix4),
                                 sizeof(ds_math::Matrix4));
    cBuffer.DescribeBufferMember("Scene.projectionMatrix",
                                 2 * sizeof(ds_math::Matrix4),
                                 sizeof(ds_math::Matrix4));

    // Update shader data
    m_renderer->UpdateConstantBuffer(m_program, "Scene", cBuffer);

    // Load texture data
    int width = 0;
    int height = 0;
    int components = 0;
    unsigned char *imageContents =
        stbi_load("../assets/test.png", &width, &height, &components, 0);

    // Create texture
    m_texture = m_renderer->Create2DTexture(
        ds_render::ImageFormat::RGB, ds_render::RenderDataType::UnsignedByte,
        ds_render::InternalImageFormat::SRGB8, true, width, height,
        imageContents);

    return result;
}

void Render::Update(float deltaTime)
{
    ProcessEvents(&m_messagesReceived);

    m_renderer->ClearBuffers();

    m_renderer->SetProgram(m_program);

    m_renderer->BindTextureToSampler(m_program, "tex", m_texture);

    // m_renderer->DrawVertices(m_vb, ds_render::PrimitiveType::Triangles, 0,
    // 3);
    m_renderer->DrawVerticesIndexed(m_vb, m_ib,
                                    ds_render::PrimitiveType::Triangles, 0, 3);
}

void Render::Shutdown()
{
}

void Render::PostMessages(const ds_msg::MessageStream &messages)
{
    AppendStreamBuffer(m_messagesReceived, messages);
}

ds_msg::MessageStream Render::CollectMessages()
{
    ds_msg::MessageStream tmp = m_messagesGenerated;

    m_messagesGenerated.Clear();

    return tmp;
}

void Render::ProcessEvents(ds_msg::MessageStream *messages)
{
    while (messages->AvailableBytes() != 0)
    {
        ds_msg::MessageHeader header;
        (*messages) >> header;

        switch (header.type)
        {
        // case ds_msg::MessageType::GraphicsContextCreated:
        //     ds_msg::GraphicsContextCreated gfxContext;
        //     (*messages) >> gfxContext;

        //     // If we haven't already created a renderer
        //     if (m_renderer->== nullptr)
        //     {
        //         // Create a renderer to match graphics context type
        //         // GL renderer for GL context, etc...
        //         if (gfxContext.contextInfo.type =
        //                 GraphicsContext::ContextType::OpenGL)
        //         {
        //         }
        //     }
        //     break;
        case ds_msg::MessageType::CreateComponent:
        {
            ds_msg::CreateComponent createComponentMsg;
            (*messages) >> createComponentMsg;

            // Load up component data for component
            Config componentData;
            if (componentData.LoadMemory(StringIntern::Instance().GetString(
                    createComponentMsg.componentData)))
            {
                if (StringIntern::Instance().GetString(
                        createComponentMsg.componentType) == "renderComponent")
                {
                    std::string meshName;
                    std::string materialName;
                    if (componentData.GetString("mesh", &meshName) &&
                        componentData.GetString("material", &materialName))
                    {
                        // Get mesh resource path
                        std::stringstream meshResourcePath;
                        meshResourcePath << "../assets/" << meshName;

                        // Get mesh resource
                        std::unique_ptr<MeshResource> meshResource =
                            m_factory.CreateResource<MeshResource>(
                                meshResourcePath.str());

                        // TODO: Renderer create mesh

                        // Get material resource path
                        std::stringstream materialResourcePath;
                        materialResourcePath << "../assets/" << materialName;

                        // Get material resource
                        std::unique_ptr<MaterialResource> materialResource =
                            m_factory.CreateResource<MaterialResource>(
                                materialResourcePath.str());

                        // TODO: Renderer create material

                        // Create render Component for entity
                    }
                }
            }
            break;
        }
        default:
            messages->Extract(header.size);
            break;
        }
    }
}
}
