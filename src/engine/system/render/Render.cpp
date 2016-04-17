#include <fstream>
#include <sstream>

#include "engine/resource/MaterialResource.h"
#include "engine/resource/MeshResource.h"
#include "engine/resource/ShaderResource.h"
#include "engine/resource/TextureResource.h"
#include "engine/system/render/GLRenderer.h"
#include "engine/system/render/Render.h"
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

    return result;
}

void Render::Update(float deltaTime)
{
    ProcessEvents(&m_messagesReceived);

    m_renderer->ClearBuffers();
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
