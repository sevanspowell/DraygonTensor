#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include "engine/Config.h"
#include "engine/common/Common.h"
#include "engine/resource/ShaderResource.h"
#include "engine/json/Json.h"

namespace ds
{
std::unique_ptr<IResource> ShaderResource::CreateFromFile(std::string filePath)
{
    std::unique_ptr<IResource> shaderResource(nullptr);

    // Open shader resource file
    std::ifstream shaderFile(filePath, std::ios::binary | std::ios::ate);
    bool didLoad = shaderFile.good();

    // Read file into buffer
    std::vector<char> shaderFileBuffer;
    if (didLoad)
    {
        // Reserve memory
        std::streamsize size = shaderFile.tellg();
        shaderFileBuffer.resize(size);

        // Read file
        shaderFile.seekg(0, shaderFile.beg);
        if (shaderFile.read(&shaderFileBuffer[0], size))
        {
            didLoad = true;
        }
        else
        {
            didLoad = false;
            shaderFileBuffer.resize(0);
        }
    }
    // Terminate string
    shaderFileBuffer.push_back('\0');

    if (didLoad)
    {
        // Create empty shader resource
        shaderResource = std::unique_ptr<IResource>(new ShaderResource());
        shaderResource->SetResourceFilePath(filePath);

        JsonObject root;
        json::parseObject(&shaderFileBuffer[0], &root);

        if (root["sources"] != nullptr)
        {
            JsonArray sources;
            json::parseArray(root["sources"], &sources);

            for (int iSource = 0; iSource < sources.size(); ++iSource)
            {
                JsonObject source;
                json::parseObject(sources[iSource], &source);

                if (source["type"] != nullptr && source["path"] != nullptr)
                {
                    std::string type;
                    std::string path;

                    json::parseString(source["type"], &type);
                    json::parseString(source["path"], &path);

                    // Construct full path
                    std::stringstream fullPath;
                    std::string folder = ds_com::GetParentDirectory(filePath);
                    fullPath << folder << path;

                    // Open source file
                    std::ifstream ifs;
                    ifs.open(fullPath.str(), std::fstream::in);
                    // If opened file successfully
                    if (!ifs.fail())
                    {
                        // Get source as string
                        std::string source;
                        ifs.seekg(0, std::ios::end);
                        source.resize(ifs.tellg());
                        ifs.seekg(0, std::ios::beg);
                        ifs.read(&source[0], source.size());
                        ifs.close();

                        // Translate key into shader source type
                        // Make sure key is valid
                        if (type == "Vertex" || type == "Fragment")
                        {
                            // Get shader type
                            ds_render::ShaderType shaderType =
                                ds_render::ShaderType::VertexShader;

                            if (type == "Vertex")
                            {
                                shaderType =
                                    ds_render::ShaderType::VertexShader;
                            }
                            if (type == "Fragment")
                            {
                                shaderType =
                                    ds_render::ShaderType::FragmentShader;
                            }

                            // Add to list of sources
                            static_cast<ShaderResource *>(shaderResource.get())
                                ->AddSource(shaderType, source);
                        }
                        else
                        {
                            std::cerr
                                << "ShaderResource::CreateFromFile: invalid "
                                   "shader type '"
                                << type << "' given." << std::endl;
                        }
                    }
                    else
                    {
                        std::cerr
                            << "ShaderResource::CreateFromFile: could not "
                               "open shader source: "
                            << fullPath.str() << std::endl;
                    }
                }
                else
                {
                    std::cerr
                        << "ShaderResource::CreateFromFile: could not find "
                           "'type' or 'path' field for array element "
                        << iSource << " in 'sources' resource: " << filePath
                        << std::endl;
                }
            }
        }
    }
    else
    {
        std::cerr << "ShaderResource::CreateFromFile: could not open shader "
                     "resource file (json file): "
                  << filePath << std::endl;
    }

    // Return resource (if any)
    return shaderResource;
}

const std::string &ShaderResource::GetResourceFilePath() const
{
    return m_filePath;
}

void ShaderResource::SetResourceFilePath(const std::string &filePath)
{
    m_filePath = filePath;
}

void ShaderResource::AddSource(ds_render::ShaderType type, std::string source)
{
    m_sources[type] = source;
}

std::vector<ds_render::ShaderType> ShaderResource::GetShaderTypes() const
{
    std::vector<ds_render::ShaderType> types;

    for (auto typeSource : m_sources)
    {
        types.push_back(typeSource.first);
    }

    return types;
}

const std::string &
ShaderResource::GetShaderSource(ds_render::ShaderType type) const
{
    std::map<ds_render::ShaderType, std::string>::const_iterator it =
        m_sources.find(type);

    if (it == m_sources.end())
    {
        assert(
            "ShaderResource::GetShaderSource: No shader of that type exists.");
    }

    return it->second;
}
}
