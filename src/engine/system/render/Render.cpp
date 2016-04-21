#include <fstream>
#include <sstream>

#include "engine/resource/MaterialResource.h"
#include "engine/resource/MeshResource.h"
#include "engine/resource/ShaderResource.h"
#include "engine/resource/TextureResource.h"
#include "engine/resource/TerrainResource.h"
#include "engine/system/render/GLRenderer.h"
#include "engine/system/render/Render.h"
#include "math/MathHelper.h"
#include "math/Matrix4.h"
#include "math/Vector4.h"

namespace ds_lua
{
extern ds::ScriptBindingSet LoadRenderScriptBindings();
}

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
    m_factory.RegisterCreator<TerrainResource>(TerrainResource::CreateFromFile);


    m_cameraActive = false;

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

ScriptBindingSet Render::GetScriptBindings() const
{
    return ds_lua::LoadRenderScriptBindings();
}

ds_math::Quaternion Render::GetCameraOrientation(Entity entity) const
{
    ds_math::Quaternion orientation;

    Instance i = m_cameraComponentManager.GetInstanceForEntity(entity);

    if (i.IsValid())
    {
        orientation = m_cameraComponentManager.GetOrientation(i);
    }

    return orientation;
}

void Render::SetCameraOrientation(Entity entity,
                                  const ds_math::Quaternion &orientation)
{
    Instance i = m_cameraComponentManager.GetInstanceForEntity(entity);

    if (i.IsValid())
    {
        m_cameraComponentManager.SetOrientation(i, orientation);
    }
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
                    }
                    // Compile shaders into shader program
                    ds_render::ProgramHandle fakeShader =
                        m_renderer->CreateProgram(shaders);

                    // Set shader data descriptions
                    m_sceneBufferDescrip = ds_render::ConstantBufferDescription(
                        2 * sizeof(ds_math::Matrix4));
                    m_sceneBufferDescrip.AddMember("Scene.viewMatrix");
                    m_sceneBufferDescrip.AddMember("Scene.projectionMatrix");
                    m_renderer->GetConstantBufferDescription(
                        fakeShader, "Scene", &m_sceneBufferDescrip);

                    m_objectBufferDescrip =
                        ds_render::ConstantBufferDescription(
                            1 * sizeof(ds_math::Matrix4));
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
                // Get component type
                std::string componentType = StringIntern::Instance().GetString(
                    createComponentMsg.componentType);
                // Create render component
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
                // Create transform component
                else if (componentType == "transformComponent")
                {
                    TransformComponentManager::
                        CreateComponentForEntityFromConfig(
                            &m_transformComponentManager,
                            createComponentMsg.entity, componentData);
                }
                // Create camera component
                else if (componentType == "cameraComponent")
                {
                    std::string projectionType;
                    float verticalFov = 0.0f;
                    float nearClip = 0.0f;
                    float farClip = 0.0f;

                    if (componentData.GetString("projection",
                                                &projectionType) &&
                        componentData.GetFloat("vertical_fov", &verticalFov) &&
                        componentData.GetFloat("near_clip", &nearClip) &&
                        componentData.GetFloat("far_clip", &farClip))
                    {
                        ds_math::Matrix4 projectionMatrix;

                        if (projectionType == "perspective")
                        {
                            projectionMatrix =
                                ds_math::Matrix4::CreatePerspectiveFieldOfView(
                                    verticalFov, 800.0f / 600.0f, nearClip,
                                    farClip);
                        }
                        else if (projectionType == "orthographic")
                        {
                            projectionMatrix =
                                ds_math::Matrix4::CreateOrthographic(
                                    800.0f, 600.0f, nearClip, farClip);
                        }

                        Entity e = createComponentMsg.entity;
                        Instance i =
                            m_cameraComponentManager.CreateComponentForEntity(
                                e);
                        m_cameraComponentManager.SetProjectionMatrix(
                            i, projectionMatrix);

                        // Is any camera currently active?
                        if (m_cameraActive == false)
                        {
                            // If no camera is current, set this camera to be
                            // current camera
                            m_activeCameraEntity = e;
                            m_cameraActive = true;
                        }
                    }
                }
                else if (componentType == "terrainComponent")
                {
                    std::string heightMapName;
                    std::string materialName;

                    if (componentData.GetString("heightmap", &heightMapName) &&
                        componentData.GetString("material", &materialName))
                    {
                        std::stringstream heightMapPath;
                        heightMapPath << "../assets/" << heightMapName;

                        std::unique_ptr<TerrainResource> terrainResource =
                            m_factory.CreateResource<TerrainResource>(
                                heightMapPath.str());

                        std::stringstream materialResourcePath;
                        materialResourcePath << "../assets/" << materialName;

                        ds_render::Material material =
                            CreateMaterialFromMaterialResource(
                                materialResourcePath.str(), m_sceneMatrices,
                                m_objectMatrices);
                        // above function does this
                        // std::unique_ptr<MaterialResource> materialResource =
                        // m_factory.CreateResource<materialResource>(materialResourcePath.str());

                        // Create vertex buffer data store

                        ds_com::StreamBuffer vertexBufferStore;

                        const std::vector<ds_math::Vector3> positions =
                            terrainResource->GetVerticesVector();

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

                        // // Create texCoord data

                        // // Get texture coordinate data

                        // meshresource change to terrainresource

                        const std::vector<
                            struct TerrainResource::TextureCoordinates>
                            textureCoordinates =
                                terrainResource->GetTextureCoordinatesVector();

                        for (const struct TerrainResource::TextureCoordinates
                                 &texCoord : textureCoordinates)
                        {
                            vertexBufferStore << texCoord.u;

                            // Flip y texcoord
                            vertexBufferStore
                                << 1.0f - texCoord.v; // removed 1 - texcoord.v
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
                            terrainResource->GetVerticesCount() *
                            sizeof(ds_math::Vector3);

                        texCoordAttributeDescriptor.normalized = false;

                        // Add position and texcoord attribute descriptions to

                        // vertex buffer
                        // descriptor

                        ds_render::VertexBufferDescription
                            vertexBufferDescriptor;

                        vertexBufferDescriptor.AddAttributeDescription(
                            positionAttributeDescriptor);

                        // for texture
                        vertexBufferDescriptor.AddAttributeDescription(
                            texCoordAttributeDescriptor);

                        // Create vertex buffer

                        ds_render::VertexBufferHandle vb =
                            m_renderer->CreateVertexBuffer(
                                ds_render::BufferUsageType::Static,
                                vertexBufferDescriptor,
                                vertexBufferStore.AvailableBytes(),
                                vertexBufferStore.GetDataPtr());

                        // Create index buffer

                        std::vector<int> indices =
                            terrainResource->GetIndicesVector();

                        // Create index buffer

                        ds_render::IndexBufferHandle ib =
                            m_renderer->CreateIndexBuffer(
                                ds_render::BufferUsageType::Static,
                                sizeof(unsigned int) * indices.size(),
                                &indices[0]);

                        ds_render::Mesh mesh =
                            ds_render::Mesh(vb, ib, 0, indices.size());

                        Instance i =
                            m_renderComponentManager.CreateComponentForEntity(
                                createComponentMsg.entity);

                        m_renderComponentManager.SetMaterial(i, material);
                        m_renderComponentManager.SetMesh(i, mesh);
                    }
                }
            }
            break;
        }
        case ds_msg::MessageType::MoveEntity:
        {
            ds_msg::MoveEntity entityMoveMsg;
            (*messages) >> entityMoveMsg;

            Instance transform =
                m_transformComponentManager.GetInstanceForEntity(
                    entityMoveMsg.entity);

            if (transform.IsValid())
            {
                // Get current transform
                const ds_math::Matrix4 &currentTransform =
                    m_transformComponentManager.GetLocalTransform(transform);
                // Translate it
                ds_math::Matrix4 newTransform =
                    currentTransform *
                    ds_math::Matrix4::CreateTranslationMatrix(
                        entityMoveMsg.deltaPosition);

                // Set transform of entity
                m_transformComponentManager.SetLocalTransform(transform,
                                                              newTransform);
            }
            break;
        }
        case ds_msg::MessageType::SetLocalTransform:
        {
            ds_msg::SetLocalTransform setLocalMsg;
            (*messages) >> setLocalMsg;

            Instance transform =
                m_transformComponentManager.GetInstanceForEntity(
                    setLocalMsg.entity);

            if (transform.IsValid())
            {
                // Set transform of entity
                m_transformComponentManager.SetLocalTransform(
                    transform, setLocalMsg.localTransform);
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
    // If there is a camera in the scene
    if (m_cameraActive)
    {
        // Get active camera
        Instance cameraTransform =
            m_transformComponentManager.GetInstanceForEntity(
                m_activeCameraEntity);
        Instance cameraComponent =
            m_cameraComponentManager.GetInstanceForEntity(m_activeCameraEntity);

        const ds_math::Matrix4 &translation =
            m_transformComponentManager.GetWorldTransform(cameraTransform);
        const ds_math::Matrix4 &orientation =
            ds_math::Matrix4::CreateFromQuaternion(
                m_cameraComponentManager.GetOrientation(cameraComponent));
        // Get transform matrix of camera, invert it to find view matrix
        const ds_math::Matrix4 &viewMatrix =
            ds_math::Matrix4::Inverse(translation * orientation);

        // Get projection matrix of camera
        const ds_math::Matrix4 &projectionMatrix =
            m_cameraComponentManager.GetProjectionMatrix(cameraComponent);

        // Update scene constant buffer
        m_sceneBufferDescrip.InsertMemberData(
            "Scene.viewMatrix", sizeof(ds_math::Matrix4), &viewMatrix);
        m_sceneBufferDescrip.InsertMemberData("Scene.projectionMatrix",
                                              sizeof(ds_math::Matrix4),
                                              &projectionMatrix);
        m_renderer->UpdateConstantBufferData(m_sceneMatrices,
                                             m_sceneBufferDescrip);

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
            ds_render::Mesh mesh =
                m_renderComponentManager.GetMesh(renderInstance);
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
                ds_render::PrimitiveType::Triangles,
                mesh.GetStartingIndex(), mesh.GetNumIndices());

            // For each texture in material, unbind
            for (auto samplerTexture : material.GetTextures())
            {
                m_renderer->UnbindTextureFromSampler(
                    samplerTexture.second.GetTextureHandle());
            }
        }
    }
}
}
