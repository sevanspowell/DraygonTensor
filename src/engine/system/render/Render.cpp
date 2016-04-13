#include "engine/system/render/Render.h"

namespace ds
{
bool Render::Initialize(const Config &config)
{
    bool result = false;

    return result;
}

void Render::Update(float deltaTime)
{
    ProcessEvents(m_messagesReceived);
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
            ds_msg::GraphicsContextCreated gfxContext;
            (*messages) >> gfxContext;

            // If we haven't already created a renderer
            if (m_renderer == nullptr)
            {
                // Create a renderer to match graphics context type
                // GL renderer for GL context, etc...
                if (gfxContext.contextInfo.type =
                        GraphicsContext::ContextType::OpenGL)
                {
                }
            }
            break;
        default:
            messages->Extract(header.size);
            break;
        }
    }
}
