#include <memory>
#include <algorithm>
#include <string>
#include <iostream>

#include "engine/resource/IResource.h"

namespace ds
{


/**
 * A texture resource.
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
     * Gets width in pixels.
     *
     * @return	The width in pixels.
     */

    unsigned int GetWidthInPixels() const;

    /**
     * Gets height in pixels.
     *
     * @return	The height in pixels.
     */

    unsigned int GetHeightInPixels() const;

    /**
     * Query if this object is RGB.
     *
     * @return	true if rgb, false if not.
     */

    bool IsRGB() const;

    /**
     * Query if this object is RGBA.
     *
     * @return	true if rgba, false if not.
     */

    bool IsRGBA() const;

    /**
     * Query if this object is grey alpha.
     *
     * @return	true if grey alpha, false if not.
     */

    bool IsGreyAlpha() const;

    /**
     * Query if this object is grey.
     *
     * @return	true if grey, false if not.
     */

    bool IsGrey() const;

    /**
     * Gets image format.
     *
     * @return	The image format.
     */

    int GetImageFormat() const;

    /**
     * Gets component flag.
     *
     * @return	The component flag.
     */

    int GetComponentFlag() const;

    /**
     * Gets texture contents.
     *
     * @return	null if it fails, else the texture contents.
     */

    unsigned char *GetTextureContents();


    /** Values that represent image formats. */
    enum ImageFormat
    {
        TGA = 1,
        BMP = 2,
        PNG = 3,
        JPEG = 4
    };

    /** Values that represent component flags. */
    enum ComponentFlag
    {
        GREY = 1,
        GREYALPHA = 2,
        RGB = 3,
        RGBA = 4
    };


private:
    /** Information describing the texture. */
    unsigned char *m_textureData = nullptr;
    /** The width pixels. */
    unsigned int m_widthPixels = 0;
    /** The height pixels. */
    unsigned int m_heightPixels = 0;
    /** Information describing the channel. */
    ComponentFlag m_channelInfo;
    /** The image format. */
    ImageFormat m_imgFormat;
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
     * Sets image format.
     *
     * @param	format	Describes the format loaded.
     */

    void SetImageFormat(ImageFormat format);

    /**
     * Sets component flag.
     *
     * @param	comp	The component/channel.
     */

    void SetComponentFlag(ComponentFlag comp);

    /**
     * Sets width in pixels.
     *
     * @param	width	The width in pixels.
     */

    void SetWidthInPixels(int width);

    /**
     * Sets height in pixels.
     *
     * @param	height	The height in pixels.
     */

    void SetHeightInPixels(int height);

    /**
     * Sets texture contents.
     *
     * @param [in,out]	textCont	If non-null, the text container.
     */

    void SetTextureContents(unsigned char *textCont);
};
}
