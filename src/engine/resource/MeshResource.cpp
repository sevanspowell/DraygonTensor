#include <assimp/Importer.hpp>

#include <string>

#include "engine/resource/MeshResource.h"
#include "math/Quaternion.h"

namespace ds
{
std::unique_ptr<IResource> MeshResource::CreateFromFile(std::string filePath)
{
    std::unique_ptr<MeshResource> meshResource =
        std::unique_ptr<MeshResource>(new MeshResource());

    // Load scene
    const aiScene *ourScene = meshResource->GetImporter().ReadFile(
        filePath.c_str(), aiProcess_Triangulate);
    meshResource->SetScene(ourScene);

    // If mesh failed to load
    if (!meshResource->GetScene())
    {
        // Destroy mesh resource
        meshResource = nullptr;
    }
    // If mesh loaded successfully
    else
    {
        int meshCount = ourScene->mNumMeshes;
        meshResource->SetMeshCount(meshCount);


        // Bone information
        std::vector<VertexBoneData> bones;
        unsigned int numVertices = 0;

        // For each mesh
        // TODO: Fix for multiple meshes in scene
        for (unsigned int iMesh = 0; iMesh < 1; iMesh++)
        {
            // Load mesh data
            meshResource->StoreMeshPositions(iMesh, ourScene->mMeshes[iMesh]);
            meshResource->StoreNormalCoords(iMesh, ourScene->mMeshes[iMesh]);
            meshResource->StoreTextureCoords(iMesh, ourScene->mMeshes[iMesh]);
            meshResource->StoreFaces(iMesh, ourScene->mMeshes[iMesh]);

            numVertices += ourScene->mMeshes[iMesh]->mNumVertices;
        }

        // TODO: Fix for multiple meshes in scene
        for (unsigned int iMesh = 0; iMesh < 1; ++iMesh)
        {
            // Reserve enough memory for the bone data of each vertex
            bones.resize(numVertices);
            meshResource->LoadBones(iMesh, ourScene->mMeshes[iMesh], &bones);
        }

        meshResource->SetVertexBoneData(bones);

        // Get global transform
        aiMatrix4x4 transform = ourScene->mRootNode->mTransformation;
        // Invert it
        transform.Inverse();
        // Convert it to our matrix 4x4 representation
        ds_math::Matrix4 mat(
            transform.a1, transform.a2, transform.a3, transform.a4,
            transform.b1, transform.b2, transform.b3, transform.b4,
            transform.c1, transform.c2, transform.c3, transform.c4,
            transform.d1, transform.d2, transform.d3, transform.d4);
        // Store global inverse transform of root node of the scene
        meshResource->SetGlobalInverseTransform(mat);
    }

    return std::unique_ptr<IResource>(
        static_cast<MeshResource *>(meshResource.release()));
}

MeshResource::MeshResource()
{
    m_scene = nullptr;
    m_numBones = 0;
}

MeshResource::~MeshResource()
{
    for (int iMesh = 0; iMesh < m_meshCollection.size(); iMesh++)
    {
        m_meshCollection[iMesh].m_vertices.clear();
        m_meshCollection[iMesh].m_texCoords.clear();
        m_meshCollection[iMesh].m_normals.clear();
        m_meshCollection[iMesh].m_indices.clear();
    }

    m_meshCollection.clear();

    // Free the scene
    m_importer.FreeScene();
}

const std::string &MeshResource::GetResourceFilePath() const
{
    return m_filePath;
}

void MeshResource::SetResourceFilePath(const std::string &filePath)
{
    m_filePath = filePath;
}

void MeshResource::StoreFaces(unsigned int meshNumber, aiMesh *singleMesh)
{
    if (singleMesh->HasFaces())
    {
        int numOfFaces = singleMesh->mNumFaces;
        m_meshCollection[meshNumber].m_indices.reserve(numOfFaces * 3);

        for (int iFaces = 0; iFaces < numOfFaces; iFaces++)
        {
            unsigned int firstInd;
            unsigned int secondInd;
            unsigned int thirdInd;

            firstInd = singleMesh->mFaces[iFaces].mIndices[0];
            secondInd = singleMesh->mFaces[iFaces].mIndices[1];
            thirdInd = singleMesh->mFaces[iFaces].mIndices[2];

            m_meshCollection[meshNumber].m_indices.push_back(firstInd);
            m_meshCollection[meshNumber].m_indices.push_back(secondInd);
            m_meshCollection[meshNumber].m_indices.push_back(thirdInd);
        }
    }
}


void MeshResource::StoreMeshPositions(unsigned int meshNumber,
                                      aiMesh *singleMesh)
{
    if (singleMesh->HasPositions())
    {

        int numberOfVerts = singleMesh->mNumVertices;
        m_meshCollection[meshNumber].m_vertices.reserve(numberOfVerts);

        for (int iVert = 0; iVert < numberOfVerts; iVert++)
        {
            ds_math::Vector3 singlePos;
            singlePos.x = singleMesh->mVertices[iVert].x;
            singlePos.y = singleMesh->mVertices[iVert].y;
            singlePos.z = singleMesh->mVertices[iVert].z;
            m_meshCollection[meshNumber].m_vertices.push_back(singlePos);
        }
    }
}


void MeshResource::StoreTextureCoords(unsigned int meshNumber,
                                      aiMesh *singleMesh)
{

    int textExistIdxIndicator = 0;

    if (singleMesh->HasTextureCoords(textExistIdxIndicator))
    {

        int numberOfVerts = singleMesh->mNumVertices;
        m_meshCollection[meshNumber].m_texCoords.reserve(numberOfVerts);

        for (int iTex = 0; iTex < numberOfVerts; iTex++)
        {
            ds_math::Vector3 singleTexCoord;
            singleTexCoord.x = singleMesh->mTextureCoords[0][iTex].x;
            singleTexCoord.y = singleMesh->mTextureCoords[0][iTex].y;
            singleTexCoord.z = 0.0f;
            m_meshCollection[meshNumber].m_texCoords.push_back(singleTexCoord);
        }
    }
}


void MeshResource::StoreNormalCoords(unsigned int meshNumber,
                                     aiMesh *singleMesh)
{
    if (singleMesh->HasNormals())
    {
        int numberOfVerts = singleMesh->mNumVertices;
        m_meshCollection[meshNumber].m_normals.resize(numberOfVerts);

        for (int iNorms = 0; iNorms < numberOfVerts; iNorms++)
        {
            ds_math::Vector3 singleNormCoords;
            singleNormCoords.x = singleMesh->mNormals[iNorms].x;
            singleNormCoords.y = singleMesh->mNormals[iNorms].y;
            singleNormCoords.z = singleMesh->mNormals[iNorms].z;
            m_meshCollection[meshNumber].m_normals.push_back(singleNormCoords);
        }
    }
}


unsigned int MeshResource::GetVertCount(unsigned int meshNumber) const
{
    unsigned int verts = 0;

    if (m_meshCollection.size() >= meshNumber)
    {
        verts = m_meshCollection[meshNumber].m_vertices.size();
    }

    return verts;
}


unsigned int MeshResource::GetTexCoordCount(unsigned int meshNumber) const
{
    unsigned int texCoords = 0;

    if (m_meshCollection.size() >= meshNumber)
    {
        texCoords = m_meshCollection[meshNumber].m_texCoords.size();
    }

    return texCoords;
}


unsigned int MeshResource::GetNormalsCount(unsigned int meshNumber) const
{
    unsigned int norms = 0;

    if (m_meshCollection.size() >= meshNumber)
    {
        norms = m_meshCollection[meshNumber].m_normals.size();
    }

    return norms;
}


unsigned int MeshResource::GetIndicesCount(unsigned int meshNumber) const
{
    unsigned int indcs = 0;

    if (m_meshCollection.size() >= meshNumber)
    {
        indcs = m_meshCollection[meshNumber].m_indices.size();
    }

    return indcs;
}


bool MeshResource::HasSingleMesh() const
{
    bool flag = false;

    if (m_meshCollection.size() == 1)
    {
        flag = true;
    }

    return flag;
}


bool MeshResource::HasMultiMesh() const
{
    bool flag = false;

    if (m_meshCollection.size() > 1)
    {
        flag = true;
    }

    return flag;
}


unsigned int MeshResource::GetVertCount() const
{
    return m_meshCollection[0].m_vertices.size();
}


unsigned int MeshResource::GetTexCoordCount() const
{
    return m_meshCollection[0].m_texCoords.size();
}


unsigned int MeshResource::GetNormalsCount() const
{
    return m_meshCollection[0].m_normals.size();
}


unsigned int MeshResource::GetIndicesCount() const
{
    return m_meshCollection[0].m_indices.size();
}


unsigned int MeshResource::GetMeshCount() const
{
    return m_meshCollection.size();
    ;
}


std::vector<ds_math::Vector3> MeshResource::GetVerts() const
{
    return m_meshCollection[0].m_vertices;
}


std::vector<ds_math::Vector3> MeshResource::GetTexCoords() const
{
    return m_meshCollection[0].m_texCoords;
}


std::vector<ds_math::Vector3> MeshResource::GetNormals() const
{
    return m_meshCollection[0].m_normals;
}


std::vector<unsigned int> MeshResource::GetIndices() const
{
    return m_meshCollection[0].m_indices;
}


std::vector<ds_math::Vector3>
MeshResource::GetVerts(unsigned int meshNumber) const
{
    return m_meshCollection[meshNumber].m_vertices;
}


std::vector<ds_math::Vector3>
MeshResource::GetTexCoords(unsigned int meshNumber) const
{
    return m_meshCollection[meshNumber].m_texCoords;
}


std::vector<ds_math::Vector3>
MeshResource::GetNormals(unsigned int meshNumber) const
{
    return m_meshCollection[meshNumber].m_normals;
}

std::vector<unsigned int>
MeshResource::GetIndices(unsigned int meshNumber) const
{
    return m_meshCollection[meshNumber].m_indices;
}

Assimp::Importer &MeshResource::GetImporter()
{
    return m_importer;
}

const aiScene *MeshResource::GetScene() const
{
    return m_scene;
}

void MeshResource::SetScene(const aiScene *scene)
{
    m_scene = scene;
}

void MeshResource::SetMeshCount(unsigned int meshCount)
{
    m_meshCollection.resize(meshCount);
}

const ds_math::Matrix4 &MeshResource::GetGlobalInverseTransform() const
{
    return m_globalInverseTransform;
}

void MeshResource::SetGlobalInverseTransform(
    const ds_math::Matrix4 &globalInvTransform)
{
    m_globalInverseTransform = globalInvTransform;
}

// unsigned int MeshResource::GetBoneCount() const
// {
//     return m_bones.size();
// }

// void MeshResource::SetBoneCount(unsigned int boneCount)
// {
//     m_bones.resize(boneCount);
// }

void MeshResource::LoadBones(unsigned int meshIndex,
                             const aiMesh *mesh,
                             std::vector<VertexBoneData> *bones)
{
    if (bones != nullptr)
    {
        for (unsigned int i = 0; i < mesh->mNumBones; ++i)
        {
            unsigned int boneIndex = 0;
            std::string boneName = std::string(mesh->mBones[i]->mName.data);

            if (m_boneMapping.find(boneName) == m_boneMapping.end())
            {
                boneIndex = m_numBones;
                ++m_numBones;
                BoneInfo bi;
                m_boneInfo.push_back(bi);
                aiMatrix4x4 temp = mesh->mBones[i]->mOffsetMatrix;
                m_boneInfo[boneIndex].boneOffset = ds_math::Matrix4(
                    temp.a1, temp.a2, temp.a3, temp.a4, temp.b1, temp.b2,
                    temp.b3, temp.b4, temp.c1, temp.c2, temp.c3, temp.c4,
                    temp.d1, temp.d2, temp.d3, temp.d4);
                m_boneMapping[boneName] = boneIndex;
            }
            else
            {
                boneIndex = m_boneMapping[boneName];
            }

            for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; ++j)
            {
                // TODO: Fix for multiple meshes in scene
                unsigned int vertexId = mesh->mBones[i]->mWeights[j].mVertexId;
                float weight = mesh->mBones[i]->mWeights[j].mWeight;
                (*bones)[vertexId].AddBoneData(boneIndex, weight);
            }
        }
    }
}

void MeshResource::BoneTransform(float timeInSeconds,
                                 std::vector<ds_math::Matrix4> *transforms)
{
    if (transforms != nullptr)
    {
        ds_math::Matrix4 identity = ds_math::Matrix4(1.0f);

        // TODO: Fix for multiple animations
        float ticksPerSecond = m_scene->mAnimations[0]->mTicksPerSecond != 0
                                   ? m_scene->mAnimations[0]->mTicksPerSecond
                                   : 25.0f;
        float timeInTicks = timeInSeconds * ticksPerSecond;
        float animationTime =
            fmod(timeInTicks, m_scene->mAnimations[0]->mDuration);

        ReadNodeHeirarchy(animationTime, m_scene->mRootNode, identity);

        transforms->resize(m_numBones);

        for (unsigned int i = 0; i < m_numBones; ++i)
        {
            (*transforms)[i] = m_boneInfo[i].finalTransform;
        }
    }
}

void MeshResource::ReadNodeHeirarchy(float animationTime,
                                     const aiNode *node,
                                     const ds_math::Matrix4 &parentTransform)
{
    std::string nodeName = std::string(node->mName.data);

    // TODO: Fix for multiple animations
    const aiAnimation *animation = m_scene->mAnimations[0];

    aiMatrix4x4 temp = node->mTransformation;
    ds_math::Matrix4 nodeTransformation = ds_math::Matrix4(
        temp.a1, temp.a2, temp.a3, temp.a4, temp.b1, temp.b2, temp.b3, temp.b4,
        temp.c1, temp.c2, temp.c3, temp.c4, temp.d1, temp.d2, temp.d3, temp.d4);

    const aiNodeAnim *nodeAnim = FindNodeAnim(animation, nodeName);

    if (nodeAnim != nullptr)
    {
        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D scaling;
        scaling = CalcInterpolatedScaling(animationTime, nodeAnim);
        ds_math::Matrix4 scalingM = ds_math::Matrix4::CreateScaleMatrix(
            scaling.x, scaling.y, scaling.z);

        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion rotation;
        rotation = CalcInterpolatedRotation(animationTime, nodeAnim);
        ds_math::Matrix4 rotationM =
            ds_math::Matrix4::CreateFromQuaternion(ds_math::Quaternion(
                rotation.x, rotation.y, rotation.z, rotation.w));

        // Interpolate translation and generate translation transformation
        // matrix
        aiVector3D translation;
        translation = CalcInterpolatedPosition(animationTime, nodeAnim);
        ds_math::Matrix4 translationM =
            ds_math::Matrix4::CreateTranslationMatrix(
                translation.x, translation.y, translation.z);

        // Combine the transformations
        nodeTransformation = translationM * rotationM * scalingM;
    }

    ds_math::Matrix4 globalTransformation =
        parentTransform * nodeTransformation;

    if (m_boneMapping.find(nodeName) != m_boneMapping.end())
    {
        unsigned int boneIndex = m_boneMapping[nodeName];
        m_boneInfo[boneIndex].finalTransform = m_globalInverseTransform *
                                               globalTransformation *
                                               m_boneInfo[boneIndex].boneOffset;
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        ReadNodeHeirarchy(animationTime, node->mChildren[i],
                          globalTransformation);
    }
}

aiVector3D MeshResource::CalcInterpolatedScaling(float animationTime,
                                                 const aiNodeAnim *nodeAnim)
{
    aiVector3D interpolatedScale = aiVector3D(1.0f, 1.0f, 1.0f);

    // Keys are the key frames
    // We need at least two to interpolate
    if (nodeAnim->mNumScalingKeys < 2)
    {
        // TODO: Fix for multiple animations
        interpolatedScale = nodeAnim->mScalingKeys[0].mValue;
    }
    else
    {
        // Get key frame index before this point in the animation
        unsigned int scalingIndex = FindScaling(animationTime, nodeAnim);
        // Get the key frame index after this point in the animation
        unsigned int nextScalingIndex = (scalingIndex + 1);
        // Ensure key frame index isn't out of bounds
        assert(nextScalingIndex < nodeAnim->mNumScalingKeys);
        float deltaTime =
            (float)(nodeAnim->mScalingKeys[nextScalingIndex].mTime -
                    nodeAnim->mScalingKeys[scalingIndex].mTime);
        float factor = (animationTime -
                        (float)nodeAnim->mScalingKeys[scalingIndex].mTime) /
                       deltaTime;
        assert(factor >= 0.0f && factor <= 1.0f);
        const aiVector3D &start = nodeAnim->mScalingKeys[scalingIndex].mValue;
        const aiVector3D &end = nodeAnim->mScalingKeys[nextScalingIndex].mValue;
        aiVector3D delta = end - start;

        // Linear interpolation
        interpolatedScale = start + (factor * delta);
    }

    return interpolatedScale;
}

aiQuaternion MeshResource::CalcInterpolatedRotation(float animationTime,
                                                    const aiNodeAnim *nodeAnim)
{
    aiQuaternion interpolatedRotation = aiQuaternion();

    // Keys are the key frames
    // We need at least two to interpolate
    if (nodeAnim->mNumRotationKeys < 2)
    {
        // TODO: Fix for multiple animations
        interpolatedRotation = nodeAnim->mRotationKeys[0].mValue;
    }
    else
    {
        // Get key frame index before this point in the animation
        unsigned int rotationIndex = FindRotation(animationTime, nodeAnim);
        // Get the key frame index after this point in the animation
        unsigned int nextRotationIndex = rotationIndex + 1;
        // Ensure key frame index isn't out of bounds
        assert(nextRotationIndex < nodeAnim->mNumRotationKeys);

        // Time difference between the two animation key frames
        float deltaTime =
            (float)(nodeAnim->mRotationKeys[nextRotationIndex].mTime -
                    nodeAnim->mRotationKeys[rotationIndex].mTime);
        // How far between two frames the current animation time is
        float factor = (animationTime -
                        (float)nodeAnim->mRotationKeys[rotationIndex].mTime) /
                       deltaTime;
        // Ensure between 0 and 1 - sanity check
        assert(factor >= 0.0f && factor <= 1.0f);

        // Rotation at key frame before current animation time
        const aiQuaternion &startRotation =
            nodeAnim->mRotationKeys[rotationIndex].mValue;
        // Rotation at key frame after current animation time
        const aiQuaternion &endRotation =
            nodeAnim->mRotationKeys[nextRotationIndex].mValue;
        // Interpolate between the two key frame rotations
        aiQuaternion::Interpolate(interpolatedRotation, startRotation,
                                  endRotation, factor);

        interpolatedRotation.Normalize();
    }

    return interpolatedRotation;
}

aiVector3D MeshResource::CalcInterpolatedPosition(float animationTime,
                                                  const aiNodeAnim *nodeAnim)
{
    aiVector3D interpolatedPosition;

    // Keys are the key frames
    // We need at least two to interpolate
    if (nodeAnim->mNumRotationKeys < 2)
    {
        // TODO: Fix for multiple animations
        interpolatedPosition = nodeAnim->mPositionKeys[0].mValue;
    }
    else
    {
        // Get key frame index before this point in the animation
        unsigned int positionIndex = FindPosition(animationTime, nodeAnim);
        // Get the key frame index after this point in the animation
        unsigned int nextPositionIndex = positionIndex + 1;
        // Ensure key frame index isn't out of bounds
        assert(nextPositionIndex < nodeAnim->mNumPositionKeys);

        // Time difference between the two animation key frames
        float deltaTime =
            (float)(nodeAnim->mPositionKeys[nextPositionIndex].mTime -
                    nodeAnim->mPositionKeys[positionIndex].mTime);
        // How far between two frames the current animation time is
        float factor = (animationTime -
                        (float)nodeAnim->mPositionKeys[positionIndex].mTime) /
                       deltaTime;
        // Ensure between 0 and 1 - sanity check
        assert(factor >= 0.0f && factor <= 1.0f);

        // Position at key frame before current animation time
        const aiVector3D &startPosition =
            nodeAnim->mPositionKeys[positionIndex].mValue;
        // Position at key frame after current animation time
        const aiVector3D &endPosition =
            nodeAnim->mPositionKeys[nextPositionIndex].mValue;

        // Change in position
        aiVector3D delta = endPosition - startPosition;
        // Linearly interpolate between start and end position
        interpolatedPosition = startPosition + factor * delta;
    }

    return interpolatedPosition;
}

unsigned int MeshResource::FindScaling(float animationTime,
                                       const aiNodeAnim *nodeAnim)
{
    // Ensure at least one scaling key - sanity check
    assert(nodeAnim->mNumScalingKeys > 0);

    // Find the animation key frame index immediately before the animation time
    // given
    for (unsigned int i = 0; i < nodeAnim->mNumScalingKeys - 1; ++i)
    {
        if (animationTime < (float)nodeAnim->mScalingKeys[i + 1].mTime)
        {
            return i;
        }
    }

    assert(false && "MeshResource::FindScaling: Animation time exceeds key "
                    "frames available.");

    return 0;
}

unsigned int MeshResource::FindRotation(float animationTime,
                                        const aiNodeAnim *nodeAnim)
{
    // Ensure at least one rotation key - sanity check
    assert(nodeAnim->mNumRotationKeys > 0);

    // Find the animation key frame index immediately before the animation time
    // given
    for (unsigned int i = 0; i < nodeAnim->mNumRotationKeys - 1; ++i)
    {
        if (animationTime < (float)nodeAnim->mRotationKeys[i + 1].mTime)
        {
            return i;
        }
    }

    assert(false && "MeshResource::FindRotation: Animation time exceeds key "
                    "frames available.");

    return 0;
}

unsigned int MeshResource::FindPosition(float animationTime,
                                        const aiNodeAnim *nodeAnim)
{
    // Ensure at least one position key - sanity check
    assert(nodeAnim->mNumPositionKeys > 0);

    // Find the animation key frame index immediately before the animation time
    // given
    for (unsigned int i = 0; i < nodeAnim->mNumPositionKeys - 1; ++i)
    {
        if (animationTime < (float)nodeAnim->mPositionKeys[i + 1].mTime)
        {
            return i;
        }
    }

    assert(false && "MeshResource::FindPosition: Animation time exceeds key "
                    "frames available.");

    return 0;
}

const aiNodeAnim *MeshResource::FindNodeAnim(const aiAnimation *animation,
                                             const std::string &nodeName)
{
    const aiNodeAnim *anim = nullptr;

    if (animation != nullptr)
    {
        for (unsigned int i = 0; i < animation->mNumChannels; ++i)
        {
            const aiNodeAnim *nodeAnim = animation->mChannels[i];

            if (std::string(nodeAnim->mNodeName.data) == nodeName)
            {
                anim = nodeAnim;
                break;
            }
        }
    }

    return anim;
}

const std::vector<MeshResource::VertexBoneData> &
MeshResource::GetVertexBoneData() const
{
    return m_vertexBoneData;
}

void MeshResource::SetVertexBoneData(
    const std::vector<MeshResource::VertexBoneData> &vertexBoneData)
{
    m_vertexBoneData = vertexBoneData;
}
}
