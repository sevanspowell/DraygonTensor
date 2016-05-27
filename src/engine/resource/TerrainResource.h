#pragma once

#include <memory>
#include <vector>
#include <string>

#include "engine/resource/ResourceFactory.h"
#include "engine/resource/TextureResource.h"
#include <math.h>

#include "math/Vector3.h"
#include "engine/resource/IResource.h"

namespace ds
{
/**
* A terrain resource contains all the information for terrain generated from a
* heightmap
* vertices, indicies, normals, width, depth and pixelheights.
*
* Terrain height data begins normalized between the range -0.5f to 0.5f.
*/
class TerrainResource : public IResource
{
public:
    /**
    * Set the file path to the resource.
    *
    * @param  filePath  const std::string &, file path of this resource.
    */
    virtual void SetResourceFilePath(const std::string &filePath);

    /**
    * Get the file path to the resource.
    *
    * @return  const std::string &, resource file path.
    */
    virtual const std::string &GetResourceFilePath() const;

    /**
    * Create a mesh resource from file.
    *
    * @param   filePath  std::string , file path to create mesh resource
    * from.
    * @return          std::unique_ptr<IResource>, pointer to mesh resource
    * created.
    */
    static std::unique_ptr<IResource> CreateFromFile(std::string filePath);

    /**
    * constructor
    */
    TerrainResource();

    /**
    * destructor
    */
    ~TerrainResource();

    /**
    * used to output to console for testing
    */
    void PrintTest();

    // GETTERS

    /**
    * Get the vertices vector
    *
    * @return  a vector containing Vector3
    */
    std::vector<ds_math::Vector3> GetVerticesVector();

    /**
    * Get the normals vector
    *
    * @return  a vector containing Vector3
    */
    std::vector<ds_math::Vector3> GetNormalsVector();


    /**
    * Get the indices vector
    *
    * @return  a vector containing int
    */
    std::vector<int> GetIndicesVector();

    /**
    * Get the 2d vector containing pixel heights
    * the index calls for each height correspond to their location on the
    * heightmap image
    *
    * @return  a 2d vector containing float
    */
    std::vector<std::vector<float>> GetPixelHeightsVector();

    /**
    * get the size of the vertices vector
    *
    *@return a int containing the size of the vector
    */
    int GetVerticesCount();

    /**
    * get the size of the indices vector
    *
    *@return a int containing the size of the vector
    */
    int GetIndicesCount();

    /**
    * get the size of the normals vector
    *
    *@return a int containing the size of the vector
    */
    int GetNormalsCount();

    /**
    * get the size of the normals vector
    *
    *@return a int containing the size of the vector
    */
    int GetTextureCoordinatesCount();

    /**
    * sets width and depth in the terrain struct
    *
    */
    void SetWidthDepth(unsigned int w, unsigned int d);

    /**
    *	represents a texture coordinate
    *
    */
    struct TextureCoordinates
    {
        float u = 0;
        float v = 1;
    };

    // /**
    // * Get the texture coordinates vector
    // *
    // * @return  a vector containing TextureCoordinates
    // */
    std::vector<struct TextureCoordinates> GetTextureCoordinatesVector();

    /**
     * Get the array of heights created from the heightmap.
     *
     * @return  const std::vector<float> &, array of heights created from the
     * heightmap.
     */
    const std::vector<float> &GetHeightArray() const;

    /**
     * Get the heightmap width.
     *
     * @return  unsigned int, width of the heightmap.
     */
    unsigned int GetHeightmapWidth() const;

    /**
     * Get the heightmap height.
     *
     * @return  unsigned int, height of heightmap.
     */
    unsigned int GetHeightmapHeight() const;

    /**
     * Set the height array of this terrain resource.
     *
     * Size of array must match GetHeightmapWidth() * GetHeightmapHeight().
     *
     * @param  heightArray  const std::vector<float> &, new height array.
     */
    void SetHeightArray(const std::vector<float> &heightArray);

    /**
     * Center the terrain vertices around the origin.
     *
     * @param  width   unsigned int, width of the heightmap.
     * @param  height  unsigned int, height of the heightmap.
     */
    void CenterVertices(unsigned int width, unsigned int height);

    /**
     * Set the scaling factor to be applied to all the height values of this
     * terrain resource.
     *
     * @param  heightScale  float, scaling factor.
     */
    void SetHeightScale(float heightScale);

    /**
     * Get the maximum height of the terrain.
     *
     * @return  float, maximum height of the terrain.
     */
    float GetMaxTerrainHeight() const;

    /**
     * Get the minimum height of the terrain.
     *
     * @return float, minimum height of the terrain.
     */
    float GetMinTerrainHeight() const;

    /**
    * Represents the terrain generated by a heightmap
    *
    */
    struct Terrain
    {
        // contains each vertice from Bot to top left to right starting at
        // bottom left
        std::vector<ds_math::Vector3> m_vertices;

        // vector of all indices for each triangle - 3 int = 1 triangle
        std::vector<int> m_indices;

        // contains the normals
        std::vector<ds_math::Vector3> m_normals;

        // terrain sizes width = width - depth = height or lenght whatever you
        // want to call it
        unsigned int m_terrainWidth, m_terrainDepth;

        // contains each pixel height in it's corresponding position
        std::vector<std::vector<float>> m_pixelHeights;

        // vector of texture coordinates - each coordinate contains a u and v
        std::vector<struct TextureCoordinates> m_textureCoordinates;
    };

private:
    /**
    * push a single Vector3 to the vertices vector
    *
    * @param	vec3		  	the vertice to push.
    */
    void PushVector3ToVertices(ds_math::Vector3 vec3);

    /**
    * push a single Vector3 to the normals vector
    *
    * @param	vec3		  	the vertice to push.
    */
    void PushVector3ToNormals(ds_math::Vector3 vec3);

    /**
    * push a single integer to the indices vector
    *
    * @param	x		  	the int to push.
    */
    void PushIntegerToIndices(int x);

    /**
    * copy a 2d vector into the 2d vector of heights for the terrain struct
    *
    * @param	tempPixelHeights		  	the 2d vector to copy.
    */
    void
    Copy2DVectorOfHeights(std::vector<std::vector<float>> tempPixelHeights);

    /*
    *  populates the normals vector with normalized surface normals.
    */
    void CalculateNormals();

    /*
    * populate the texture coordinate vector with UV
    */
    void CalculateTextureCoordinates();

    /*
     *  the file path to the resource
     */
    std::string m_filePath;

    /*
     *  a single terrain struct
     */
    struct Terrain m_terrain;

    /**
     * Array of height of each pixel in the terrain image
     * (of size image width * height)
     */
    std::vector<float> m_heights;

    /** Maximum terrain height */
    float m_maxTerrainHeight;
    /** Minimum terrain height */
    float m_minTerrainHeight;
};
}
