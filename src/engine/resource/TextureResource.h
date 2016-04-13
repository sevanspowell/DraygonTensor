#include <memory>

#include "engine/resource/IResource.h"

namespace ds
{
class TextureResource : public IResource
{
public:
    /**
     * Create a texture resource from file.
     *
     * @param   fileIn  std::fstream &, file stream to create texture resource
     * from.
     * @return          std::unique_ptr<IResource>, pointer to texture resource
     * created.
     */
    static std::unique_ptr<IResource> CreateFromFile(std::fstream &fileIn);

private:
    // TODO: unsigned char *m_textureData;
    // TODO: unsigned int width;
    // TODO: unsigned int height;
    // TODO: format
};
}
