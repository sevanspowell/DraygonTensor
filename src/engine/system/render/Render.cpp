#include <fstream>
#include <sstream>

#include <stb_image.h>

#include "engine/resource/MaterialResource.h"
#include "engine/resource/MeshResource.h"
#include "engine/resource/ShaderResource.h"
#include "engine/resource/TextureResource.h"
#include "engine/system/render/GLRenderer.h"
#include "engine/system/render/Render.h"
#include "math/MathHelper.h"
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

    // Create Mesh
    m_mesh = CreateMeshFromMeshResource("../assets/cube.obj");

    // Create material
    m_material = CreateMaterialFromMaterialResource("../assets/test.material");

    Entity testEntity;
    testEntity.id = 0;
    Instance i = m_renderComponentManager.CreateComponentForEntity(testEntity);
    m_renderComponentManager.SetMaterial(i, m_material);
    m_renderComponentManager.SetMesh(i, m_mesh);

    // Get shader data descriptions
    m_sceneBufferDescrip.AddMember("Scene.viewMatrix");
    m_sceneBufferDescrip.AddMember("Scene.projectionMatrix");
    m_renderer->GetConstantBufferDescription(m_material.GetProgram(), "Scene",
                                             &m_sceneBufferDescrip);

    m_objectBufferDescrip.AddMember("Object.modelMatrix");
    m_renderer->GetConstantBufferDescription(m_material.GetProgram(), "Object",
                                             &m_objectBufferDescrip);

    // Create shader data
    ds_math::Matrix4 viewMatrix = ds_math::Matrix4(1.0f);
    ds_math::Matrix4 projectionMatrix =
        ds_math::Matrix4::CreatePerspectiveFieldOfView(
            ds_math::MathHelper::PI / 3.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    m_sceneBufferDescrip.InsertMemberData(
        "Scene.viewMatrix", sizeof(ds_math::Matrix4), &viewMatrix);
    m_sceneBufferDescrip.InsertMemberData(
        "Scene.projectionMatrix", sizeof(ds_math::Matrix4), &projectionMatrix);

    ds_math::Matrix4 modelMatrix =
        ds_math::Matrix4::CreateTranslationMatrix(-4.0f, -3.0f, -10.0f);
    m_objectBufferDescrip.InsertMemberData(
        "Object.modelMatrix", sizeof(ds_math::Matrix4), &modelMatrix);


    m_sceneMatrices = m_renderer->CreateConstantBuffer(m_sceneBufferDescrip);
    m_renderer->BindConstantBuffer(m_material.GetProgram(), "Scene",
                                   m_sceneMatrices);

    m_objectMatrices = m_renderer->CreateConstantBuffer(m_objectBufferDescrip);
    m_renderer->BindConstantBuffer(m_material.GetProgram(), "Object",
                                   m_objectMatrices);

    return result;
}

void Render::Update(float deltaTime)
{
    ProcessEvents(&m_messagesReceived);

    m_renderer->ClearBuffers();

    // For each render component instance
    for (unsigned int i = 0; i < m_renderComponentManager.GetNumInstances();
         ++i)
    {
        Instance instance = Instance::MakeInstance(i);

        // Set shader program
        m_renderer->SetProgram(
            m_renderComponentManager.GetMaterial(instance).GetProgram());

        // For each texture in material, bind it to shader
        for (auto samplerTexture : m_material.GetTextures())
        {
            m_renderer->BindTextureToSampler(
                m_material.GetProgram(), samplerTexture.first,
                samplerTexture.second.GetTextureHandle());
        }

        // Draw mesh
        m_renderer->DrawVerticesIndexed(
            m_mesh.GetVertexBuffer(), m_mesh.GetIndexBuffer(),
            ds_render::PrimitiveType::Triangles, m_mesh.GetStartingIndex(),
            m_mesh.GetNumIndices());

        // For each texture in material, unbind
        for (auto samplerTexture : m_material.GetTextures())
        {
            m_renderer->UnbindTextureFromSampler(
                samplerTexture.second.GetTextureHandle());
        }
    }
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

                        std::cout << meshResourcePath.str() << std::endl;

                        // Get mesh resource
                        std::unique_ptr<MeshResource> meshResource =
                            m_factory.CreateResource<MeshResource>(
                                meshResourcePath.str());

                        // std::cout << "Index count: "
                        //           << meshResource->GetIndicesCount()
                        //           << std::endl;
                        // // TODO: Renderer create mesh
                        // // Create vertex buffer data store
                        // ds_com::StreamBuffer vertexBufferDataStore;

                        // // Get position data
                        // const std::vector<ds_math::Vector3> positions =
                        //     meshResource->GetVerts();
                        // for (const ds_math::Vector3 &position : positions)
                        // {
                        //     std::cout << position << std::endl;
                        //     vertexBufferDataStore << position;
                        // }

                        // // Describe position data
                        // ds_render::VertexBufferDescription::AttributeDescription
                        //     positionAttributeDescriptor;
                        // positionAttributeDescriptor.attributeType =
                        //     ds_render::AttributeType::Position;
                        // positionAttributeDescriptor.attributeDataType =
                        //     ds_render::RenderDataType::Float;
                        // positionAttributeDescriptor.numElementsPerAttribute =
                        // 3;
                        // positionAttributeDescriptor.stride = 0;
                        // positionAttributeDescriptor.offset = 0;
                        // positionAttributeDescriptor.normalized = false;

                        // // // Get texture coordinate data
                        // // const std::vector<ds_math::Vector3>
                        // // textureCoordinates =
                        // //     meshResource->GetTexCoords();
                        // // for (const ds_math::Vector3 &texCoord :
                        // //      textureCoordinates)
                        // // {
                        // //     vertexBufferDataStore << texCoord.x;
                        // //     vertexBufferDataStore << texCoord.y;
                        // // }

                        // // // Describe texCoord data
                        // //
                        // ds_render::VertexBufferDescription::AttributeDescription
                        // //     texCoordAttributeDescriptor;
                        // // texCoordAttributeDescriptor.attributeType =
                        // //     ds_render::AttributeType::TextureCoordinate;
                        // // texCoordAttributeDescriptor.attributeDataType =
                        // //     ds_render::RenderDataType::Float;
                        // //
                        // texCoordAttributeDescriptor.numElementsPerAttribute =
                        // // 2;
                        // // texCoordAttributeDescriptor.stride = 0;
                        // // texCoordAttributeDescriptor.offset =
                        // //     meshResource->GetVertCount() *
                        // //     sizeof(ds_math::Vector3);
                        // // texCoordAttributeDescriptor.normalized = false;

                        // // Create vertex buffer descriptor
                        // ds_render::VertexBufferDescription
                        //     vertexBufferDescriptor;
                        // vertexBufferDescriptor.AddAttributeDescription(
                        //     positionAttributeDescriptor);
                        // // vertexBufferDescriptor.AddAttributeDescription(
                        // //     texCoordAttributeDescriptor);

                        // // Create vertex buffer
                        // // m_vb = m_renderer->CreateVertexBuffer(
                        // //     ds_render::BufferUsageType::Static,
                        // //     vertexBufferDescriptor,
                        // //     vertexBufferDataStore.AvailableBytes(),
                        // //     vertexBufferDataStore.GetDataPtr());

                        // std::cout << "Bad: " << std::endl;
                        // while (vertexBufferDataStore.AvailableBytes() > 0)
                        // {
                        //     ds_math::Vector3 v;
                        //     vertexBufferDataStore >> v;
                        //     std::cout << v << std::endl;
                        // }

                        // // Create index buffer
                        // std::vector<unsigned int> indices =
                        //     meshResource->GetIndices();
                        // for (auto index : indices)
                        // {
                        //     std::cout << index << std::endl;
                        // }
                        // m_ib = m_renderer->CreateIndexBuffer(
                        //     ds_render::BufferUsageType::Static,
                        //     sizeof(unsigned int) * indices.size(),
                        //     &indices[0]);

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

ds_render::Texture
Render::CreateTextureFromTextureResource(const std::string &filePath)
{
    // Texture from texture resource
    std::unique_ptr<TextureResource> textureResource =
        m_factory.CreateResource<TextureResource>(filePath);

    ds_render::ImageFormat format;
    switch (textureResource->GetComponentFlag())
    {
    case TextureResource::ComponentFlag::GREY:
        format = ds_render::ImageFormat::R;
        break;
    case TextureResource::ComponentFlag::GREYALPHA:
        format = ds_render::ImageFormat::RG;
        break;
    case TextureResource::ComponentFlag::RGB:
        format = ds_render::ImageFormat::RGB;
        break;
    case TextureResource::ComponentFlag::RGBA:
        format = ds_render::ImageFormat::RGBA;
        break;
    default:
        assert("Unsupported image component flag.");
        format = ds_render::ImageFormat::RGBA;
        break;
    }

    // Create texture
    ds_render::Texture texture = ds_render::Texture(m_renderer->Create2DTexture(
        format, ds_render::RenderDataType::UnsignedByte,
        ds_render::InternalImageFormat::RGBA8, true,
        textureResource->GetWidthInPixels(),
        textureResource->GetHeightInPixels(),
        textureResource->GetTextureContents()));

    return texture;
}

ds_render::Mesh Render::CreateMeshFromMeshResource(const std::string &filePath)
{
    std::unique_ptr<MeshResource> meshResource =
        m_factory.CreateResource<MeshResource>(filePath);

    // Create vertex buffer data store
    ds_com::StreamBuffer vertexBufferStore;

    const std::vector<ds_math::Vector3> positions = meshResource->GetVerts();
    for (const ds_math::Vector3 &position : positions)
    {
        vertexBufferStore << position;
    }

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
    // Get texture coordinate data
    const std::vector<ds_math::Vector3> textureCoordinates =
        meshResource->GetTexCoords();
    for (const ds_math::Vector3 &texCoord : textureCoordinates)
    {
        vertexBufferStore << texCoord.x;
        // Flip y texcoord
        vertexBufferStore << 1.0f - texCoord.y;
    }

    // Describe texCoord data
    ds_render::VertexBufferDescription::AttributeDescription
        texCoordAttributeDescriptor;
    texCoordAttributeDescriptor.attributeType =
        ds_render::AttributeType::TextureCoordinate;
    texCoordAttributeDescriptor.attributeDataType =
        ds_render::RenderDataType::Float;
    texCoordAttributeDescriptor.numElementsPerAttribute = 2;
    texCoordAttributeDescriptor.stride = 0;
    texCoordAttributeDescriptor.offset =
        meshResource->GetVertCount() * sizeof(ds_math::Vector3);
    texCoordAttributeDescriptor.normalized = false;

    // Add position and texcoord attribute descriptions to vertex buffer
    // descriptor
    ds_render::VertexBufferDescription vertexBufferDescriptor;
    vertexBufferDescriptor.AddAttributeDescription(positionAttributeDescriptor);
    vertexBufferDescriptor.AddAttributeDescription(texCoordAttributeDescriptor);

    // Create vertex buffer
    ds_render::VertexBufferHandle vb = m_renderer->CreateVertexBuffer(
        ds_render::BufferUsageType::Static, vertexBufferDescriptor,
        vertexBufferStore.AvailableBytes(), vertexBufferStore.GetDataPtr());

    // Create index buffer
    std::vector<unsigned int> indices = meshResource->GetIndices();

    // Create index buffer
    ds_render::IndexBufferHandle ib = m_renderer->CreateIndexBuffer(
        ds_render::BufferUsageType::Static,
        sizeof(unsigned int) * indices.size(), &indices[0]);

    // Create Mesh
    return ds_render::Mesh(vb, ib, 0, meshResource->GetIndicesCount());
}

ds_render::Material
Render::CreateMaterialFromMaterialResource(const std::string &filePath)
{
    ds_render::Material material;

    // Generate material resource
    std::unique_ptr<MaterialResource> materialResource =
        m_factory.CreateResource<MaterialResource>(filePath);

    // Create shader program
    std::unique_ptr<ShaderResource> shaderResource =
        m_factory.CreateResource<ShaderResource>(
            materialResource->GetShaderResourceFilePath());

    // Load each shader
    std::vector<ds_render::ShaderHandle> shaders;
    std::vector<ds_render::ShaderType> shaderTypes =
        shaderResource->GetShaderTypes();
    for (auto shaderType : shaderTypes)
    {
        const std::string &shaderSource =
            shaderResource->GetShaderSource(shaderType);

        // Append shader to list
        shaders.push_back(m_renderer->CreateShaderObject(
            shaderType, shaderSource.size(), shaderSource.c_str()));
    }
    // Compile shaders into shader program
    ds_render::ProgramHandle shaderProgram = m_renderer->CreateProgram(shaders);

    // Set shader program of material
    material.SetProgram(shaderProgram);

    // Create each texture and add to material
    std::vector<std::string> textureSamplerNames =
        materialResource->GetTextureSamplerNames();
    for (auto samplerName : textureSamplerNames)
    {
        // Create texture from texture resource
        const std::string &textureResourceFilePath =
            materialResource->GetTextureResourceFilePath(samplerName);

        material.AddTexture(samplerName, CreateTextureFromTextureResource(
                                             textureResourceFilePath));
    }

    return material;
}
}
