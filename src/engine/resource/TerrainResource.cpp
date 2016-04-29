#include "engine/resource/TerrainResource.h"

namespace ds
{

void TerrainResource::SetResourceFilePath(const std::string &filePath)
{
    m_filePath = filePath;
}

const std::string &TerrainResource::GetResourceFilePath() const
{
    return m_filePath;
}

std::unique_ptr<IResource> TerrainResource::CreateFromFile(std::string filePath)
{

    ResourceFactory factory;

    // register texture resource creature
    factory.RegisterCreator<TextureResource>(TextureResource::CreateFromFile);

    // uses filepath to get a pointer to a terrain resource which contains the
    // heightmap image
    std::unique_ptr<TextureResource> changedResourcePointer =
        factory.CreateResource<TextureResource>((filePath));

    // create a new terrain resource
    std::unique_ptr<TerrainResource> createdTerrainResource(
        new TerrainResource());

    // set base height of the terrain - must be above 0
    // float normalX, normalZ

    unsigned int width = changedResourcePointer->GetWidthInPixels();
    unsigned int height = changedResourcePointer->GetHeightInPixels();

    std::vector<float> heights;
    heights.resize(width * height, 0.0f);

    std::vector<std::vector<float>> tempPixelHeights;
    tempPixelHeights.resize(height, std::vector<float>(width, 0.0));

    // iterate through height and width to use each pixel
    // fill heights vector to be used for determining normals in next set of for
    // loops
    for (int z = 0; z < height; z++)
    {
        for (int x = 0; x < width; x++)
        {
            // get the color of the current pixel to be used to determine height
            // (lighter = higher)
            unsigned char color =
                (unsigned char)changedResourcePointer
                    ->GetTextureContents()[3 * (z * (int)width + x)];

            // calculate the height of the point using the color of the
            // associated pixel
            // -0.5f to get each height within range -0.5 to 0.5
            float h = 20 * ((color / 255.0f) - 0.5f);
            heights[z * width + x] = h;

            // std::cout << h << std::endl;
            // HEIGHTS
            // put heights in tempPixelHeights 2d vector from BL to TR pixel
            // tempPixelHeights[x][z] = h;

            // VERTICES
            // put each vector3 into the vector m_vertices
            createdTerrainResource->PushVector3ToVertices(
                ds_math::Vector3(x, h, z));

            // swapping some stuff
            // createdTerrainResource->PushVector3ToVertices(ds_math::Vector3(h,z,x));

            // push pixel heights to the 2d vector
            // createdTerrainResource->Copy2DVectorOfHeights(tempPixelHeights);
        }
    }


    // INDICES
    int currentSquare, squareX, squareZ, tl, tr, bl, br; //
    int squareAmount = (width - 1) * (height - 1);

    for (currentSquare = 0; currentSquare < squareAmount; currentSquare++)
    {
        // calculate current square position
        squareX = currentSquare % (width - 1);
        squareZ = (currentSquare - squareX) / (width - 1);

        // calcluate indices of each square corner
        tl = (squareZ * width) + squareX;
        tr = (squareZ * width) + squareX + 1;
        bl = ((squareZ + 1) * width) + squareX;
        br = ((squareZ + 1) * width) + (squareX + 1);

        // 1st triangle
        createdTerrainResource->PushIntegerToIndices(bl);
        createdTerrainResource->PushIntegerToIndices(tr);
        createdTerrainResource->PushIntegerToIndices(tl);

        // 2nd triangle
        createdTerrainResource->PushIntegerToIndices(bl);
        createdTerrainResource->PushIntegerToIndices(br);
        createdTerrainResource->PushIntegerToIndices(tr);
    }

    // set width and depth in terrain struct
    createdTerrainResource->SetWidthDepth(width, height);

    // NORMALS
    // iterate through height and width to use each pixel
    // fill vertices and normals vectors
    createdTerrainResource->CalculateNormals();

    // TEX COORDS
    // fill tex coords vector
    createdTerrainResource->CalculateTextureCoordinates();

    // Set height array
    createdTerrainResource->SetHeightArray(heights);

    std::unique_ptr<IResource> TerrainResource =
        std::move(createdTerrainResource);

    return TerrainResource;
}


TerrainResource::TerrainResource()
{
}


TerrainResource::~TerrainResource()
{
    m_terrain.m_vertices.clear();
    m_terrain.m_normals.clear();
    m_terrain.m_indices.clear();
    m_terrain.m_textureCoordinates.clear();
}

void TerrainResource::PrintTest()
{
    // m_terrain.m_vertices.size();
    // for(int i = 0; i < m_terrain.m_vertices.size(); i++)
    // {
    // 	std::cout << "indices for point " << i << " : " <<
    // m_terrain.m_indices[i*3] << ", " << m_terrain.m_indices[i*3+1] << ", " <<
    // m_terrain.m_indices[i*3+2] << std::endl;

    // }
}


void TerrainResource::PushVector3ToVertices(ds_math::Vector3 vec3)
{
    m_terrain.m_vertices.push_back(vec3);
}

void TerrainResource::PushVector3ToNormals(ds_math::Vector3 vec3)
{
    m_terrain.m_normals.push_back(vec3);
}

void TerrainResource::PushIntegerToIndices(int x)
{
    m_terrain.m_indices.push_back(x);
}

void TerrainResource::Copy2DVectorOfHeights(
    std::vector<std::vector<float>> tempPixelHeights)
{
    m_terrain.m_pixelHeights = tempPixelHeights;
}

void TerrainResource::CalculateNormals()
{
    // NORMALS
    // 3 points of a triangle
    ds_math::Vector3 p1, p2, p3;
    // 3 vec3s for sideA and sideB of triangle and surface normal of triangle
    ds_math::Vector3 vecA, vecB, vecN;

    for (int k = 0; k < m_terrain.m_indices.size(); k += 3)
    {
        p1 = m_terrain.m_vertices[m_terrain.m_indices[k]];
        p2 = m_terrain.m_vertices[m_terrain.m_indices[k + 1]];
        p3 = m_terrain.m_vertices[m_terrain.m_indices[k + 2]];

        vecA = p2 - p1;
        vecB = p3 - p1;

        vecN = ds_math::Vector3((vecA.y * vecB.z) - (vecA.z * vecB.y),
                                (vecA.z * vecB.x) - (vecA.x * vecB.z),
                                (vecA.x * vecB.y) - (vecA.y * vecB.x));

        // terrible gross hacky yuck-ness to be able to normalize and test - fix
        // before final submission
        vecN = vecN + ds_math::Vector3(1, 1, 1);

        vecN.Normalize();

        // std::cout << "normalized vecN : " << vecN << std::endl;

        m_terrain.m_normals.push_back(vecN);
    }
}

void TerrainResource::CalculateTextureCoordinates()
{
    int incrementCount, i, j, uCount = 0, vCount = 0;
    float incrementValue, tempU = 0.0f, tempV = 1.0f;

    incrementValue = 1 / m_terrain.m_terrainWidth;
    incrementCount = m_terrain.m_terrainWidth / 1;

    struct TextureCoordinates texcoord;

    // loop through entire heightmap and calculator the u and v tex coords for
    // each vertex
    for (j = 0; j < m_terrain.m_terrainDepth; j++)
    {
        for (i = 0; i < m_terrain.m_terrainWidth; i++)
        {
            // Store the texture coordinate in the height map.
            // m_heightMap[(m_terrain.terrainDepth * j) + i].tu = tempU;
            // m_heightMap[(m_terrain.terrainDepth * j) + i].tv = tempV;

            tempU = (float)j / m_terrain.m_terrainDepth;
            tempV = (float)i / m_terrain.m_terrainWidth;

            // Rotate texture co-ordinates 45degrees (pi / 2.0 radians)
            texcoord.u = tempV;
            texcoord.v = -tempU;


            // Increment the tu texture coordinate by the increment value and
            // increment the index by one.
            tempU += incrementValue;
            uCount++;

            // Check if at the far right end of the texture and if so then start
            // at the beginning again.
            if (uCount == incrementCount)
            {
                tempU = 0.0f;
                uCount = 0;
            }

            m_terrain.m_textureCoordinates.push_back(texcoord);

            // std::cout << "u " << j << ", " << i << ": " << tempU << ", " <<
            // tempV << std::endl;
        }

        // Increment the tv texture coordinate by the increment value and
        // increment the index by one.
        tempV -= incrementValue;
        vCount++;

        // Check if at the top of the texture and if so then start at the bottom
        // again.
        if (vCount == incrementCount)
        {
            tempV = 1.0f;
            vCount = 0;
        }
    }
}

// GETTERS
std::vector<ds_math::Vector3> TerrainResource::GetVerticesVector()
{

    return m_terrain.m_vertices;
}

std::vector<ds_math::Vector3> TerrainResource::GetNormalsVector()
{

    return m_terrain.m_normals;
}

std::vector<struct TerrainResource::TextureCoordinates>
TerrainResource::GetTextureCoordinatesVector()
{

    return m_terrain.m_textureCoordinates;
}

std::vector<int> TerrainResource::GetIndicesVector()
{

    return m_terrain.m_indices;
}

std::vector<std::vector<float>> TerrainResource::GetPixelHeightsVector()
{

    return m_terrain.m_pixelHeights;
}

int TerrainResource::GetVerticesCount()
{

    return m_terrain.m_vertices.size();
}

int TerrainResource::GetIndicesCount()
{

    return m_terrain.m_indices.size();
}

int TerrainResource::GetNormalsCount()
{

    return m_terrain.m_normals.size();
}

int TerrainResource::GetTextureCoordinatesCount()
{

    return m_terrain.m_textureCoordinates.size();
}

void TerrainResource::SetWidthDepth(unsigned int w, unsigned int d)
{
    m_terrain.m_terrainWidth = w;
    m_terrain.m_terrainDepth = d;
}

const std::vector<float> &TerrainResource::GetHeightArray() const
{
    return m_heights;
}

unsigned int TerrainResource::GetHeightmapWidth() const
{
    return m_terrain.m_terrainWidth;
}

unsigned int TerrainResource::GetHeightmapHeight() const
{
    return m_terrain.m_terrainDepth;
}

void TerrainResource::SetHeightArray(const std::vector<float> &heightArray)
{
    m_heights = heightArray;
}
}
