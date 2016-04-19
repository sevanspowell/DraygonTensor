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

    return result;
}

void Render::Update(float deltaTime)
{
    ProcessEvents(&m_messagesReceived);

    // Make sure renderer has been created
    if (m_renderer != nullptr)
    {
        m_renderer->ClearBuffers();

        RenderScene();
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
        case ds_msg::MessageType::GraphicsContextCreated:
        {
            ds_msg::GraphicsContextCreated gfxContext;
            (*messages) >> gfxContext;

            // If we haven't already created a renderer
            if (m_renderer == nullptr)
            {
                // Create a renderer to match graphics context type
                // GL renderer for GL context, etc...
                switch (gfxContext.contextInfo.type)
                {
                case ds_platform::GraphicsContext::ContextType::OpenGL:
                {
                    m_renderer = std::unique_ptr<ds_render::IRenderer>(
                        new ds_render::GLRenderer());

                    // TODO: Handle resize messages to change this
                    unsigned int viewportWidth = 800;
                    unsigned int viewportHeight = 600;

                    m_renderer->Init(viewportWidth, viewportHeight);

                    // Need a program to get information about Scene and Object
                    // constant
                    // buffers, so create a "fake" one.
                    // Create shader program
                    std::unique_ptr<ShaderResource> shaderResource =
                        m_factory.CreateResource<ShaderResource>(
                            "../assets/constantBuffer.shader");

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
                            shaderType, shaderSource.size(),
                            shaderSource.c_str()));
                        std::cout << shaderSource << std::endl;
                    }
                    // Compile shaders into shader program
                    ds_render::ProgramHandle fakeShader =
                        m_renderer->CreateProgram(shaders);

                    // Get shader data descriptions
                    m_sceneBufferDescrip.AddMember("Scene.viewMatrix");
                    m_sceneBufferDescrip.AddMember("Scene.projectionMatrix");
                    m_renderer->GetConstantBufferDescription(
                        fakeShader, "Scene", &m_sceneBufferDescrip);

                    m_objectBufferDescrip.AddMember("Object.modelMatrix");
                    m_renderer->GetConstantBufferDescription(
                        fakeShader, "Object", &m_objectBufferDescrip);


                    // Create shader data
                    m_viewMatrix = ds_math::Matrix4(1.0f);
                    m_projectionMatrix =
                        ds_math::Matrix4::CreatePerspectiveFieldOfView(
                            ds_math::MathHelper::PI / 3.0f, 800.0f / 600.0f,
                            0.1f, 100.0f);
                    m_sceneBufferDescrip.InsertMemberData(
                        "Scene.viewMatrix", sizeof(ds_math::Matrix4),
                        &m_viewMatrix);
                    m_sceneBufferDescrip.InsertMemberData(
                        "Scene.projectionMatrix", sizeof(ds_math::Matrix4),
                        &m_projectionMatrix);

                    ds_math::Matrix4 modelMatrix = ds_math::Matrix4(1.0f);
                    m_objectBufferDescrip.InsertMemberData(
                        "Object.modelMatrix", sizeof(ds_math::Matrix4),
                        &modelMatrix);

                    m_sceneMatrices =
                        m_renderer->CreateConstantBuffer(m_sceneBufferDescrip);
                    m_objectMatrices =
                        m_renderer->CreateConstantBuffer(m_objectBufferDescrip);

                    break;
                }
                default:
                    break;
                }
            }
            break;
        }
        case ds_msg::MessageType::CreateComponent:
        {
            ds_msg::CreateComponent createComponentMsg;
            (*messages) >> createComponentMsg;

            // Load up component data for component
            Config componentData;
            if (componentData.LoadMemory(StringIntern::Instance().GetString(
                    createComponentMsg.componentData)))
            {
                std::string componentType = StringIntern::Instance().GetString(
                    createComponentMsg.componentType);
                std::cout << componentType << std::endl;
                std::cout << StringIntern::Instance().GetString(
                    createComponentMsg.componentData);
                if (componentType == "renderComponent")
                {
                    std::string meshName;
                    std::string materialName;
                    if (componentData.GetString("mesh", &meshName) &&
                        componentData.GetString("material", &materialName))
                    {
                        // Get mesh resource path
                        std::stringstream meshResourcePath;
                        meshResourcePath << "../assets/" << meshName;

                        // Get material resource path
                        std::stringstream materialResourcePath;
                        materialResourcePath << "../assets/" << materialName;

                        // Create Mesh
                        ds_render::Mesh mesh =
                            CreateMeshFromMeshResource(meshResourcePath.str());

                        // Create material
                        ds_render::Material material =
                            CreateMaterialFromMaterialResource(
                                materialResourcePath.str(), m_sceneMatrices,
                                m_objectMatrices);

                        Instance i =
                            m_renderComponentManager.CreateComponentForEntity(
                                createComponentMsg.entity);
                        m_renderComponentManager.SetMaterial(i, material);
                        m_renderComponentManager.SetMesh(i, mesh);
                    }
                }
                else if (componentType == "transformComponent")
                {
                    std::vector<float> position;
                    std::vector<float> orientation;
                    std::vector<float> scale;

                    if (componentData.GetFloatArray("position", &position) &&
                        componentData.GetFloatArray("orientation",
                                                    &orientation) &&
                        componentData.GetFloatArray("scale", &scale))
                    {
                        // Get entity
                        Entity entity = createComponentMsg.entity;

                        // Create component for entity
                        Instance instance =
                            m_transformComponentManager
                                .CreateComponentForEntity(entity);

                        // Transform position, rotation and scale into a single
                        // matrix
                        ds_math::Matrix4 mat =
                            ds_math::Matrix4::CreateTranslationMatrix(
                                position[0], position[1], position[2]) *
                            ds_math::Matrix4::CreateFromQuaternion(
                                ds_math::Quaternion(
                                    orientation[0], orientation[1],
                                    orientation[2], orientation[3])) *
                            ds_math::Matrix4::CreateScaleMatrix(
                                scale[0], scale[1], scale[2]);


                        m_transformComponentManager.SetLocalTransform(instance,
                                                                      mat);
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

ds_render::Material Render::CreateMaterialFromMaterialResource(
    const std::string &filePath,
    ds_render::ConstantBufferHandle sceneMatrices,
    ds_render::ConstantBufferHandle objectMatrices)
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

    // Bind constant buffers to program
    m_renderer->BindConstantBuffer(material.GetProgram(), "Scene",
                                   sceneMatrices);
    m_renderer->BindConstantBuffer(material.GetProgram(), "Object",
                                   objectMatrices);

    return material;
}

void Render::RenderScene()
{
    // Update scene constant buffer
    m_sceneBufferDescrip.InsertMemberData(
        "Scene.viewMatrix", sizeof(ds_math::Matrix4), &m_viewMatrix);
    m_sceneBufferDescrip.InsertMemberData("Scene.projectionMatrix",
                                          sizeof(ds_math::Matrix4),
                                          &m_projectionMatrix);
    m_renderer->UpdateConstantBufferData(m_sceneMatrices, m_sceneBufferDescrip);

    // For each render component
    for (unsigned int i = 0; i < m_renderComponentManager.GetNumInstances();
         ++i)
    {
        Instance renderInstance = Instance::MakeInstance(i);
        // Get transform component
        Entity entity =
            m_renderComponentManager.GetEntityForInstance(renderInstance);
        Instance transformInstance =
            m_transformComponentManager.GetInstanceForEntity(entity);

        // If has transform instance
        if (transformInstance.IsValid())
        {
            // Update object constant buffer with world transform of this
            // transform instance
            m_objectBufferDescrip.InsertMemberData(
                "Object.modelMatrix", sizeof(ds_math::Matrix4),
                &m_transformComponentManager.GetWorldTransform(
                    transformInstance));
            m_renderer->UpdateConstantBufferData(m_objectMatrices,
                                                 m_objectBufferDescrip);
        }

        // Get mesh
        ds_render::Mesh mesh = m_renderComponentManager.GetMesh(renderInstance);
        // Get material
        ds_render::Material material =
            m_renderComponentManager.GetMaterial(renderInstance);

        // Set shader program
        m_renderer->SetProgram(material.GetProgram());

        // For each texture in material, bind it to shader
        for (auto samplerTexture : material.GetTextures())
        {
            m_renderer->BindTextureToSampler(
                material.GetProgram(), samplerTexture.first,
                samplerTexture.second.GetTextureHandle());
        }

        // Draw mesh
        m_renderer->DrawVerticesIndexed(
            mesh.GetVertexBuffer(), mesh.GetIndexBuffer(),
            ds_render::PrimitiveType::Triangles, mesh.GetStartingIndex(),
            mesh.GetNumIndices());

        // For each texture in material, unbind
        for (auto samplerTexture : material.GetTextures())
        {
            m_renderer->UnbindTextureFromSampler(
                samplerTexture.second.GetTextureHandle());
        }
    }
}
}
