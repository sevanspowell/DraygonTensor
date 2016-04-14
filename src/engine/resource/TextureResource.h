#include <memory>
#include <algorithm>
#include <string>
#include <iostream>

#include "engine/resource/IResource.h"

namespace ds
{


/**
 * A texture resource.
 *
 * @author	Luke Sharland
 * @date	14/04/2016
 */
class TextureResource : public IResource
{
public:
    /**
     * Create a texture resource from file.
     *
     * @param   fileIn  std::string &, file path to create texture resource
     * from.
     * @return          std::unique_ptr<IResource>, pointer to texture resource
     * created.
     */
    static std::unique_ptr<IResource> CreateFromFile(std::string filePath);

    /**
     * Default constructor.
     *
     * @author	Luke Sharland
     * @date	14/04/2016
     */

    TextureResource();

	/**
	 * Destructor.
	 *
	 * @author	Luke Sharland
	 * @date	14/04/2016
	 */

	~TextureResource();

    /**
     * Gets width in pixels.
     *
     * @author	Luke Sharland
     * @date	14/04/2016
     *
     * @return	The width in pixels.
     */

    unsigned int GetWidthInPixels() const;

    /**
     * Gets height in pixels.
     *
     * @author	Luke Sharland
     * @date	14/04/2016
     *
     * @return	The height in pixels.
     */

    unsigned int GetHeightInPixels() const;

    /**
     * Query if this object is RGB.
     *
     * @author	Luke Sharland
     * @date	14/04/2016
     *
     * @return	true if rgb, false if not.
     */

    bool IsRGB() const;

    /**
     * Query if this object is RGBA.
     *
     * @author	Luke Sharland
     * @date	14/04/2016
     *
     * @return	true if rgba, false if not.
     */

    bool IsRGBA() const;

    /**
     * Query if this object is grey alpha.
     *
     * @author	Luke Sharland
     * @date	14/04/2016
     *
     * @return	true if grey alpha, false if not.
     */

    bool IsGreyAlpha() const;

    /**
     * Query if this object is grey.
     *
     * @author	Luke Sharland
     * @date	14/04/2016
     *
     * @return	true if grey, false if not.
     */

    bool IsGrey() const;

    /**
     * Gets image format.
     *
     * @author	Luke Sharland
     * @date	14/04/2016
     *
     * @return	The image format.
     */

    int GetImageFormat() const;

    /**
     * Gets component flag.
     *
     * @author	Luke Sharland
     * @date	14/04/2016
     *
     * @return	The component flag.
     */

    int GetComponentFlag() const;

    /**
     * Gets texture contents.
     *
     * @author	Luke Sharland
     * @date	14/04/2016
     *
     * @return	null if it fails, else the texture contents.
     */

    unsigned char * GetTextureContents();

    
    
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
	unsigned char * m_textureData = nullptr;
    /** The width pixels. */
    unsigned int m_widthPixels = 0;
    /** The height pixels. */
    unsigned int m_heightPixels = 0;
	/** Information describing the channel. */
	ComponentFlag m_channelInfo;
	/** The image format. */
	ImageFormat m_imgFormat;

    /**
     * Extracts the extension described by path.
     *
     * @author	Luke Sharland
     * @date	14/04/2016
     *
     * @param	path	Full pathname of the file.
     *
     * @return	The extracted extension.
     */

    static std::string ExtractExtension(std::string path);

    /**
     * Determine type flag.
     *
     * @author	Luke Sharland
     * @date	14/04/2016
     *
     * @param	ext	The extentension.
     *
     * @return	An ImageFormat.
     */

    static ImageFormat DetermineTypeFlag(std::string ext);

    /**
     * Sets image format.
     *
     * @author	Luke Sharland
     * @date	14/04/2016
     *
     * @param	format	Describes the format loaded.
     */

    void SetImageFormat(ImageFormat format);

    /**
     * Sets component flag.
     *
     * @author	Luke Sharland
     * @date	14/04/2016
     *
     * @param	comp	The component/channel.
     */

    void SetComponentFlag(ComponentFlag comp);

    /**
     * Sets width in pixels.
     *
     * @author	Luke Sharland
     * @date	14/04/2016
     *
     * @param	width	The width in pixels.
     */

    void SetWidthInPixels(int width);

    /**
     * Sets height in pixels.
     *
     * @author	Luke Sharland
     * @date	14/04/2016
     *
     * @param	height	The height in pixels.
     */

    void SetHeightInPixels(int height);

    /**
     * Sets texture contents.
     *
     * @author	Luke Sharland
     * @date	14/04/2016
     *
     * @param [in,out]	textCont	If non-null, the text container.
     */

    void SetTextureContents(unsigned char * textCont);
    
    };
}
