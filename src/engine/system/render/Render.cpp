#include <fstream>
#include <sstream>

#include "engine/message/MessageHelper.h"
#include "engine/resource/MaterialResource.h"
#include "engine/resource/ShaderResource.h"
#include "engine/resource/TerrainResource.h"
#include "engine/resource/TextureResource.h"
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

    m_timeInSeconds = 0.0f;

    m_cameraActive = false;

    m_hasSkybox = false;

    return result;
}

void Render::Update(float deltaTime)
{
    m_timeInSeconds += deltaTime;
    ProcessEvents(&m_messagesReceived);

    // Make sure renderer has been created
    if (m_renderer != nullptr)
    {
        m_renderer->ClearBuffers();

        RenderScene(deltaTime);
    }
}

void Render::Shutdown()
{
}

void Render::PostMessages(const ds_msg::MessageStream &messages)
{
    AppendStreamBuffer(&m_messagesReceived, messages);
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

void Render::SetSkyboxMaterial(const std::string &skyboxMaterial)
{
    std::stringstream skyboxMaterialFullPath;
    skyboxMaterialFullPath << "../assets/" << skyboxMaterial;
    // If this is the first time we are setting the skybox, make the mesh
    if (m_hasSkybox == false)
    {
        m_skybox.SetMesh(CreateSkyboxMesh());
    }

    ds_render::Mesh currentMesh = m_skybox.GetMesh();
    ds_render::SubMesh currentSubMesh = currentMesh.GetSubMesh(0);
    // Update material
    currentSubMesh.material = CreateMaterialFromMaterialResource(
        skyboxMaterialFullPath.str(), m_sceneMatrices, m_objectMatrices);
    // Update submesh
    currentMesh.SetSubMesh(0, currentSubMesh);
    // Update mesh
    m_skybox.SetMesh(currentMesh);

    m_hasSkybox = true;
}

ds_render::Mesh Render::CreatePanelMesh(float startX,
                                        float startY,
                                        float endX,
                                        float endY,
                                        const std::string &materialPath)
{
    ds_render::Mesh mesh = ds_render::Mesh();

    // Create vertex buffer data store
    ds_com::StreamBuffer vertexBufferStore;
    // Create vertex buffer descriptor
    ds_render::VertexBufferDescription vertexBufferDescriptor;

    // Add vertex positions
    vertexBufferStore << startX;
    vertexBufferStore << startY;
    vertexBufferStore << endX;
    vertexBufferStore << startY;
    vertexBufferStore << endX;
    vertexBufferStore << endY;
    vertexBufferStore << startX;
    vertexBufferStore << endY;

    // Describe position data
    ds_render::VertexBufferDescription::AttributeDescription
        positionAttributeDescriptor;
    positionAttributeDescriptor.attributeType =
        ds_render::AttributeType::Position;
    positionAttributeDescriptor.attributeDataType =
        ds_render::RenderDataType::Float;
    positionAttributeDescriptor.numElementsPerAttribute = 2;
    positionAttributeDescriptor.stride = 0;
    positionAttributeDescriptor.offset = 0;
    positionAttributeDescriptor.normalized = false;

    // Add position attribute descriptor to buffer descriptor
    vertexBufferDescriptor.AddAttributeDescription(positionAttributeDescriptor);

    // Add texcoord data
    vertexBufferStore << 0.0f;
    vertexBufferStore << 0.0f;
    vertexBufferStore << 1.0f;
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
    texCoordAttributeDescriptor.offset = 8 * sizeof(float);
    texCoordAttributeDescriptor.normalized = false;

    // Add texcoord attribute descriptor to buffer descriptor
    vertexBufferDescriptor.AddAttributeDescription(texCoordAttributeDescriptor);

    // Create vertex buffer
    ds_render::VertexBufferHandle vb = m_renderer->CreateVertexBuffer(
        ds_render::BufferUsageType::Static, vertexBufferDescriptor,
        vertexBufferStore.AvailableBytes(), vertexBufferStore.GetDataPtr());

    // Create index buffer data store
    ds_com::StreamBuffer indexBufferStore;

    // Add indices
    indexBufferStore << 0;
    indexBufferStore << 1;
    indexBufferStore << 3;
    indexBufferStore << 3;
    indexBufferStore << 1;
    indexBufferStore << 2;

    // Create index buffer
    ds_render::IndexBufferHandle ib = m_renderer->CreateIndexBuffer(
        ds_render::BufferUsageType::Static, indexBufferStore.AvailableBytes(),
        indexBufferStore.GetDataPtr());

    // Create mesh
    mesh = ds_render::Mesh(vb, ib, ds_render::MeshResourceHandle());

    std::stringstream materialFullPath;
    materialFullPath << "../assets/" << materialPath;

    // Add submesh
    mesh.AddSubMesh(ds_render::SubMesh(
        0, 6, CreateMaterialFromMaterialResource(
                  materialFullPath.str(), m_sceneMatrices, m_objectMatrices)));

    return mesh;
}

void Render::SetAnimationIndex(Entity entity, int animationIndex)
{
    Instance i = m_renderComponentManager.GetInstanceForEntity(entity);

    if (i.IsValid())
    {
        // Get mesh resource handle for mesh
        ds_render::MeshResourceHandle meshResourceHandle =
            m_renderComponentManager.GetMesh(i).GetMeshResourceHandle();

        // Get mesh resource for mesh
        MeshResource *meshResource =
            (MeshResource *)m_handleManager.Get(meshResourceHandle);

        // Set animation index
        meshResource->SetAnimationIndex(animationIndex);
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
                    // Allocate space for view and projection matrices
                    m_sceneBufferDescrip = ds_render::ConstantBufferDescription(
                        2 * sizeof(ds_math::Matrix4));
                    m_sceneBufferDescrip.AddMember("Scene.viewMatrix");
                    m_sceneBufferDescrip.AddMember("Scene.projectionMatrix");
                    m_renderer->GetConstantBufferDescription(
                        fakeShader, "Scene", &m_sceneBufferDescrip);

                    // Allocate space for model matrix and bone transforms
                    m_objectBufferDescrip =
                        ds_render::ConstantBufferDescription(
                            1 * sizeof(ds_math::Matrix4) +
                            MeshResource::MAX_BONES * sizeof(ds_math::Matrix4));
                    // Add model matrix description
                    m_objectBufferDescrip.AddMember("Object.modelMatrix");
                    // Add bone transform descriptions
                    m_objectBufferDescrip.AddMember("Object.boneTransforms");
                    // Ask renderer to construct constant buffer
                    m_renderer->GetConstantBufferDescription(
                        fakeShader, "Object", &m_objectBufferDescrip);

                    // Create shader data
                    // Insert default data for view and projection matrix
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

                    // Insert default data for model matrix and bone transforms
                    ds_math::Matrix4 modelMatrix = ds_math::Matrix4(1.0f);
                    m_objectBufferDescrip.InsertMemberData(
                        "Object.modelMatrix", sizeof(ds_math::Matrix4),
                        &modelMatrix);

                    std::vector<ds_math::Matrix4> identityMatrices(
                        MeshResource::MAX_BONES, ds_math::Matrix4(1.0f));
                    m_objectBufferDescrip.InsertMemberData(
                        "Object.boneTransforms",
                        MeshResource::MAX_BONES * sizeof(ds_math::Matrix4),
                        &identityMatrices[0]);

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
                    if (componentData.GetString("mesh", &meshName))
                    {
                        // Get mesh resource path
                        std::stringstream meshResourcePath;
                        meshResourcePath << "../assets/" << meshName;

                        // Get material file paths
                        std::vector<std::string> materialKeys =
                            componentData.GetObjectKeys("materials");

                        // Create Mesh
                        ds_render::Mesh mesh =
                            CreateMeshFromMeshResource(meshResourcePath.str());

                        // Only continue if the number of submeshes matches the
                        // number of materials provided
                        if (mesh.GetNumSubMeshes() == materialKeys.size())
                        {
                            // For each material file path
                            for (unsigned int iMaterial = 0;
                                 iMaterial < materialKeys.size(); ++iMaterial)
                            {
                                // Load material resource
                                // Get material resource path
                                std::stringstream materialResourcePath;
                                materialResourcePath << "../assets/"
                                                     << materialKeys[iMaterial];

                                // Create material
                                ds_render::Material material =
                                    CreateMaterialFromMaterialResource(
                                        materialResourcePath.str(),
                                        m_sceneMatrices, m_objectMatrices);

                                // Each material maps to one submesh, get that
                                // submesh and set material
                                ds_render::SubMesh subMesh =
                                    mesh.GetSubMesh(iMaterial);
                                subMesh.material = material;

                                mesh.SetSubMesh(iMaterial, subMesh);
                            }

                            Instance i = m_renderComponentManager
                                             .CreateComponentForEntity(
                                                 createComponentMsg.entity);
                            m_renderComponentManager.SetMesh(i, mesh);
                        }
                        else
                        {
                            std::cerr
                                << "Number of materials specified in "
                                << meshResourcePath.str()
                                << " not equal to number of submeshes in mesh."
                                << std::endl;
                        }
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
                    float heightScale;

                    if (componentData.GetString("heightmap", &heightMapName) &&
                        componentData.GetString("material", &materialName) &&
                        componentData.GetFloat("heightScale", &heightScale))
                    {
                        std::stringstream heightMapPath;
                        heightMapPath << "../assets/" << heightMapName;

                        std::unique_ptr<TerrainResource> terrainResource =
                            m_factory.CreateResource<TerrainResource>(
                                heightMapPath.str());
                        terrainResource->SetHeightScale(heightScale);

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

                        ds_render::Mesh mesh = ds_render::Mesh(
                            vb, ib, ds_render::MeshResourceHandle());
                        mesh.AddSubMesh(
                            ds_render::SubMesh(0, indices.size(), material));

                        Instance i =
                            m_renderComponentManager.CreateComponentForEntity(
                                createComponentMsg.entity);

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
        case ds_msg::MessageType::SetAnimationIndex:
        {
            ds_msg::SetAnimationIndex setAnimationMsg;
            (*messages) >> setAnimationMsg;

            SetAnimationIndex(setAnimationMsg.entity,
                              setAnimationMsg.animationIndex);

            break;
        }
        case ds_msg::MessageType::SetSkyboxMaterial:
        {
            ds_msg::SetSkyboxMaterial setSkyboxMaterialMsg;
            (*messages) >> setSkyboxMaterialMsg;

            SetSkyboxMaterial(StringIntern::Instance().GetString(
                setSkyboxMaterialMsg.skyboxMaterialPath));
            break;
        }
        case ds_msg::MessageType::CreatePanel:
        {
            ds_msg::CreatePanel createPanelMsg;
            (*messages) >> createPanelMsg;

            ds_render::Mesh mesh =
                CreatePanelMesh(createPanelMsg.startX, createPanelMsg.startY,
                                createPanelMsg.endX, createPanelMsg.endY,
                                StringIntern::Instance().GetString(
                                    createPanelMsg.materialPath));
            Instance i = m_renderComponentManager.CreateComponentForEntity(
                createPanelMsg.entity);

            m_renderComponentManager.SetMesh(i, mesh);
            break;
        }
        case ds_msg::MessageType::CreateButton:
        {
            ds_msg::CreateButton createButtonMsg;
            (*messages) >> createButtonMsg;

            ds_render::Mesh mesh =
                CreatePanelMesh(createButtonMsg.startX, createButtonMsg.startY,
                                createButtonMsg.endX, createButtonMsg.endY,
                                StringIntern::Instance().GetString(
                                    createButtonMsg.defaultMaterialPath));

            // Create render component
            Instance render = m_renderComponentManager.CreateComponentForEntity(
                createButtonMsg.entity);
            m_renderComponentManager.SetMesh(render, mesh);

            std::stringstream defaultMaterialPathFull;
            defaultMaterialPathFull << "../assets/"
                                    << StringIntern::Instance().GetString(
                                           createButtonMsg.defaultMaterialPath);
            std::stringstream pressedMaterialPathFull;
            pressedMaterialPathFull << "../assets/"
                                    << StringIntern::Instance().GetString(
                                           createButtonMsg.pressedMaterialPath);
            std::stringstream hoverMaterialPathFull;
            hoverMaterialPathFull << "../assets/"
                                  << StringIntern::Instance().GetString(
                                         createButtonMsg.hoverMaterialPath);

            // Create button component
            Instance button = m_buttonComponentManager.CreateComponentForEntity(
                createButtonMsg.entity);
            m_buttonComponentManager.SetDefaultMaterial(
                button, CreateMaterialFromMaterialResource(
                            defaultMaterialPathFull.str(), m_sceneMatrices,
                            m_objectMatrices));
            m_buttonComponentManager.SetPressedMaterial(
                button, CreateMaterialFromMaterialResource(
                            pressedMaterialPathFull.str(), m_sceneMatrices,
                            m_objectMatrices));
            m_buttonComponentManager.SetHoverMaterial(
                button, CreateMaterialFromMaterialResource(
                            hoverMaterialPathFull.str(), m_sceneMatrices,
                            m_objectMatrices));
            m_buttonComponentManager.SetStartXCoordinate(
                button, createButtonMsg.startX);
            m_buttonComponentManager.SetStartYCoordinate(
                button, createButtonMsg.startY);
            m_buttonComponentManager.SetEndXCoordinate(button,
                                                       createButtonMsg.endX);
            m_buttonComponentManager.SetEndYCoordinate(button,
                                                       createButtonMsg.endY);

            break;
        }
        case ds_msg::MessageType::MouseMotion:
        {
            ds_msg::MouseMotion mouseMotionEvent;
            (*messages) >> mouseMotionEvent;

            // For each button component
            for (unsigned int i = 0;
                 i < m_buttonComponentManager.GetNumInstances(); ++i)
            {
                // Get button component instance
                Instance button = Instance::MakeInstance(i);

                // Get entity
                Entity entity =
                    m_buttonComponentManager.GetEntityForInstance(button);

                // Get render component instance
                Instance render =
                    m_renderComponentManager.GetInstanceForEntity(entity);

                if (render.IsValid())
                {
                    // Reset all materials to default material
                    ds_render::Mesh mesh =
                        m_renderComponentManager.GetMesh(render);
                    ds_render::SubMesh subMesh = mesh.GetSubMesh(0);
                    subMesh.material =
                        m_buttonComponentManager.GetDefaultMaterial(button);
                    mesh.SetSubMesh(0, subMesh);
                    m_renderComponentManager.SetMesh(render, mesh);

                    // Is mouse colliding with button
                    if (mouseMotionEvent.x >
                            m_buttonComponentManager.GetStartXCoordinate(
                                button) &&
                        mouseMotionEvent.y <
                            m_buttonComponentManager.GetStartYCoordinate(
                                button) &&
                        mouseMotionEvent.x <
                            m_buttonComponentManager.GetEndXCoordinate(
                                button) &&
                        mouseMotionEvent.y >
                            m_buttonComponentManager.GetEndYCoordinate(button))

                    {
                        // If user is holding down left mouse button
                        if (mouseMotionEvent.button.left == true)
                        {
                            // Set pressed material
                            ds_render::Mesh mesh =
                                m_renderComponentManager.GetMesh(render);
                            ds_render::SubMesh subMesh = mesh.GetSubMesh(0);
                            subMesh.material =
                                m_buttonComponentManager.GetPressedMaterial(
                                    button);
                            mesh.SetSubMesh(0, subMesh);
                            m_renderComponentManager.SetMesh(render, mesh);
                        }
                        // Else
                        else
                        {
                            // Set hover material
                            ds_render::Mesh mesh =
                                m_renderComponentManager.GetMesh(render);
                            ds_render::SubMesh subMesh = mesh.GetSubMesh(0);
                            subMesh.material =
                                m_buttonComponentManager.GetHoverMaterial(
                                    button);
                            mesh.SetSubMesh(0, subMesh);
                            m_renderComponentManager.SetMesh(render, mesh);
                        }
                    }
                }
            }
            break;
        }
        case ds_msg::MessageType::MouseButton:
        {
            ds_msg::MouseButton mouseButtonEvent;
            (*messages) >> mouseButtonEvent;

            // For each button component
            for (unsigned int i = 0;
                 i < m_buttonComponentManager.GetNumInstances(); ++i)
            {
                // Get button component instance
                Instance button = Instance::MakeInstance(i);

                // Get entity
                Entity entity =
                    m_buttonComponentManager.GetEntityForInstance(button);

                // Get render component instance
                Instance render =
                    m_renderComponentManager.GetInstanceForEntity(entity);

                if (render.IsValid())
                {
                    // Reset all materials to default material
                    // ds_render::Mesh mesh =
                    //     m_renderComponentManager.GetMesh(render);
                    // ds_render::SubMesh subMesh = mesh.GetSubMesh(0);
                    // subMesh.material =
                    //     m_buttonComponentManager.GetDefaultMaterial(button);
                    // mesh.SetSubMesh(0, subMesh);
                    // m_renderComponentManager.SetMesh(render, mesh);

                    // Is mouse colliding with button
                    if (mouseButtonEvent.x >
                            m_buttonComponentManager.GetStartXCoordinate(
                                button) &&
                        mouseButtonEvent.y <
                            m_buttonComponentManager.GetStartYCoordinate(
                                button) &&
                        mouseButtonEvent.x <
                            m_buttonComponentManager.GetEndXCoordinate(
                                button) &&
                        mouseButtonEvent.y >
                            m_buttonComponentManager.GetEndYCoordinate(button))

                    {
                        // If user has pressed left mouse button
                        if (mouseButtonEvent.button.left == true)
                        {
                            // Set pressed material
                            ds_render::Mesh mesh =
                                m_renderComponentManager.GetMesh(render);
                            ds_render::SubMesh subMesh = mesh.GetSubMesh(0);
                            subMesh.material =
                                m_buttonComponentManager.GetPressedMaterial(
                                    button);
                            mesh.SetSubMesh(0, subMesh);
                            m_renderComponentManager.SetMesh(render, mesh);

                            // Send a button fired message with the id of the
                            // pressed button
                            ds_msg::ButtonFired buttonFiredMsg;
                            buttonFiredMsg.entity = entity;

                            ds_msg::AppendMessage(
                                &m_messagesGenerated,
                                ds_msg::MessageType::ButtonFired,
                                sizeof(ds_msg::ButtonFired), &buttonFiredMsg);
                        }
                        // If user has let go of left mouse button
                        if (mouseButtonEvent.button.left == false)
                        {
                            // Set hover material
                            ds_render::Mesh mesh =
                                m_renderComponentManager.GetMesh(render);
                            ds_render::SubMesh subMesh = mesh.GetSubMesh(0);
                            subMesh.material =
                                m_buttonComponentManager.GetHoverMaterial(
                                    button);
                            mesh.SetSubMesh(0, subMesh);
                            m_renderComponentManager.SetMesh(render, mesh);
                        }
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

ds_render::Texture Render::CreateTextureFromTextureResource(
    const ds_render::SamplerType &samplerType,
    const std::vector<std::string> &filePaths)
{
    ds_render::Texture texture;

    switch (samplerType)
    {
    case ds_render::SamplerType::TwoDimensional:
    {
        // Texture from texture resource
        std::unique_ptr<TextureResource> textureResource =
            m_factory.CreateResource<TextureResource>(filePaths[0]);

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
            assert(false && "Unsupported image component flag.");
            format = ds_render::ImageFormat::RGBA;
            break;
        }

        // Create texture
        texture = ds_render::Texture(
            ds_render::SamplerType::TwoDimensional,
            m_renderer->Create2DTexture(
                format, ds_render::RenderDataType::UnsignedByte,
                ds_render::InternalImageFormat::RGBA8, true,
                textureResource->GetWidthInPixels(),
                textureResource->GetHeightInPixels(),
                textureResource->GetTextureContents()));

        break;
    }
    case ds_render::SamplerType::Cubemap:
    {
        assert(filePaths.size() == 6 &&
               "Incorrect number of cubemap images provided.");
        // Get texture resources
        std::vector<std::unique_ptr<TextureResource>> textureResources;
        for (auto filePath : filePaths)
        {
            textureResources.push_back(
                m_factory.CreateResource<TextureResource>(filePath));
        }

        // Use format of first image for all images
        ds_render::ImageFormat format;
        switch (textureResources[0]->GetComponentFlag())
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
            assert(false && "Unsupported image component flag.");
            format = ds_render::ImageFormat::RGBA;
            break;
        }

        // Create texture
        texture = ds_render::Texture(
            ds_render::SamplerType::Cubemap,
            m_renderer->CreateCubemapTexture(
                format, ds_render::RenderDataType::UnsignedByte,
                ds_render::InternalImageFormat::RGBA8,
                textureResources[0]->GetWidthInPixels(), // Use width of first
                // resource for all resources
                textureResources[0]->GetHeightInPixels(), // Use height of first
                // resource for all resources
                textureResources[0]->GetTextureContents(),
                textureResources[1]->GetTextureContents(),
                textureResources[2]->GetTextureContents(),
                textureResources[3]->GetTextureContents(),
                textureResources[4]->GetTextureContents(),
                textureResources[5]->GetTextureContents()));

        break;
    }
    default:
    {
        assert(false && "Unhandled sampler type for texture.");
        break;
    }
    }

    return texture;
}

ds_render::Mesh Render::CreateSkyboxMesh()
{
    ds_render::Mesh mesh = ds_render::Mesh();

    // Create vertex buffer data store
    ds_com::StreamBuffer vertexBufferStore;
    // Create vertex buffer descriptor
    ds_render::VertexBufferDescription vertexBufferDescriptor;

    // Add vertex positions
    vertexBufferStore << -1.0f;
    vertexBufferStore << -1.0f;
    vertexBufferStore << 1.0f;
    vertexBufferStore << -1.0f;
    vertexBufferStore << 1.0f;
    vertexBufferStore << 1.0f;
    vertexBufferStore << 1.0f;
    vertexBufferStore << 1.0f;
    vertexBufferStore << -1.0f;
    vertexBufferStore << 1.0f;
    vertexBufferStore << -1.0f;
    vertexBufferStore << -1.0f;

    // Describe position data
    ds_render::VertexBufferDescription::AttributeDescription
        positionAttributeDescriptor;
    positionAttributeDescriptor.attributeType =
        ds_render::AttributeType::Position;
    positionAttributeDescriptor.attributeDataType =
        ds_render::RenderDataType::Float;
    positionAttributeDescriptor.numElementsPerAttribute = 2;
    positionAttributeDescriptor.stride = 0;
    positionAttributeDescriptor.offset = 0;
    positionAttributeDescriptor.normalized = false;

    // Add position attribute descriptor to buffer descriptor
    vertexBufferDescriptor.AddAttributeDescription(positionAttributeDescriptor);

    // Create vertex buffer
    ds_render::VertexBufferHandle vb = m_renderer->CreateVertexBuffer(
        ds_render::BufferUsageType::Static, vertexBufferDescriptor,
        vertexBufferStore.AvailableBytes(), vertexBufferStore.GetDataPtr());

    // Create mesh
    mesh = ds_render::Mesh(vb, ds_render::IndexBufferHandle(),
                           ds_render::MeshResourceHandle());

    // Add submesh
    mesh.AddSubMesh(ds_render::SubMesh(0, 6, ds_render::Material()));

    return mesh;
}

ds_render::Mesh Render::CreateMeshFromMeshResource(const std::string &filePath)
{
    ds_render::Mesh mesh = ds_render::Mesh();

    // Store mesh resource
    ds_render::MeshResourceHandle meshResourceHandle =
        (ds_render::MeshResourceHandle)m_handleManager.Add(
            (void *)m_factory.CreateResource<MeshResource>(filePath).release(),
            0);

    // Get mesh resource data
    MeshResource *meshResource =
        (MeshResource *)m_handleManager.Get(meshResourceHandle);

    if (meshResource != nullptr)
    {
        // Create vertex buffer data store
        ds_com::StreamBuffer vertexBufferStore;
        // Create vertex buffer descriptor
        ds_render::VertexBufferDescription vertexBufferDescriptor;

        const std::vector<ds_math::Vector3> positions =
            meshResource->GetVerts();
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

        // Add position attribute descriptor to buffer descriptor
        vertexBufferDescriptor.AddAttributeDescription(
            positionAttributeDescriptor);

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

        // Add texcoord attribute descriptor to buffer descriptor
        vertexBufferDescriptor.AddAttributeDescription(
            texCoordAttributeDescriptor);

        // If we have bone data
        const std::vector<MeshResource::VertexBoneData> boneData =
            meshResource->GetVertexBoneData();
        if (boneData.size() > 0)
        {
            // Get bone data
            for (const MeshResource::VertexBoneData &boneDatum : boneData)
            {
                // Interlace ids and weights
                vertexBufferStore << boneDatum;
            }
            // Describe bone data
            // Describe bone IDs
            ds_render::VertexBufferDescription::AttributeDescription
                boneIDAttributeDescriptor;
            boneIDAttributeDescriptor.attributeType =
                ds_render::AttributeType::BoneID;
            boneIDAttributeDescriptor.attributeDataType =
                ds_render::RenderDataType::Int;
            boneIDAttributeDescriptor.numElementsPerAttribute = 4;
            boneIDAttributeDescriptor.stride =
                sizeof(MeshResource::VertexBoneData);
            // Jump over position and texcoord data
            boneIDAttributeDescriptor.offset =
                meshResource->GetVertCount() *
                    (sizeof(ds_math::Vector3) + 2 * sizeof(float)) +
                0;
            boneIDAttributeDescriptor.normalized = false;

            // Add bone ID attribute descriptor to buffer descriptor
            vertexBufferDescriptor.AddAttributeDescription(
                boneIDAttributeDescriptor);

            // Describe bone weights
            ds_render::VertexBufferDescription::AttributeDescription
                boneWeightAttributeDescriptor;
            boneWeightAttributeDescriptor.attributeType =
                ds_render::AttributeType::BoneWeight;
            boneWeightAttributeDescriptor.attributeDataType =
                ds_render::RenderDataType::Float;
            boneWeightAttributeDescriptor.numElementsPerAttribute = 4;
            boneWeightAttributeDescriptor.stride =
                sizeof(MeshResource::VertexBoneData);
            // Jump over position, texcoord and bone id data
            boneWeightAttributeDescriptor.offset =
                meshResource->GetVertCount() *
                    (sizeof(ds_math::Vector3) + 2 * sizeof(float)) +
                (sizeof(unsigned int) * 4);
            boneWeightAttributeDescriptor.normalized = false;

            // Add bone weight attribute descriptor to buffer descriptor
            vertexBufferDescriptor.AddAttributeDescription(
                boneWeightAttributeDescriptor);
        }

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

        mesh = ds_render::Mesh(vb, ib, meshResourceHandle);
        // Create Mesh

        // For 0 to meshCount - 1, add submesh..
        for (unsigned int iSubMesh = 0; iSubMesh < meshResource->GetMeshCount();
             ++iSubMesh)
        {
            mesh.AddSubMesh(ds_render::SubMesh(
                meshResource->GetBaseIndex(iSubMesh),
                meshResource->GetNumIndices(iSubMesh), ds_render::Material()));
        }
    }
    else
    {
        std::cerr << filePath << " failed to load." << std::endl;
    }

    return mesh;
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
        // const std::string &textureResourceFilePath =
        //     materialResource->GetTextureResourceFilePath(samplerName);
        std::vector<std::string> textureResourceFilePaths =
            materialResource->GetTextureSamplerFilePaths(samplerName);

        material.AddTexture(
            samplerName,
            CreateTextureFromTextureResource(
                materialResource->GetTextureSamplerType(samplerName),
                textureResourceFilePaths));
    }

    // Bind constant buffers to program
    m_renderer->BindConstantBuffer(material.GetProgram(), "Scene",
                                   sceneMatrices);
    m_renderer->BindConstantBuffer(material.GetProgram(), "Object",
                                   objectMatrices);

    return material;
}

void Render::RenderScene(float deltaTime)
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

        if (m_hasSkybox)
        {
            // Render skybox
            // Disable writing to the depth buffer so that the depth information
            // of the skybox mesh isn't written to the depth buffer and hence
            // everything is rendered on top of the skybox (given that the clear
            // depth is 1.0f (far away as possible))
            m_renderer->SetDepthWriting(false);
            for (unsigned int iSubMesh = 0;
                 iSubMesh < m_skybox.GetMesh().GetNumSubMeshes(); ++iSubMesh)
            {
                // Get material
                ds_render::Material material =
                    m_skybox.GetMesh().GetSubMesh(iSubMesh).material;

                // Set shader program
                m_renderer->SetProgram(material.GetProgram());

                // For each texture in material, bind it to shader
                for (auto samplerTexture : material.GetTextures())
                {
                    m_renderer->BindTextureToSampler(
                        material.GetProgram(), samplerTexture.first,
                        samplerTexture.second.GetSamplerType(),
                        samplerTexture.second.GetTextureHandle());
                }

                // Draw the mesh
                m_renderer->DrawVertices(
                    m_skybox.GetMesh().GetVertexBuffer(),
                    ds_render::PrimitiveType::Triangles,
                    m_skybox.GetMesh().GetSubMesh(iSubMesh).startingIndex,
                    m_skybox.GetMesh().GetSubMesh(iSubMesh).numIndices);

                // For each texture in material, unbind
                for (auto samplerTexture : material.GetTextures())
                {
                    m_renderer->UnbindTextureFromSampler(
                        samplerTexture.second.GetSamplerType(),
                        samplerTexture.second.GetTextureHandle());
                }
            }
            // Re-enable depth writing
            m_renderer->SetDepthWriting(true);
        }

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

            // Get mesh
            ds_render::Mesh mesh =
                m_renderComponentManager.GetMesh(renderInstance);

            // If has transform instance
            if (transformInstance.IsValid())
            {
                // Update object constant buffer with world transform of this
                // transform instance
                m_objectBufferDescrip.InsertMemberData(
                    "Object.modelMatrix", sizeof(ds_math::Matrix4),
                    &m_transformComponentManager.GetWorldTransform(
                        transformInstance));

                // First, get mesh resource that holds skeleton/animation data
                MeshResource *meshResource =
                    (MeshResource *)m_handleManager.Get(
                        mesh.GetMeshResourceHandle());
                // Get bone transform data to bind to shader
                std::vector<ds_math::Matrix4> boneTransforms(
                    MeshResource::MAX_BONES, ds_math::Matrix4());

                // If we have mesh resource (terrain components don't)
                if (meshResource != nullptr)
                {
                    // Then query mesh resource for data
                    meshResource->BoneTransform(deltaTime, &boneTransforms);
                }
                m_objectBufferDescrip.InsertMemberData(
                    "Object.boneTransforms",
                    MeshResource::MAX_BONES * sizeof(ds_math::Matrix4),
                    &boneTransforms[0]);
                m_renderer->UpdateConstantBufferData(m_objectMatrices,
                                                     m_objectBufferDescrip);
            }

            for (unsigned int iSubMesh = 0; iSubMesh < mesh.GetNumSubMeshes();
                 ++iSubMesh)
            {
                // Get material
                ds_render::Material material =
                    mesh.GetSubMesh(iSubMesh).material;

                // Set shader program
                m_renderer->SetProgram(material.GetProgram());

                // For each texture in material, bind it to shader
                for (auto samplerTexture : material.GetTextures())
                {
                    m_renderer->BindTextureToSampler(
                        material.GetProgram(), samplerTexture.first,
                        samplerTexture.second.GetSamplerType(),
                        samplerTexture.second.GetTextureHandle());
                }

                // Draw the mesh
                m_renderer->DrawVerticesIndexed(
                    mesh.GetVertexBuffer(), mesh.GetIndexBuffer(),
                    ds_render::PrimitiveType::Triangles,
                    mesh.GetSubMesh(iSubMesh).startingIndex,
                    mesh.GetSubMesh(iSubMesh).numIndices);

                // For each texture in material, unbind
                for (auto samplerTexture : material.GetTextures())
                {
                    m_renderer->UnbindTextureFromSampler(
                        samplerTexture.second.GetSamplerType(),
                        samplerTexture.second.GetTextureHandle());
                }
            }
        }
    }
}
}
