#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#include "engine/resource/TextureResource.h"

namespace ds
{

<<<<<<< HEAD
TextureResource::TextureResource()
{
    m_textureData = nullptr;
    m_widthPixels = 0;
    m_heightPixels = 0;
}

const std::string &TextureResource::GetResourceFilePath() const
{
    return m_filePath;
}

void TextureResource::SetResourceFilePath(const std::string &filePath)
{
    m_filePath = filePath;
}

std::unique_ptr<IResource> TextureResource::CreateFromFile(std::string filePath)
{

    std::string fileExtension = ExtractExtension(filePath);
    ImageFormat typeFlag = DetermineTypeFlag(fileExtension);

    int widthInPixels = 0;
    int heightInPixels = 0;
    int components = 0;
    ComponentFlag channels;


    unsigned char *imageContents = stbi_load(filePath.c_str(), &widthInPixels,
                                             &heightInPixels, &components, 0);

    channels = static_cast<ComponentFlag>(components);

    std::unique_ptr<TextureResource> createdTexResource(new TextureResource());

    createdTexResource->SetWidthInPixels(widthInPixels);
    createdTexResource->SetHeightInPixels(heightInPixels);
    createdTexResource->SetComponentFlag(channels);
    createdTexResource->SetImageFormat(typeFlag);
    createdTexResource->SetTextureContents(imageContents);

    std::unique_ptr<IResource> convertedTexPointer =
        std::move(createdTexResource);


    return convertedTexPointer;
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
    ImageFormat type = ImageFormat::BMP;
    std::string fileExtension = ext;

    std::transform(fileExtension.begin(), fileExtension.end(),
                   fileExtension.begin(), ::tolower);
    std::cout << fileExtension << std::endl;

    if (ext == "tga")
    {
        type = ImageFormat::TGA;
    }
    else if (ext == "bmp")
    {
        type = ImageFormat::BMP;
    }
    else if (ext == "png")
    {
        type = ImageFormat::PNG;
    }
    else if (ext == "jpeg" || ext == "jpg")
    {
        type = ImageFormat::JPEG;
    }

    return type;
}

void TextureResource::SetImageFormat(ImageFormat format)
{
    m_imgFormat = format;
}

void TextureResource::SetComponentFlag(ComponentFlag comp)
{
    m_channelInfo = comp;
}

void TextureResource::SetWidthInPixels(int width)
{
    m_widthPixels = width;
}

void TextureResource::SetHeightInPixels(int height)
{
    m_heightPixels = height;
}

void TextureResource::SetTextureContents(unsigned char *textCont)
{
    m_textureData = textCont;
}

unsigned char *TextureResource::GetTextureContents()
{
    return m_textureData;
}

unsigned int TextureResource::GetWidthInPixels() const
{
    return m_widthPixels;
}

unsigned int TextureResource::GetHeightInPixels() const
{
    return m_heightPixels;
}

int TextureResource::GetImageFormat() const
{
    return m_imgFormat;
}

int TextureResource::GetComponentFlag() const
{
    return m_channelInfo;
}

bool TextureResource::IsRGB() const
{
    bool rgbFlag = false;

    if (m_channelInfo == RGB)
    {
        rgbFlag = true;
    }

    return rgbFlag;
}

bool TextureResource::IsRGBA() const
{
    bool rgbaFlag = false;

    if (m_channelInfo == RGBA)
    {
        rgbaFlag = true;
    }

    return rgbaFlag;
}

bool TextureResource::IsGreyAlpha() const
{
    bool greyAlphaFlag = false;

    if (m_channelInfo == GREYALPHA)
    {
        greyAlphaFlag = true;
    }

    return greyAlphaFlag;
}

bool TextureResource::IsGrey() const
{
    bool greyFlag = false;

    if (m_channelInfo == GREY)
    {
        greyFlag = true;
    }

    return greyFlag;
}

TextureResource::~TextureResource()
{
    stbi_image_free(m_textureData);
}
}
