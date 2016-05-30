#include <fstream>
#include <sstream>

#include "engine/common/HandleCommon.h"
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
// Static member definitions
std::unique_ptr<ds_render::IRenderer> Render::m_renderer;
TextureResourceManager Render::m_textureResourceManager;
Render::TextureManager Render::m_textureManager;
MaterialResourceManager Render::m_materialResourceManager;
Render::MaterialManager Render::m_materialManager;
ResourceFactory Render::m_factory;
ds_render::ConstantBufferHandle Render::m_sceneMatrices;
ds_render::ConstantBufferHandle Render::m_objectMatrices;

ds_render::Texture *
Render::TextureManager::GetTexture(ds_render::TextureHandle textureHandle)
{
    ds_render::Texture *texture = nullptr;

    // Get managed texture resource
    ManagedTexture *managedTexture = nullptr;
    bool result = m_handleManager.Get(textureHandle, (void **)&managedTexture);

    // If successful
    if (result == true)
    {
        // Get texture resource from managed texture
        texture = &(managedTexture->texture);
    }

    return texture;
}

const ds_render::Texture *
Render::TextureManager::GetTexture(ds_render::TextureHandle textureHandle) const
{
    const ds_render::Texture *texture = nullptr;

    // Get managed texture resource
    ManagedTexture *managedTexture = nullptr;
    bool result = m_handleManager.Get(textureHandle, (void **)&managedTexture);

    // If successful
    if (result == true)
    {
        // Get texture resource from managed texture
        texture = &(managedTexture->texture);
    }

    return texture;
}

bool Render::TextureManager::GetTextureForResourceHandle(
    TextureResourceHandle textureResourceHandle,
    ds_render::TextureHandle *textureHandle)
{
    bool result = false;

    // Attempt to find texture for the given resource handle
    std::vector<ManagedTexture>::const_iterator it =
        std::find_if(m_textures.begin(), m_textures.end(),
                     [&](const ManagedTexture &managedTexture)
                     {
                         return managedTexture.texture.textureResourceHandle ==
                                textureResourceHandle;
                     });

    // If found, return it
    if (it != m_textures.end())
    {
        *textureHandle = it->handle;

        result = true;
    }
    // If not found, create texture for texture resource
    else
    {
        // Create texture from texture resource handle
        ds_render::Texture texture =
            CreateTextureFromTextureResource(textureResourceHandle);

        ManagedTexture managedTexture;
        managedTexture.texture = texture;

        // Add texture to list
        m_textures.push_back(managedTexture);
        // Location in vector of texture we just added
        size_t loc = m_textures.size() - 1;

        // Add texture to handle manager
        Handle handle = m_handleManager.Add(
            (void *)&m_textures[loc], (uint32_t)HandleTypes::TextureHandleType);
        // Store handle with texture resource
        m_textures[loc].handle = handle;

        // Because pushing adding an element to the vector might cause the
        // vector to realloc, update the address of all managed texture
        // resource objects
        for (unsigned int i = 0; i < m_textures.size(); ++i)
        {
            // Get handle
            ds::Handle handle = m_textures[i].handle;
            // Update with new memory address
            m_handleManager.Update(handle, &m_textures[i]);
        }

        // If successful, return texture resource handle to caller
        *textureHandle = handle;

        result = true;
    }

    return result;
}

ds_render::Material *
Render::MaterialManager::GetMaterial(ds_render::MaterialHandle materialHandle)
{
    ds_render::Material *material = nullptr;

    // Get managed material resource
    ManagedMaterial *managedMaterial = nullptr;
    bool result =
        m_handleManager.Get(materialHandle, (void **)&managedMaterial);

    // If successful
    if (result == true)
    {
        // Get material resource from managed material
        material = &(managedMaterial->material);
    }

    return material;
}

const ds_render::Material *Render::MaterialManager::GetMaterial(
    ds_render::MaterialHandle materialHandle) const
{
    const ds_render::Material *material = nullptr;

    // Get managed material resource
    ManagedMaterial *managedMaterial = nullptr;
    bool result =
        m_handleManager.Get(materialHandle, (void **)&managedMaterial);

    // If successful
    if (result == true)
    {
        // Get material resource from managed material
        material = &(managedMaterial->material);
    }

    return material;
}

bool Render::MaterialManager::GetMaterialForResourceHandle(
    MaterialResourceHandle materialResourceHandle,
    ds_render::MaterialHandle *materialHandle)
{
    bool result = false;

    // Attempt to find material for the given resource handle
    std::vector<ManagedMaterial>::const_iterator it = std::find_if(
        m_materials.begin(), m_materials.end(),
        [&](const ManagedMaterial &managedMaterial)
        {
            return managedMaterial.material.GetMaterialResourceHandle() ==
                   materialResourceHandle;
        });

    // If found, return it
    if (it != m_materials.end())
    {
        *materialHandle = it->handle;

        result = true;
    }
    // If not found, create material for material resource
    else
    {
        // Create material from material resource handle
        ds_render::Material material =
            CreateMaterialFromMaterialResource(materialResourceHandle);

        ManagedMaterial managedMaterial;
        managedMaterial.material = material;

        // Add material to list
        m_materials.push_back(managedMaterial);
        // Location in vector of material we just added
        size_t loc = m_materials.size() - 1;

        // Add material to handle manager
        Handle handle =
            m_handleManager.Add((void *)&m_materials[loc],
                                (uint32_t)HandleTypes::MaterialHandleType);
        // Store handle with material resource
        m_materials[loc].handle = handle;

        // Because pushing adding an element to the vector might cause the
        // vector to realloc, update the address of all managed material
        // resource objects
        for (unsigned int i = 0; i < m_materials.size(); ++i)
        {
            // Get handle
            ds::Handle handle = m_materials[i].handle;
            // Update with new memory address
            m_handleManager.Update(handle, &m_materials[i]);
        }

        // If successful, return material resource handle to caller
        *materialHandle = handle;

        result = true;
    }

    return result;
}

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
        case ds_msg::MessageType::SetLocalTranslation:
        {
            ds_msg::SetLocalTranslation setTranslationMsg;
            (*messages) >> setTranslationMsg;

            // Get component instance of entity to move
            Instance transform =
                m_transformComponentManager.GetInstanceForEntity(
                    setTranslationMsg.entity);

            // If has transform component
            if (transform.IsValid())
            {
                // Set translation of entity
                m_transformComponentManager.SetLocalTranslation(
                    transform, setTranslationMsg.localTranslation);
            }

            break;
        }
        case ds_msg::MessageType::SetLocalOrientation:
        {
            ds_msg::SetLocalOrientation setOrientationMsg;
            (*messages) >> setOrientationMsg;

            // Get component instance of entity to rotate
            Instance transform =
                m_transformComponentManager.GetInstanceForEntity(
                    setOrientationMsg.entity);

            // If has transform component
            if (transform.IsValid())
            {
                // Set orientation of entity
                m_transformComponentManager.SetLocalOrientation(
                    transform, setOrientationMsg.localOrientation);
            }

            break;
        }
        case ds_msg::MessageType::SetLocalScale:
        {
            ds_msg::SetLocalScale setScaleMsg;
            (*messages) >> setScaleMsg;

            // Get component instance of entity to scale
            Instance transform =
                m_transformComponentManager.GetInstanceForEntity(
                    setScaleMsg.entity);

            // If has transform component
            if (transform.IsValid())
            {
                // Set scale of entity
                m_transformComponentManager.SetLocalScale(
                    transform, setScaleMsg.localScale);
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
    const std::string &textureResourceFilePath)
{
    ds_render::Texture texture;

    // Create texture resource and get handle to it
    TextureResourceHandle handle;
    m_textureResourceManager.LoadTextureResourceFromFile(
        textureResourceFilePath, &handle);
    // Get the texture resource
    const TextureResource *textureResource =
        m_textureResourceManager.GetTextureResource(handle);

    // Use it to make the appropriate texture
    switch (textureResource->GetTextureType())
    {
    case ds_render::TextureType::TwoDimensional:
    {
        assert(
            textureResource->GetNumImages() == 1 &&
            "Incorrect number of images for two-dimensional texture - need 1.");

        ds_render::ImageFormat format;
        switch (textureResource->GetComponentFlag(0))
        {
        case TextureResource::ComponentFlag::COMPONENT_FLAG_GREY:
            format = ds_render::ImageFormat::R;
            break;
        case TextureResource::ComponentFlag::COMPONENT_FLAG_GREYALPHA:
            format = ds_render::ImageFormat::RG;
            break;
        case TextureResource::ComponentFlag::COMPONENT_FLAG_RGB:
            format = ds_render::ImageFormat::RGB;
            break;
        case TextureResource::ComponentFlag::COMPONENT_FLAG_RGBA:
            format = ds_render::ImageFormat::RGBA;
            break;
        default:
            assert(false && "Unsupported image component flag.");
            format = ds_render::ImageFormat::RGBA;
            break;
        }

        // Create texture
        texture = ds_render::Texture(
            handle, ds_render::TextureType::TwoDimensional,
            m_renderer->Create2DTexture(
                format, ds_render::RenderDataType::UnsignedByte,
                ds_render::InternalImageFormat::RGBA8, true,
                textureResource->GetWidthInPixels(0),
                textureResource->GetHeightInPixels(0),
                textureResource->GetImageData(0)));

        break;
    }
    case ds_render::TextureType::Cubemap:
    {
        assert(textureResource->GetNumImages() == 6 &&
               "Incorrect number of cubemap images provided - need 6.");

        // Use format of first image for all images
        ds_render::ImageFormat format;
        switch (textureResource->GetComponentFlag(0))
        {
        case TextureResource::ComponentFlag::COMPONENT_FLAG_GREY:
            format = ds_render::ImageFormat::R;
            break;
        case TextureResource::ComponentFlag::COMPONENT_FLAG_GREYALPHA:
            format = ds_render::ImageFormat::RG;
            break;
        case TextureResource::ComponentFlag::COMPONENT_FLAG_RGB:
            format = ds_render::ImageFormat::RGB;
            break;
        case TextureResource::ComponentFlag::COMPONENT_FLAG_RGBA:
            format = ds_render::ImageFormat::RGBA;
            break;
        default:
            assert(false && "Unsupported image component flag.");
            format = ds_render::ImageFormat::RGBA;
            break;
        }

        // Create texture
        texture = ds_render::Texture(
            handle, ds_render::TextureType::Cubemap,
            m_renderer->CreateCubemapTexture(
                format, ds_render::RenderDataType::UnsignedByte,
                ds_render::InternalImageFormat::RGBA8,
                textureResource->GetWidthInPixels(0), // Use width of first
                // image for all resources
                textureResource->GetHeightInPixels(0), // Use height of first
                // image for all resources
                textureResource->GetImageData(0),
                textureResource->GetImageData(1),
                textureResource->GetImageData(2),
                textureResource->GetImageData(3),
                textureResource->GetImageData(4),
                textureResource->GetImageData(5)));
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

ds_render::Texture
Render::CreateTextureFromTextureResource(TextureResourceHandle handle)
{
    ds_render::Texture texture;

    // Get the texture resource
    const TextureResource *textureResource =
        m_textureResourceManager.GetTextureResource(handle);

    if (textureResource != nullptr)
    {
        // Use it to make the appropriate texture
        switch (textureResource->GetTextureType())
        {
        case ds_render::TextureType::TwoDimensional:
        {
            assert(textureResource->GetNumImages() == 1 &&
                   "Incorrect number of images for two-dimensional texture - "
                   "need 1.");

            ds_render::ImageFormat format;
            switch (textureResource->GetComponentFlag(0))
            {
            case TextureResource::ComponentFlag::COMPONENT_FLAG_GREY:
                format = ds_render::ImageFormat::R;
                break;
            case TextureResource::ComponentFlag::COMPONENT_FLAG_GREYALPHA:
                format = ds_render::ImageFormat::RG;
                break;
            case TextureResource::ComponentFlag::COMPONENT_FLAG_RGB:
                format = ds_render::ImageFormat::RGB;
                break;
            case TextureResource::ComponentFlag::COMPONENT_FLAG_RGBA:
                format = ds_render::ImageFormat::RGBA;
                break;
            default:
                assert(false && "Unsupported image component flag.");
                format = ds_render::ImageFormat::RGBA;
                break;
            }

            // Create texture
            texture = ds_render::Texture(
                handle, ds_render::TextureType::TwoDimensional,
                m_renderer->Create2DTexture(
                    format, ds_render::RenderDataType::UnsignedByte,
                    ds_render::InternalImageFormat::RGBA8, true,
                    textureResource->GetWidthInPixels(0),
                    textureResource->GetHeightInPixels(0),
                    textureResource->GetImageData(0)));

            break;
        }
        case ds_render::TextureType::Cubemap:
        {
            assert(textureResource->GetNumImages() == 6 &&
                   "Incorrect number of cubemap images provided - need 6.");

            // Use format of first image for all images
            ds_render::ImageFormat format;
            switch (textureResource->GetComponentFlag(0))
            {
            case TextureResource::ComponentFlag::COMPONENT_FLAG_GREY:
                format = ds_render::ImageFormat::R;
                break;
            case TextureResource::ComponentFlag::COMPONENT_FLAG_GREYALPHA:
                format = ds_render::ImageFormat::RG;
                break;
            case TextureResource::ComponentFlag::COMPONENT_FLAG_RGB:
                format = ds_render::ImageFormat::RGB;
                break;
            case TextureResource::ComponentFlag::COMPONENT_FLAG_RGBA:
                format = ds_render::ImageFormat::RGBA;
                break;
            default:
                assert(false && "Unsupported image component flag.");
                format = ds_render::ImageFormat::RGBA;
                break;
            }

            // Create texture
            texture = ds_render::Texture(
                handle, ds_render::TextureType::Cubemap,
                m_renderer->CreateCubemapTexture(
                    format, ds_render::RenderDataType::UnsignedByte,
                    ds_render::InternalImageFormat::RGBA8,
                    textureResource->GetWidthInPixels(0), // Use width of first
                    // image for all resources
                    textureResource->GetHeightInPixels(
                        0), // Use height of first
                    // image for all resources
                    textureResource->GetImageData(0),
                    textureResource->GetImageData(1),
                    textureResource->GetImageData(2),
                    textureResource->GetImageData(3),
                    textureResource->GetImageData(4),
                    textureResource->GetImageData(5)));
            break;
        }
        default:
        {
            assert(false && "Unhandled sampler type for texture.");
            break;
        }
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
        std::string textureResourceFilePath =
            materialResource->GetSamplerTextureResourceFilePath(samplerName);

        TextureResourceHandle textureResourceHandle;
        m_textureResourceManager.LoadTextureResourceFromFile(
            textureResourceFilePath, &textureResourceHandle);

        ds_render::TextureHandle textureHandle;
        m_textureManager.GetTextureForResourceHandle(textureResourceHandle,
                                                     &textureHandle);

        // Change this to take texture handle
        material.AddTexture(samplerName, textureHandle);
    }

    // Bind constant buffers to program
    m_renderer->BindConstantBuffer(material.GetProgram(), "Scene",
                                   sceneMatrices);
    m_renderer->BindConstantBuffer(material.GetProgram(), "Object",
                                   objectMatrices);

    return material;
}

ds_render::Material Render::CreateMaterialFromMaterialResource(
    MaterialResourceHandle handle)
{
    ds_render::Material material;

    // Get the material resource
    const MaterialResource *materialResource =
        m_materialResourceManager.GetMaterialResource(handle);

    if (materialResource != nullptr)
    {
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
        ds_render::ProgramHandle shaderProgram =
            m_renderer->CreateProgram(shaders);

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
            std::string textureResourceFilePath =
                materialResource->GetSamplerTextureResourceFilePath(
                    samplerName);

            TextureResourceHandle textureResourceHandle;
            m_textureResourceManager.LoadTextureResourceFromFile(
                textureResourceFilePath, &textureResourceHandle);

            ds_render::TextureHandle textureHandle;
            m_textureManager.GetTextureForResourceHandle(textureResourceHandle,
                                                         &textureHandle);

            // Change this to take texture handle
            material.AddTexture(samplerName, textureHandle);
        }

        // Bind constant buffers to program
        m_renderer->BindConstantBuffer(material.GetProgram(), "Scene",
                                       m_sceneMatrices);
        m_renderer->BindConstantBuffer(material.GetProgram(), "Object",
                                       m_objectMatrices);
    }

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

        const ds_math::Matrix4 &worldTransform =
            m_transformComponentManager.GetWorldTransform(cameraTransform);
        const ds_math::Matrix4 &viewMatrix =
            ds_math::Matrix4::Inverse(worldTransform);

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
                for (auto shaderTexture : material.GetTextures())
                {
                    ds_render::Texture *texture = m_textureManager.GetTexture(
                        shaderTexture.textureHandle);

                    if (texture != nullptr)
                    {
                        m_renderer->BindTextureToSampler(
                            material.GetProgram(), shaderTexture.samplerName,
                            texture->textureType, texture->renderTextureHandle);
                    }
                }

                // Draw the mesh
                m_renderer->DrawVertices(
                    m_skybox.GetMesh().GetVertexBuffer(),
                    ds_render::PrimitiveType::Triangles,
                    m_skybox.GetMesh().GetSubMesh(iSubMesh).startingIndex,
                    m_skybox.GetMesh().GetSubMesh(iSubMesh).numIndices);

                // For each texture in material, unbind
                for (auto shaderTexture : material.GetTextures())
                {
                    ds_render::Texture *texture = m_textureManager.GetTexture(
                        shaderTexture.textureHandle);

                    if (texture != nullptr)
                    {
                        m_renderer->UnbindTextureFromSampler(
                            texture->textureType, texture->renderTextureHandle);
                    }
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
                ds_math::Matrix4 worldTransform =
                    m_transformComponentManager.GetWorldTransform(
                        transformInstance);
                // Update object constant buffer with world transform of this
                // transform instance
                m_objectBufferDescrip.InsertMemberData("Object.modelMatrix",
                                                       sizeof(ds_math::Matrix4),
                                                       &worldTransform[0][0]);

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
                for (auto shaderTexture : material.GetTextures())
                {
                    ds_render::Texture *texture = m_textureManager.GetTexture(
                        shaderTexture.textureHandle);

                    if (texture != nullptr)
                    {
                        m_renderer->BindTextureToSampler(
                            material.GetProgram(), shaderTexture.samplerName,
                            texture->textureType, texture->renderTextureHandle);
                    }
                }

                // Draw the mesh
                m_renderer->DrawVerticesIndexed(
                    mesh.GetVertexBuffer(), mesh.GetIndexBuffer(),
                    ds_render::PrimitiveType::Triangles,
                    mesh.GetSubMesh(iSubMesh).startingIndex,
                    mesh.GetSubMesh(iSubMesh).numIndices);

                // For each texture in material, unbind
                for (auto shaderTexture : material.GetTextures())
                {
                    ds_render::Texture *texture = m_textureManager.GetTexture(
                        shaderTexture.textureHandle);

                    if (texture != nullptr)
                    {
                        m_renderer->UnbindTextureFromSampler(
                            texture->textureType, texture->renderTextureHandle);
                    }
                }
            }
        }
    }
}
}
