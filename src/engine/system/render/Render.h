#pragma once

#include <string>

#include "engine/common/HandleManager.h"
#include "engine/resource/MeshResource.h"
#include "engine/resource/ResourceFactory.h"
#include "engine/resource/MaterialResourceManager.h"
#include "engine/resource/TextureResourceManager.h"
#include "engine/system/ISystem.h"
#include "engine/system/render/ButtonComponentManager.h"
#include "engine/system/render/CameraComponentManager.h"
#include "engine/system/render/IRenderer.h"
#include "engine/system/render/Material.h"
#include "engine/system/render/Mesh.h"
#include "engine/system/render/RenderComponentManager.h"
#include "engine/system/render/Skybox.h"
#include "engine/system/render/Texture.h"
#include "engine/system/scene/TransformComponentManager.h"

#include "math/Vector3.h"

namespace ds
{
/**
 * The render system is responsible for rendering the world, it contains all
 * render specific data, including the render component data for each entity.
 */
class Render : public ISystem
{
public:
    /**
     * The texture manager class manages access to and creation of texture
     * objects.
     */
    class TextureManager
    {
    public:
        /**
         * Get the texture associated with the given texture handle.
         *
         * If no texture associated with given handle, will return FALSE and
         * memory at the address given will be set to nullptr.
         *
         * @param   textureHandle  TextureHandle, texture handle to get texture
         * associated with.
         * @return                 ds_render::Texture *, pointer to texture or
         * nullptr.
         */
        ds_render::Texture *GetTexture(ds_render::TextureHandle textureHandle);

        /**
         * Get the texture associated with the given texture handle.
         *
         * If no texture associated with given handle, will return nullptr.
         *
         * @param   textureHandle  TextureHandle, texture handle to get texture
         * associated with.
         * @return                 const ds_render::Texture *, pointer to
         * texture or nullptr.
         */
        const ds_render::Texture *
        GetTexture(ds_render::TextureHandle textureHandle) const;

        /**
         * Get the handle to the texture associated with the given texture
         * resource handle, if no texture is associated with that texture
         * resource handle, one will be created and the caller will be given a
         * handle to it.
         *
         * If the result is TRUE, the textureHandle pointer is updated with the
         * texture handle, otherwise it is not touched.
         *
         * @param   textureResourceHandle  TextureResourceHandle, handle to
         * texture resource to get texture for.
         * @param   textureHandle          TextureHandle *, address to put
         * texture handle if found.
         * @return                         bool, TRUE if texture was created or
         * found for texture resource handle, FALSE otherwise.
         */
        bool
        GetTextureForResourceHandle(TextureResourceHandle textureResourceHandle,
                                    ds_render::TextureHandle *textureHandle);

    private:
        /** Store handle with managed texture object for update purposes */
        struct ManagedTexture
        {
            ds_render::TextureHandle handle;
            ds_render::Texture texture;
        };

        /** Texture storage */
        std::vector<ManagedTexture> m_textures;
        /** Handle manager */
        HandleManager m_handleManager;
    };

    /**
     * The material manager class manages access to and creation of material
     * objects.
     */
    class MaterialManager
    {
    public:
        /**
         * Get the material associated with the given material handle.
         *
         * If no material associated with given handle, will return FALSE and
         * memory at the address given will be set to nullptr.
         *
         * @param   materialHandle  MaterialHandle, material handle to get
         * material
         * associated with.
         * @return                  ds_render::Material *, pointer to material
         * or nullptr.
         */
        ds_render::Material *
        GetMaterial(ds_render::MaterialHandle materialHandle);

        /**
         * Get the material associated with the given material handle.
         *
         * If no material associated with given handle, will return nullptr.
         *
         * @param   materialHandle  MaterialHandle, material handle to get
         * material
         * associated with.
         * @return                 const ds_render::Material *, pointer to
         * material or nullptr.
         */
        const ds_render::Material *
        GetMaterial(ds_render::MaterialHandle materialHandle) const;

        /**
         * Get the handle to the material associated with the given material
         * resource handle, if no material is associated with that material
         * resource handle, one will be created and the caller will be given a
         * handle to it.
         *
         * If the result is TRUE, the materialHandle pointer is updated with the
         * material handle, otherwise it is not touched.
         *
         * @param   materialResourceHandle  MaterialResourceHandle, handle to
         * material resource to get material for.
         * @param   materialHandle          MaterialHandle *, address to put
         * material handle if found.
         * @return                         bool, TRUE if material was created or
         * found for material resource handle, FALSE otherwise.
         */
        bool GetMaterialForResourceHandle(
            MaterialResourceHandle materialResourceHandle,
            ds_render::MaterialHandle *materialHandle);

    private:
        /** Store handle with managed material object for update purposes */
        struct ManagedMaterial
        {
            ds_render::MaterialHandle handle;
            ds_render::Material material;
        };

        /** Material storage */
        std::vector<ManagedMaterial> m_materials;
        /** Handle manager */
        HandleManager m_handleManager;
    };

    /**
     * Initialize the render system.
     *
     * Returns FALSE if initialization fails.
     *
     * @param   config  const Config &, configuration loaded by engine.
     * @return          bool, TRUE if initialization succeeds, FALSE otherwise.
     */
    virtual bool Initialize(const Config &config);

    /**
     * Update the render system over the given timestep.
     *
     * @param  deltaTime  float, timestep to update the system over.
     */
    virtual void Update(float deltaTime);

    /**
     * Perform teardown of the render system.
     */
    virtual void Shutdown();

    /**
     * Give messages to the render system.
     *
     * Messages are not handled in this method. Handling of events is deferred
     * to the Update method.
     *
     * @param  messages  const ds_msg::MessageStream &messages, messages to send
     * to the Message Bus.
     */
    virtual void PostMessages(const ds_msg::MessageStream &messages);

    /**
     * Collect messages generated by the render system.
     *
     * @return  ds_msg::MessageStream, stream of messages generated by this
     * system.
     */
    virtual ds_msg::MessageStream CollectMessages();

    /**
     * Return required script bindings.
     *
     * @return  ScriptBindingSet, the script bindings the render system wants to
     * register with the Script system.
     */
    virtual ScriptBindingSet GetScriptBindings() const;

    /**
     * Get the orientation of the camera component associated with the given
     * entity (if any).
     *
     * Will return Quaternion(0, 0, 0, 1) if entity has no camera component.
     *
     * @param   entity  Entity, entity to get camera component orientation of.
     * @return          ds_math::Quaternion, camera orientation.
     */
    ds_math::Quaternion GetCameraOrientation(Entity entity) const;

    /**
     * Set the orientation of the camera component associated with the given
     * entity (if any).
     *
     * @param  entity      Entity, entity to set camera component orientation
     * of.
     * @param  orienation  const ds_math::Quaternion &, new orientation.
     */
    void SetCameraOrientation(Entity entity,
                              const ds_math::Quaternion &orientation);

private:
    /**
     * Process messages in the given message stream.
     *
     * @param  ds_msg::MessageStream *, messages to process.
     */
    void ProcessEvents(ds_msg::MessageStream *messages);

    /**
     * Create a Texture object from a texture sampler type and a list of file
     * paths that will be bound to that sampler. In the case of a cubemap
     * sampler for example, 6 images should be provided.
     *
     * @param   textureResourceFilePath const std::vector<std::string> &, path
     * to texture resource.
     * @return                          ds_render::Texture, texture created.
     */
    ds_render::Texture CreateTextureFromTextureResource(
        const std::string &textureResourceFilePath);

    /**
     * Create a Texture object from a texture resource handle.
     *
     * @param   handle  TextureResourceHandle, texture handle to create Texture
     * from.
     * @return          ds_render::Texture, texture object created.
     */
    static ds_render::Texture
    CreateTextureFromTextureResource(TextureResourceHandle handle);

    /**
     * Create the mesh for a skybox.
     *
     * @return  ds_render::Mesh, skybox mesh.
     */
    ds_render::Mesh CreateSkyboxMesh();

    /**
     * Create a Mesh object from a path to a mesh resource.
     *
     * @param   filePath  const std::string &, path to mesh resource.
     * @return            ds_render::Mesh, mesh created.
     */
    ds_render::Mesh CreateMeshFromMeshResource(const std::string &filePath);

    /**
     * Create a Material object from a path to a material resource.
     *
     * Will attempt to bind Scene and Object constant buffers by default.
     *
     * @param   filePath        const std::string &, path to material resource.
     * @param   sceneMatrices   ds_render::ConstantBufferHandle, handle to
     * constant buffer containing scene matrix data.
     * @param   objectMatrices  ds_render::ConstantBufferHandle, handle to
     * constant buffer containing object matrix data.
     * @return                  ds_render::Material, material created.
     */
    // ds_render::Material
    // CreateMaterialFromMaterialResource(const std::string &filePath);
    ds_render::Material CreateMaterialFromMaterialResource(
        const std::string &filePath,
        ds_render::ConstantBufferHandle sceneMatrices,
        ds_render::ConstantBufferHandle objectMatrices);

    /**
     * Create a Material object from a material resource handle.
     *
     * @param   handle  MaterialResourceHandle, material handle to create Material
     * from.
     * @return          ds_render::Material, material object created.
     */
    static ds_render::Material
    CreateMaterialFromMaterialResource(MaterialResourceHandle handle);

    /**
     * Create a render component for the given entity using the given component
     * data string.
     *
     * @param  entity         Entity, entity to create render component for.
     * @param  componentData  const std::string &, component data string to
     * create render component from.
     */
    // void CreateRenderComponentFor(Entity entity,
    //                               const std::string &componentData);

    /**
     * Render the scene.
     *
     * @param  deltaTime  float, time since last engine tick.
     */
    void RenderScene(float deltaTime);

    /**
     * Set the animation index (which animation to play) of the mesh associated
     * with the given entity (if any).
     *
     * @param  entity          Entity, entity to set animation of.
     * @param  animationIndex  int, index of animation to set.
     */
    void SetAnimationIndex(Entity entity, int animationIndex);

    /**
     * Set the skybox material.
     *
     * @param  skyboxMaterial  const std::string &, path to skybox material.
     */
    void SetSkyboxMaterial(const std::string &skyboxMaterial);

    /**
     * Create the mesh for a GUI panel.
     *
     * @param  startX        float, start x co-ordinate of the panel.
     * @param  startY        float, start y co-ordinate of the panel.
     * @param  endX          float, end x co-ordinate of the panel.
     * @param  endY          float, end y co-ordinate of the panel.
     * @param  materialPath  const std::string &, path to material to render
     * panel with.
     */
    ds_render::Mesh CreatePanelMesh(float startX,
                                    float startY,
                                    float endX,
                                    float endY,
                                    const std::string &materialPath);

    /** Messages generated and received by this system */
    ds_msg::MessageStream m_messagesGenerated, m_messagesReceived;

    /** Renderer */
    static std::unique_ptr<ds_render::IRenderer> m_renderer;

    /** Render component manager */
    ds_render::RenderComponentManager m_renderComponentManager;
    /** Transform component manager */
    TransformComponentManager m_transformComponentManager;
    /** Camera component manager */
    ds_render::CameraComponentManager m_cameraComponentManager;
    /** GUI-specific data */
    ds_render::ButtonComponentManager m_buttonComponentManager;

    ds_render::Mesh m_mesh;
    ds_render::Material m_material;
    ds_render::ProgramHandle m_program;

    static ds_render::ConstantBufferHandle m_sceneMatrices;
    static ds_render::ConstantBufferHandle m_objectMatrices;
    ds_render::ConstantBufferDescription m_sceneBufferDescrip;
    ds_render::ConstantBufferDescription m_objectBufferDescrip;

    ds_math::Matrix4 m_viewMatrix;
    ds_math::Matrix4 m_projectionMatrix;

    bool m_cameraActive;
    Entity m_activeCameraEntity;

    float m_timeInSeconds;

    /** Manage storage of mesh resources among other things */
    ds::HandleManager m_handleManager;

    /** Skybox */
    ds_render::Skybox m_skybox;
    bool m_hasSkybox;

    /** Texture resource manager */
    static TextureResourceManager m_textureResourceManager;
    /** Texture manager */
    static TextureManager m_textureManager;
    /** Material resource Manager */
    static MaterialResourceManager m_materialResourceManager;
    /** Material manager */
    static MaterialManager m_materialManager;

    /** Resource factory */
    static ResourceFactory m_factory;
};
}
