#define STB_IMAGE_IMPLEMENTATION

#include <fstream>
#include <sstream>

#include <stb_image.h>

#include "engine/Config.h"
#include "engine/common/Common.h"
#include "engine/json/Json.h"
#include "engine/resource/TextureResource.h"

namespace ds
{
std::unique_ptr<IResource> TextureResource::CreateFromFile(std::string filePath)
{
    // Open texture resource file
    std::ifstream texFile(filePath, std::ios::binary | std::ios::ate);
    bool didLoad = texFile.good();

    // Read file into buffer
    std::vector<char> texFileBuffer;
    if (didLoad)
    {
        // Reserve memory
        std::streamsize size = texFile.tellg();
        texFileBuffer.resize(size);

        // Read file
        texFile.seekg(0, texFile.beg);
        if (texFile.read(&texFileBuffer[0], size))
        {
            didLoad = true;
        }
        else
        {
            didLoad = false;
            texFileBuffer.resize(0);
        }
    }
    // Terminate string
    texFileBuffer.push_back('\0');

    std::unique_ptr<IResource> convertedTexPointer(nullptr);

    if (didLoad)
    {
        // Create empty texture resource
        std::unique_ptr<TextureResource> createdTexResource(
            new TextureResource());
        createdTexResource->SetResourceFilePath(filePath);

        JsonObject root;
        json::parseObject(&texFileBuffer[0], &root);

        if (root["type"] != nullptr)
        {
            // Get type of the texture
            std::string textureTypeAsString;

            json::parseString(root["type"], &textureTypeAsString);

            ds_render::TextureType textureType = ds_render::TextureType::None;

            if (textureTypeAsString == "2D")
            {
                textureType = ds_render::TextureType::TwoDimensional;
            }
            else if (textureTypeAsString == "Cubemap")
            {
                textureType = ds_render::TextureType::Cubemap;
            }

            createdTexResource->SetTextureType(textureType);
        }

        // Get containing folder of this texture resource
        std::string folder = ds_com::GetParentDirectory(filePath);

        if (root["images"] != nullptr)
        {
            JsonArray images;
            json::parseArray(root["images"], &images);

            // Make sure the tex resource has the right number of images
            createdTexResource->SetNumImages(images.size());

            for (unsigned int iImage = 0; iImage < images.size(); ++iImage)
            {
                std::string imagePath;
                json::parseString(images[iImage], &imagePath);

                // Load image
                std::string fileExtension = ExtractExtension(imagePath);
                ImageFormat typeFlag = DetermineTypeFlag(fileExtension);

                int widthInPixels = 0;
                int heightInPixels = 0;

                std::stringstream fullImagePath;
                fullImagePath << "../assets/" << imagePath;

                unsigned char *imageContents =
                    stbi_load(fullImagePath.str().c_str(), &widthInPixels,
                              &heightInPixels, nullptr, 4);

                createdTexResource->SetWidthInPixels(iImage, widthInPixels);
                createdTexResource->SetHeightInPixels(iImage, heightInPixels);
                createdTexResource->SetComponentFlag(
                    iImage, ComponentFlag::COMPONENT_FLAG_RGBA);
                createdTexResource->SetImageFormat(iImage, typeFlag);
                createdTexResource->SetTextureContents(iImage, imageContents);
            }

            convertedTexPointer = std::move(createdTexResource);
        }
    }
    else
    {
        std::cerr << "TextureResource::CreateFromFile: could not open texture "
                     "resource file: "
                  << filePath << std::endl;
    }

    return convertedTexPointer;
}

TextureResource::TextureResource()
{
    m_textureType = ds_render::TextureType::None;
}

TextureResource::~TextureResource()
{
    for (unsigned int i = 0; i < m_images.size(); ++i)
    {
        free(m_images[i].imageData);
    }
}

TextureResource::TextureResource(const TextureResource &other)
{
    // Make sure we have the correct number of images
    this->m_images.resize(other.m_images.size());

    // For each image in other texture
    for (unsigned int i = 0; i < other.m_images.size(); ++i)
    {
        // Copy image data
        size_t imageSize =
            sizeof(unsigned char) * other.m_images[i].widthPixels *
            other.m_images[i].heightPixels * (int)other.m_images[i].channelInfo;
        this->m_images[i].imageData = (unsigned char *)malloc(imageSize);
        memcpy(this->m_images[i].imageData, other.m_images[i].imageData,
               imageSize);

        // Copy other
        this->m_images[i].widthPixels = other.m_images[i].widthPixels;
        this->m_images[i].heightPixels = other.m_images[i].heightPixels;
        this->m_images[i].channelInfo = other.m_images[i].channelInfo;
        this->m_images[i].imgFormat = other.m_images[i].imgFormat;
    }

    this->m_filePath = other.m_filePath;
    this->m_textureType = other.m_textureType;
}

TextureResource &TextureResource::operator=(const TextureResource &other)
{
    // Delete current image data
    for (unsigned int i = 0; i < this->m_images.size(); ++i)
    {
        if (this->m_images[i].imageData != nullptr)
        {
            free(this->m_images[i].imageData);
        }
    }

    // Resize our image vector
    this->m_images.resize(other.m_images.size());

    // For each image in other texture
    for (unsigned int i = 0; i < other.m_images.size(); ++i)
    {
        // Copy image data
        size_t imageSize =
            sizeof(unsigned char) * other.m_images[i].widthPixels *
            other.m_images[i].heightPixels * (int)other.m_images[i].channelInfo;
        this->m_images[i].imageData = (unsigned char *)malloc(imageSize);
        memcpy(this->m_images[i].imageData, other.m_images[i].imageData,
               imageSize);

        // Copy other
        this->m_images[i].widthPixels = other.m_images[i].widthPixels;
        this->m_images[i].heightPixels = other.m_images[i].heightPixels;
        this->m_images[i].channelInfo = other.m_images[i].channelInfo;
        this->m_images[i].imgFormat = other.m_images[i].imgFormat;
    }

    this->m_filePath = other.m_filePath;
    this->m_textureType = other.m_textureType;

    return *this;
}

TextureResource::TextureResource(TextureResource &&other)
{
    // Resize our image vector
    this->m_images.resize(other.m_images.size());

    // For each image in other texture
    for (unsigned int i = 0; i < other.m_images.size(); ++i)
    {
        // 'Steal' image data
        this->m_images[i].imageData = other.m_images[i].imageData;
        other.m_images[i].imageData = nullptr;

        // Copy other fields
        this->m_images[i].widthPixels = other.m_images[i].widthPixels;
        this->m_images[i].heightPixels = other.m_images[i].heightPixels;
        this->m_images[i].channelInfo = other.m_images[i].channelInfo;
        this->m_images[i].imgFormat = other.m_images[i].imgFormat;
    }

    this->m_filePath = other.m_filePath;
    this->m_textureType = other.m_textureType;
}

TextureResource &TextureResource::operator=(TextureResource &&other)
{
    // Delete any current image data
    for (unsigned int i = 0; i < this->m_images.size(); ++i)
    {
        // Delete current texture data
        if (this->m_images[i].imageData != nullptr)
        {
            free(this->m_images[i].imageData);
        }
    }

    // Resize our image vector
    this->m_images.resize(other.m_images.size());

    // For each image in other texture
    for (unsigned int i = 0; i < other.m_images.size(); ++i)
    {
        // 'Steal' image data
        this->m_images[i].imageData = other.m_images[i].imageData;
        other.m_images[i].imageData = nullptr;

        // Copy other fields
        this->m_images[i].widthPixels = other.m_images[i].widthPixels;
        this->m_images[i].heightPixels = other.m_images[i].heightPixels;
        this->m_images[i].channelInfo = other.m_images[i].channelInfo;
        this->m_images[i].imgFormat = other.m_images[i].imgFormat;
    }

    this->m_filePath = other.m_filePath;
    this->m_textureType = other.m_textureType;

    return *this;
}

const std::string &TextureResource::GetResourceFilePath() const
{
    return m_filePath;
}

void TextureResource::SetResourceFilePath(const std::string &filePath)
{
    m_filePath = filePath;
}

ds_render::TextureType TextureResource::GetTextureType() const
{
    return m_textureType;
}

unsigned int TextureResource::GetNumImages() const
{
    return m_images.size();
}

void TextureResource::SetTextureType(ds_render::TextureType textureType)
{
    m_textureType = textureType;
}

void TextureResource::SetNumImages(unsigned int numImages)
{
    m_images.resize(numImages);
}

unsigned int TextureResource::GetWidthInPixels(unsigned int imageIndex) const
{
    assert(imageIndex >= 0 && imageIndex < m_images.size() &&
           "TextureResource::GetWidthInPixels: Invalid image index.");

    return m_images[imageIndex].widthPixels;
}

unsigned int TextureResource::GetHeightInPixels(unsigned int imageIndex) const
{
    assert(imageIndex >= 0 && imageIndex < m_images.size() &&
           "TextureResource::GetHeightInPixels: Invalid image index.");

    return m_images[imageIndex].heightPixels;
}

int TextureResource::GetImageFormat(unsigned int imageIndex) const
{
    assert(imageIndex >= 0 && imageIndex < m_images.size() &&
           "TextureResource::GetImageFormat: Invalid image index.");

    return m_images[imageIndex].imgFormat;
}

int TextureResource::GetComponentFlag(unsigned int imageIndex) const
{
    assert(imageIndex >= 0 && imageIndex < m_images.size() &&
           "TextureResource::GetComponentFlag: Invalid image index.");

    return m_images[imageIndex].channelInfo;
}

const unsigned char *
TextureResource::GetImageData(unsigned int imageIndex) const
{
    assert(imageIndex >= 0 && imageIndex < m_images.size() &&
           "TextureResource::GetImageData: Invalid image index.");

    return m_images[imageIndex].imageData;
}

std::string TextureResource::ExtractExtension(std::string path)
{
    std::string ext = "";

    size_t foundPos = path.find_last_of(".");

    if (foundPos == std::string::npos)
    {
        std::string errorMessage = "Cannot load texture:";

        std::cout << errorMessage << std::endl;
        std::cout << path << std::endl;

        return NULL;
    }
    else
    {
        ext = path.substr(foundPos + 1);
        return ext;
    }
}

TextureResource::ImageFormat TextureResource::DetermineTypeFlag(std::string ext)
{
    ImageFormat type = ImageFormat::IMAGE_FORMAT_BMP;
    std::string fileExtension = ext;

    std::transform(fileExtension.begin(), fileExtension.end(),
                   fileExtension.begin(), ::tolower);
    // std::cout << fileExtension << std::endl;

    if (ext == "tga")
    {
        type = ImageFormat::IMAGE_FORMAT_TGA;
    }
    else if (ext == "bmp")
    {
        type = ImageFormat::IMAGE_FORMAT_BMP;
    }
    else if (ext == "png")
    {
        type = ImageFormat::IMAGE_FORMAT_PNG;
    }
    else if (ext == "jpeg" || ext == "jpg")
    {
        type = ImageFormat::IMAGE_FORMAT_JPEG;
    }

    return type;
}

void TextureResource::SetImageFormat(unsigned int imageIndex,
                                     ImageFormat format)
{
    assert(imageIndex >= 0 && imageIndex < m_images.size() &&
           "TextureResource::SetImageFormat: Invalid image index.");

    m_images[imageIndex].imgFormat = format;
}

void TextureResource::SetComponentFlag(unsigned int imageIndex,
                                       ComponentFlag comp)
{
    assert(imageIndex >= 0 && imageIndex < m_images.size() &&
           "TextureResource::SetComponentFlag: Invalid image index.");

    m_images[imageIndex].channelInfo = comp;
}

void TextureResource::SetWidthInPixels(unsigned int imageIndex,
                                       unsigned int width)
{
    assert(imageIndex >= 0 && imageIndex < m_images.size() &&
           "TextureResource::SetWidthInPixels: Invalid image index.");

    m_images[imageIndex].widthPixels = width;
}

void TextureResource::SetHeightInPixels(unsigned int imageIndex,
                                        unsigned int height)
{
    assert(imageIndex >= 0 && imageIndex < m_images.size() &&
           "TextureResource::SetHeightInPixels: Invalid image index.");

    m_images[imageIndex].heightPixels = height;
}

void TextureResource::SetTextureContents(unsigned int imageIndex,
                                         unsigned char *textCont)
{
    assert(imageIndex >= 0 && imageIndex < m_images.size() &&
           "TextureResource::SetTextureContents: Invalid image index.");

    m_images[imageIndex].imageData = textCont;
}
}
