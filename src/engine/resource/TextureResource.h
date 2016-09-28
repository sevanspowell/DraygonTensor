#pragma once

#include <memory>
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>

#include "engine/resource/IResource.h"
#include "engine/system/render/RenderCommon.h"

namespace ds
{
/**
 * A texture resource.
 *
 * @author Samuel Evans-Powell
 *
 */
class TextureResource : public IResource
{
public:
    /**
     * Create a texture resource from file.
     *
     * @param   fileIn  std::string, file path to create texture resource
     * from.
     * @return          std::unique_ptr<IResource>, pointer to texture resource
     * created.
     */
    static std::unique_ptr<IResource> CreateFromFile(std::string filePath);

    /**
     * Default constructor.
     */
    TextureResource();

    /**
     * Copy constructor.
     *
     * @param  other  const TextureResource &, texture resource to
     * copy.
     */
    TextureResource(const TextureResource &other);

    /**
     * Move constructor.
     *
     * @param  other  TextureResource &&, texture resource to move.
     */
    TextureResource(TextureResource &&other);

    /**
     * Copy assignment operator.
     *
     * @param   other  TextureResource &, texture resource to copy.
     * @return         TextureResource &, copied texture resource.
     */
    TextureResource &operator=(const TextureResource &other);

    /**
     * Move assignment operator.
     *
     * @param   other  TextureResource &&, texture resource to move.
     * @return         TextureResource &, moved texture resource.
     */
    TextureResource &operator=(TextureResource &&other);

    /**
     * Destructor.
     */
    ~TextureResource();

    /**
     * Get the file path to the resource.
     *
     * @return  const std::string &, resource file path.
     */
    virtual const std::string &GetResourceFilePath() const;

    /**
     * Set the file path to the resource.
     *
     * @param  filePath  const std::string &, file path of this resource.
     */
    virtual void SetResourceFilePath(const std::string &filePath);

    /**
     * Get the type of this texture.
     *
     * @return  ds_render::TextureType, type of this texture.
     */
    ds_render::TextureType GetTextureType() const;

    /**
     * Get the number of images that make up this texture.
     *
     * @return  unsigned int, number of images that make up this texture.
     */
    unsigned int GetNumImages() const;

    /**
     * Gets width in pixels for a particular image in the texture.
     *
     * @param   imageIndex  unsigned int, index of image to get the width in
     * pixels of.
     * @return	unsigned int, the width in pixels of the given image in the
     * texture.
     */
    unsigned int GetWidthInPixels(unsigned int imageIndex) const;

    /**
     * Gets height in pixels for a particular image in the texture.
     *
     * @param   imageIndex  unsigned int, index of image to get the height in
     * pixels of.
     * @return	unsigned int, the height in pixels of the given image in the
     * texture.
     */
    unsigned int GetHeightInPixels(unsigned int imageIndex) const;

    /**
     * Gets image format for a particular image in the texture.
     *
     * @param   imageIndex  unsigned int, index of image to get the image format
     * for.
     * @return	int, The image format of the given image in the texture.
     */
    int GetImageFormat(unsigned int imageIndex) const;

    /**
     * Get the component flag for a particular image in the texture.
     *
     * @param   imageIndex  unsigned int, index of image to get component flag
     * for.
     * @return	int, the component flag for the given image in the texture.
     */
    int GetComponentFlag(unsigned int imageIndex) const;

    /**
     * Get the image data for a particular image in the texture.
     *
     * @param   imageIndex  unsigned int, index of image to get data for.
     * @return	            const unsigned char *, nullptr if method fails, else
     * the texture contents.
     */
    const unsigned char *GetImageData(unsigned int imageIndex) const;


    /** Values that represent image formats. */
    enum ImageFormat
    {
        IMAGE_FORMAT_NONE = 0,
        IMAGE_FORMAT_TGA = 1,
        IMAGE_FORMAT_BMP = 2,
        IMAGE_FORMAT_PNG = 3,
        IMAGE_FORMAT_JPEG = 4
    };

    /** Values that represent component flags. */
    enum ComponentFlag
    {
        COMPONENT_FLAG_NONE = 0,
        COMPONENT_FLAG_GREY = 1,
        COMPONENT_FLAG_GREYALPHA = 2,
        COMPONENT_FLAG_RGB = 3,
        COMPONENT_FLAG_RGBA = 4
    };


private:
    struct Image
    {
        Image()
        {
            imageData = nullptr;
            widthPixels = 0;
            heightPixels = 0;
            channelInfo = ComponentFlag::COMPONENT_FLAG_NONE;
            imgFormat = ImageFormat::IMAGE_FORMAT_NONE;
        };

        /** Information describing the texture. */
        unsigned char *imageData;
        /** The width pixels. */
        unsigned int widthPixels;
        /** The height pixels. */
        unsigned int heightPixels;
        /** Information describing the channel. */
        ComponentFlag channelInfo;
        /** The image format. */
        ImageFormat imgFormat;
    };

    /** Type of the texture */
    ds_render::TextureType m_textureType;

    /** Images making up texture */
    std::vector<Image> m_images;

    /** The path to this resource */
    std::string m_filePath;

    /**
     * Extracts the extension described by path.
     *
     * @param	path	Full pathname of the file.
     * @return	The extracted extension.
     */
    static std::string ExtractExtension(std::string path);

    /**
     * Determine type flag.
     *
     * @param	ext	The extentension.
     * @return	An ImageFormat.
     */
    static ImageFormat DetermineTypeFlag(std::string ext);

    /**
     * Set the texture type of the texture.
     *
     * @param  textureType  ds_render::TextureType, type of the texture to set.
     */
    void SetTextureType(ds_render::TextureType textureType);

    /**
     * Set the number of images the texture resource contains. Will destory
     * textures that already exist if size is set smaller than current size.
     *
     * @param  numImages  unsigned int, number of images this texture resource
     * is made up of.
     */
    void SetNumImages(unsigned int numImages);

    /**
     * Sets image format of a particular image in the texture.
     *
     * @param  imageIndex  unsigned int, index of image to set the image
     * format of.
     * @param  format	   ImageFormat, format of the image to set.
     */
    void SetImageFormat(unsigned int imageIndex, ImageFormat format);

    /**
     * Sets component flag of a particular image in the texture.
     *
     * @param  imageIndex  unsigned int, index of image to set the component
     * flag of.
     * @param  comp        The component/channel to set for the given image.
     */
    void SetComponentFlag(unsigned int imageIndex, ComponentFlag comp);

    /**
     * Sets width in pixels of a particular image in the texture.
     *
     * @param   imageIndex  unsigned int, image index to set the width in
     * pixels of.
     * @param	width	    unsigned int, width in pixels to set.
     */
    void SetWidthInPixels(unsigned int imageIndex, unsigned int width);

    /**
     * Sets height in pixels of a particular image in the texture.
     *
     * @param   imageIndex  unsigned int, image index to set the height in
     * pixels of.
     * @param	height	    unsigned int, height in pixels to set.
     */
    void SetHeightInPixels(unsigned int imageIndex, unsigned int height);

    /**
     * Sets texture contents of a particular image in the texture.
     *
     * @param  imageIndex  unsigned int, image index to set the texture contents
     * of.
     * @param  textCont	   unsigned char *, texture contents to set.
     */
    void SetTextureContents(unsigned int imageIndex, unsigned char *textCont);
};
}
