#pragma once

#include <string>

#include "engine/resource/ResourceFactory.h"
#include "engine/system/ISystem.h"
#include "engine/system/render/IRenderer.h"
#include "engine/system/render/Material.h"
#include "engine/system/render/Mesh.h"
#include "engine/system/render/Texture.h"

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

private:
    /**
     * Process messages in the given message stream.
     *
     * @param  ds_msg::MessageStream *, messages to process.
     */
    void ProcessEvents(ds_msg::MessageStream *messages);

    /**
     * Create a renderer texture and return a handle to it.
     *
     * @param   filePath  const std::string &, path to texture file.
     * @return            Texture, handle to renderer texture created. 
     */

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

    /** Messages generated and received by this system */
    ds_msg::MessageStream m_messagesGenerated, m_messagesReceived;

    ResourceFactory m_factory;

    /** Renderer */
    std::unique_ptr<ds_render::IRenderer> m_renderer;

    ds_render::Mesh m_mesh;
    ds_render::Texture m_texture;
    ds_render::Material m_material;
    ds_render::ProgramHandle m_program;

    // const char *m_vertexShader =
    //     "#version 330\n"
    //     "layout(location = 0) in vec3 vp;"
    //     "layout(location = 1) in vec2 vt;"
    //     "uniform Scene { mat4 modelMatrix; mat4 viewMatrix; mat4 "
    //     "projectionMatrix; } scene;\n"
    //     "out vec2 texCoord;"
    //     "void main() {"
    //     "  gl_Position = scene.projectionMatrix * scene.viewMatrix * "
    //     "scene.modelMatrix * vec4(vp, 1.0);"
    //     "  texCoord = vt;"
    //     "}";
    // const char *m_fragmentShader = "#version 330\n"
    //                                "in vec2 texCoord;"
    //                                "out vec4 frag_colour;"
    //                                "uniform sampler2D tex;"
    //                                "void main() {"
    //                                // "  frag_colour = vec4(1.0, 0.0, 0.0, 1.0);"
    //                                "frag_colour = texture(tex, texCoord);"
    //                                "}";
};
}
