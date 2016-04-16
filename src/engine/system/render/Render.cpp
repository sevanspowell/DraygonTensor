#pragma once

#include <fstream>


#include "engine/resource/MaterialResource.h"
#include "engine/resource/MeshResource.h"
#include "engine/resource/ShaderResource.h"
#include "engine/resource/TextureResource.h"
#include "engine/system/render/Render.h"

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


	//// Example of new mesh loading.
	//std::unique_ptr<MeshResource> changedResourcePointer =
	//	m_factory.CreateResource<MeshResource>("../assets/cube.obj");
	//
	//std::cout << "Ind: " << changedResourcePointer->GetIndicesCount() << std::endl;
	//std::cout << "Vert: " << changedResourcePointer->GetVertCount()<< std::endl;
	//std::cout << "Mesh: " << changedResourcePointer->GetMeshCount() << std::endl;
	//std::cout << "Tex: " << changedResourcePointer->GetTexCoordCount() << std::endl;
	//std::cout << "Norm: " << changedResourcePointer->GetNormalsCount() << std::endl;

	//std::vector<ds_math::Vector3> verts = changedResourcePointer->GetVerts();
	//for (int i = 0; i < changedResourcePointer->GetVertCount(); i++)
	//{
	//	std::cout << "x: " << verts[i].x;
	//	std::cout << ", Y: " << verts[i].y;
	//	std::cout << ", Z :" << verts[i].z << std::endl;
	//}
	// 
	
	return result;
}

void Render::Update(float deltaTime)
{
    ProcessEvents(&m_messagesReceived);
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
        //     if (m_renderer == nullptr)
        //     {
        //         // Create a renderer to match graphics context type
        //         // GL renderer for GL context, etc...
        //         if (gfxContext.contextInfo.type =
        //                 GraphicsContext::ContextType::OpenGL)
        //         {
        //         }
        //     }
        //     break;
        default:
            messages->Extract(header.size);
            break;
        }
    }
}
}
