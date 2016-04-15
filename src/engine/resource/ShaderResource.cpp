#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>

#include "engine/Config.h"
#include "engine/common/Common.h"
#include "engine/resource/ShaderResource.h"

namespace ds
{
std::unique_ptr<IResource> ShaderResource::CreateFromFile(std::string filePath)
{
    std::unique_ptr<IResource> shaderResource(nullptr);

    // Open shader resource file
    Config config;
    bool didLoad = config.LoadFile(filePath);

    if (didLoad)
    {
        // Create empty shader resource
        shaderResource = std::unique_ptr<IResource>(new ShaderResource());
        shaderResource->SetResourceFilePath(filePath);

        // Get shader source keys
        std::vector<std::string> sourceKeys = config.GetObjectKeys("sources");

        // For each key
        for (auto key : sourceKeys)
        {
            // Get source file path
            std::stringstream ss;
            ss << "sources"
               << "." << key;
            std::string relPath;
            if (config.GetString(ss.str(), &relPath))
            {
                // Construct full path
                std::stringstream fullPath;
                std::string folder = ds_com::GetParentDirectory(filePath);
                fullPath << folder << relPath;

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
                    if (key == "Vertex" || key == "Fragment")
                    {
                        // Get shader type
                        ShaderType type = ShaderType::Vertex;

                        if (key == "Vertex")
                        {
                            type = ShaderType::Vertex;
                        }
                        if (key == "Fragment")
                        {
                            type = ShaderType::Fragment;
                        }

                        // Add to list of sources
                        static_cast<ShaderResource *>(shaderResource.get())
                            ->AddSource(type, source);
                    }
                    else
                    {
                        std::cerr << "ShaderResource::CreateFromFile: invalid "
                                     "shader key '"
                                  << key << "' given." << std::endl;
                    }
                }
                else
                {
                    std::cerr << "ShaderResource::CreateFromFile: could not "
                                 "open shader source: "
                              << fullPath.str() << std::endl;
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

void ShaderResource::AddSource(ShaderType type, std::string source)
{
    m_sources[type] = source;
}

std::vector<ShaderResource::ShaderType> ShaderResource::GetShaderTypes() const
{
    std::vector<ShaderType> types;

    for (auto typeSource : m_sources)
    {
        types.push_back(typeSource.first);
    }

    return types;
}

const std::string &ShaderResource::GetShaderSource(ShaderType type) const
{
    std::map<ShaderType, std::string>::const_iterator it = m_sources.find(type);

    if (it == m_sources.end())
    {
        assert(
            "ShaderResource::GetShaderSource: No shader of that type exists.");
    }

    return it->second;
}
}
