#include <fstream>

#include "engine/resource/MaterialResource.h"
#include "engine/resource/MeshResource.h"
#include "engine/resource/ShaderResource.h"
#include "engine/resource/TextureResource.h"
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


    //   // Example ...
    // std::string meshResourcePath = "../assets/boot.lua";

    //   std::unique_ptr<IResource> meshResource =
    //       m_factory.CreateResource<MeshResource>(meshResourcePath);


    /* Example using shader gen */
    // std::string shaderExamplePath = "../assets/diffuse.shader";
    // std::unique_ptr<ShaderResource> shaderResource =
    //     m_factory.CreateResource<ShaderResource>(shaderExamplePath);
    // std::cout << shaderResource->GetResourceFilePath() << std::endl;
    // // For each shader type
    // for (auto shaderType : shaderResource->GetShaderTypes())
    // {
    //     std::cout << shaderResource->GetShaderSource(shaderType) <<
    //     std::endl;
    // }

    /* Example using material gen */
    // std::string materialExamplePath = "../assets/test.material";
    // std::unique_ptr<MaterialResource> materialResource =
    //     m_factory.CreateResource<MaterialResource>(materialExamplePath);
    // std::cout << materialResource->GetResourceFilePath() << std::endl;
    // const std::vector<ds_render::Uniform> &uniforms =
    //     materialResource->GetUniforms();
    // for (const ds_render::Uniform &uniform : uniforms)
    // {
    //     switch (uniform.GetDataType())
    //     {
    //     case ds_render::RenderDataType::Int:
    //         std::cout << uniform.GetName() << " int "
    //                   << *((const int *)uniform.GetUniformData()) << std::endl;
    //         break;
    //     case ds_render::RenderDataType::Vec4:
    //         std::cout << uniform.GetName() << " vec4 "
    //                   << *((ds_math::Vector4 *)uniform.GetUniformData())
    //                   << std::endl;
    //         break;
    //     }
    // }

    /* Example using the texture gen */
    // std::string textureExamplePath = "../assets/test.png";
    // std::unique_ptr<IResource> texResource =
    //	m_factory.CreateResource<TextureResource>(textureExamplePath);
    // std::unique_ptr<TextureResource> changedResourcePointer
    //	(static_cast<TextureResource*>(texResource.release()));

    // std::cout << "width: " << changedResourcePointer->GetWidthInPixels() <<
    // std::endl;
    // std::cout << "height: " << changedResourcePointer->GetHeightInPixels() <<
    // std::endl;

    // if (changedResourcePointer->GetImageFormat() == TextureResource::PNG) {
    //	std::cout << "Format read - Yes." << std::endl;
    //}

    // std::cout << "Components/Channels: " <<
    // (int)changedResourcePointer->GetComponentFlag() << std::endl;

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
