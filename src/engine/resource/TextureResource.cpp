#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#include "engine/resource/TextureResource.h"

namespace ds
{
std::unique_ptr<IResource> TextureResource::CreateFromFile(std::fstream &fileIn)
{
    std::unique_ptr<IResource> textureResource(nullptr);

    // TODO: Parse file
    int width, height, channels;
    // unsigned char *heightMap = SOIL_load_image("../assets/terrain.tga",
    // &width,
    //                                            &height, &channels,
    //                                            SOIL_LOAD_L);
    unsigned char *image =
        stbi_load("../assets/terrain.tga", &width, &height, &channels, 0);

    // TODO: Create texture resource

    // Return resource (if any)
    return textureResource;
}

// TODO: other methods (e.g. GetVertices...)
}
