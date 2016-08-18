#include <cassert>

#include "engine/system/scene/TransformComponentManager.h"

namespace ds
{
Instance TransformComponentManager::CreateComponentForEntityFromConfig(
    TransformComponentManager *transformComponentManager,
    Entity entity,
    const Config &config)
{
    Instance instance = Instance::MakeInvalidInstance();

    if (transformComponentManager != nullptr)
    {
        std::vector<float> position;
        std::vector<float> orientation;
        std::vector<float> scale;

        if (config.GetFloatArray("position", &position) &&
            config.GetFloatArray("orientation", &orientation) &&
            config.GetFloatArray("scale", &scale))
        {
            // Does entity already have transform component?
            instance = transformComponentManager->GetInstanceForEntity(entity);

            // Only continue if it doesn't
            if (!instance.IsValid())
            {
                // Create component for entity
                instance =
                    transformComponentManager->CreateComponentForEntity(entity);

                // Setup component
                transformComponentManager->SetLocalTranslation(
                    instance,
                    ds_math::Vector3(position[0], position[1], position[2]));
                transformComponentManager->SetLocalOrientation(
                    instance,
                    ds_math::Quaternion(orientation[0], orientation[1],
                                        orientation[2], orientation[3]));
                transformComponentManager->SetLocalScale(
                    instance, ds_math::Vector3(scale[0], scale[1], scale[2]));
            }
        }
    }

    return instance;
}

ds_math::Matrix4 TransformComponentManager::GetLocalTransform(Instance i) const
{
    assert(i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
           "TransformComponentManager::GetLocalTransform tried to get invalid "
           "instance");

    return ds_math::Matrix4(ds_math::Matrix4::CreateTranslationMatrix(
                                m_data.component[i.index].localTranslation) *
                            ds_math::Matrix4::CreateFromQuaternion(
                                m_data.component[i.index].localOrientation) *
                            ds_math::Matrix4::CreateScaleMatrix(
                                m_data.component[i.index].localScale));
}

const ds_math::Vector3 &
TransformComponentManager::GetLocalTranslation(Instance i) const
{
    assert(
        i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
        "TransformComponentManager::GetLocalTranslation tried to get invalid "
        "instance");

    return m_data.component[i.index].localTranslation;
}

const ds_math::Vector3 &
TransformComponentManager::GetLocalScale(Instance i) const
{
    assert(
           i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
           "TransformComponentManager::GetLocalScale tried to get invalid "
           "instance");

    return m_data.component[i.index].localScale;
}

const ds_math::Quaternion &
TransformComponentManager::GetLocalOrientation(Instance i) const
{
    assert(
           i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
           "TransformComponentManager::GetLocalOrientation tried to get invalid "
           "instance");

    return m_data.component[i.index].localOrientation;
}

ds_math::Matrix4 TransformComponentManager::GetWorldTransform(Instance i) const
{
    assert(i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
           "TransformComponentManager::GetWorldTransform tried to get invalid "
           "instance");

    return ds_math::Matrix4(ds_math::Matrix4::CreateTranslationMatrix(
                                m_data.component[i.index].worldTranslation) *
                            ds_math::Matrix4::CreateFromQuaternion(
                                m_data.component[i.index].worldOrientation) *
                            ds_math::Matrix4::CreateScaleMatrix(
                                m_data.component[i.index].worldScale));
}


void TransformComponentManager::SetLocalTranslation(
    Instance i, const ds_math::Vector3 &translation)
{
    assert(i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
           "TransformComponentManager::SetLocalTranslation: tried to set "
           "invalid instance.");

    // Set local translation
    m_data.component[i.index].localTranslation = translation;

    // Get parent
    Instance parent = m_data.component[i.index].parent;
    // Get world transform of parent
    // ds_math::Matrix4 parentTransform =
    //     parent.IsValid() ? GetWorldTransform(parent) : ds_math::Matrix4();

    // Update instance i's world transform and all it's children
    // UpdateWorldTransform(i, parentTransform);

    // Get world translation of parent
    ds_math::Vector3 parentTranslation =
        parent.IsValid() ? m_data.component[parent.index].worldTranslation
                         : ds_math::Vector3(0.0f, 0.0f, 0.0f);

    // Update instance i's world translation and all it's children
    UpdateWorldTranslation(i, parentTranslation);
}

void TransformComponentManager::UpdateWorldTranslation(
    Instance i, const ds_math::Vector3 &parentTranslation)
{
    // Parent translation then local translation
    m_data.component[i.index].worldTranslation =
        m_data.component[i.index].localTranslation + parentTranslation;

    Instance child = m_data.component[i.index].firstChild;
    while (child.IsValid())
    {
        UpdateWorldTranslation(child,
                               m_data.component[i.index].worldTranslation);
        child = m_data.component[child.index].nextSibling;
    }
}

void TransformComponentManager::SetLocalScale(Instance i,
                                              const ds_math::Vector3 &scale)
{
    assert(i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
           "TransformComponentManager::SetLocalScale: tried to set "
           "invalid instance.");

    // Set local translation
    m_data.component[i.index].localScale = scale;

    // Get parent
    Instance parent = m_data.component[i.index].parent;
    // Get world transform of parent
    // ds_math::Matrix4 parentTransform =
    //     parent.IsValid() ? m_data.component[parent.index].worldTransform
    //                      : ds_math::Matrix4();

    // Update instance i's world transform and all it's children
    // UpdateWorldTransform(i, parentTransform);

    // Get world scale of parent
    ds_math::Vector3 parentScale =
        parent.IsValid() ? m_data.component[parent.index].worldScale
                         : ds_math::Vector3(1.0f, 1.0f, 1.0f);

    // Update instance i's world scale and all it's children
    UpdateWorldScale(i, parentScale);
}

void TransformComponentManager::UpdateWorldScale(
    Instance i, const ds_math::Vector3 &parentScale)
{
    // Parent scale then local scale
    m_data.component[i.index].worldScale =
        m_data.component[i.index].localScale * parentScale;

    Instance child = m_data.component[i.index].firstChild;
    while (child.IsValid())
    {
        UpdateWorldScale(child, m_data.component[i.index].worldScale);
        child = m_data.component[child.index].nextSibling;
    }
}

void TransformComponentManager::SetLocalOrientation(
    Instance i, const ds_math::Quaternion &orientation)
{
    assert(i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
           "TransformComponentManager::SetLocalOrientation: tried to set "
           "invalid instance.");

    // Set local translation
    m_data.component[i.index].localOrientation = orientation;

    // Get parent
    Instance parent = m_data.component[i.index].parent;
    // Get world transform of parent
    // ds_math::Matrix4 parentTransform =
    //     parent.IsValid() ? m_data.component[parent.index].worldTransform
    //                      : ds_math::Matrix4();

    // Update instance i's world transform and all it's children
    // UpdateWorldTransform(i, parentTransform);

    // Get world orientation of parent
    ds_math::Quaternion parentOrientation =
        parent.IsValid() ? m_data.component[parent.index].worldOrientation
                         : ds_math::Quaternion();

    // Update instance i's world quaternion and all it's children
    UpdateWorldOrientation(i, parentOrientation);
}

void TransformComponentManager::UpdateWorldOrientation(
    Instance i, const ds_math::Quaternion &parentOrientation)
{
    // Parent orientation then local orientation
    m_data.component[i.index].worldOrientation =
        m_data.component[i.index].localOrientation * parentOrientation;

    Instance child = m_data.component[i.index].firstChild;
    while (child.IsValid())
    {
        UpdateWorldOrientation(child,
                               m_data.component[i.index].worldOrientation);
        child = m_data.component[child.index].nextSibling;
    }
}

// void TransformComponentManager::SetLocalTransform(
//     Instance i, const ds_math::Matrix4 &matrix)
// {
//     assert(i.index >= 0 && i.index < GetNumInstances() &&
//            "TransformComponentManager::SetLocalTransform tried to set invalid
//            "
//            "instance");

//     // Set local transform
//     m_data.component[i.index].localTransform = matrix;

//     // Get parent
//     Instance parent = m_data.component[i.index].parent;
//     // Get world transform of parent
//     ds_math::Matrix4 parentTransform =
//         parent.IsValid() ? m_data.component[parent.index].worldTransform
//                          : ds_math::Matrix4();

//     // Update Instance i's world transform and all it's children
//     UpdateWorldTransform(i, parentTransform);
// }

const ds_math::Vector3 &
TransformComponentManager::GetWorldTranslation(Instance i) const
{
    assert(
        i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
        "TransformComponentManager::GetWorldTranslation: tried to get invalid "
        "instance");

    return m_data.component[i.index].worldTranslation;
}

const ds_math::Vector3 &
TransformComponentManager::GetWorldScale(Instance i) const
{
    assert(i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
           "TransformComponentManager::GetWorldScale: tried to get invalid "
           "instance");

    return m_data.component[i.index].worldScale;
}

const ds_math::Quaternion &
TransformComponentManager::GetWorldOrientation(Instance i) const
{
    assert(
        i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
        "TransformComponentManager::GetWorldOrientation: tried to get invalid "
        "instance");

    return m_data.component[i.index].worldOrientation;
}


const Instance &TransformComponentManager::GetParent(Instance i) const
{
    assert(
        i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
        "TransformComponentManager::GetParent tried to get invalid instance");

    return m_data.component[i.index].parent;
}

void TransformComponentManager::SetParent(Instance i, Instance parent)
{
    assert(
        i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
        "TransformComponentManager::SetParent tried to set invalid instance");

    // Set child's parent
    m_data.component[i.index].parent = parent;

    // Update child's local transform to be based off new parent -- is this
    // correct?
    m_data.component[i.index].localTranslation =
        m_data.component[i.index].worldTranslation -
        m_data.component[parent.index].worldTranslation;
    m_data.component[i.index].localOrientation =
        m_data.component[i.index].worldOrientation *
        ds_math::Quaternion::Invert(
            m_data.component[parent.index].worldOrientation);
    m_data.component[i.index].localScale =
        m_data.component[i.index].worldScale *
        ds_math::Vector3(1.0f / m_data.component[parent.index].worldScale.x,
                         1.0f / m_data.component[parent.index].worldScale.y,
                         1.0f / m_data.component[parent.index].worldScale.z);

    // Set parent's child
    // Is this first child of parent?
    Instance firstChild = m_data.component[parent.index].firstChild;
    if (firstChild.IsValid() == false)
    {
        m_data.component[parent.index].firstChild = i;
    }
    // Not the first child, therefore put in linked list of siblings
    else
    {
        // Loop thru iren until end of linked list
        Instance currentChild = firstChild;
        while (m_data.component[currentChild.index].nextSibling.IsValid())
        {
            currentChild = m_data.component[currentChild.index].nextSibling;
        }
        // Once at end of linked list, place child
        m_data.component[currentChild.index].nextSibling = i;
    }
}

const Instance &TransformComponentManager::GetFirstChild(Instance i) const
{
    assert(i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
           "TransformComponentManager::GetFirstChild tried to get invalid "
           "instance");

    return m_data.component[i.index].firstChild;
}

const Instance &TransformComponentManager::GetNextSibling(Instance i) const
{
    assert(i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
           "TransformComponentManager::GetNextSibling tried to get invalid "
           "instance");

    return m_data.component[i.index].nextSibling;
}

const Instance &TransformComponentManager::GetPrevSibling(Instance i) const
{
    assert(i.index >= 0 && (unsigned int)i.index < GetNumInstances() &&
           "TransformComponentManager::GetPrevSibling tried to get invalid "
           "instance");

    return m_data.component[i.index].prevSibling;
}

void TransformComponentManager::OnAddressChange(const Instance &oldAddress,
                                                const Instance &newAddress)
{
    // Loop thru all components
    for (unsigned int i = 0; i < GetNumInstances(); ++i)
    {
        // If any reference is referencing old instance address, update
        // it to new address.
        if (m_data.component[i].parent == oldAddress)
        {
            m_data.component[i].parent = newAddress;
        }
        if (m_data.component[i].firstChild == oldAddress)
        {
            // Case: first child of many siblings is removed
            // (address changed to -1)
            if (newAddress == -1)
            {
                // Move first child to next sibling
                m_data.component[i].firstChild =
                    m_data.component[oldAddress.index].nextSibling;
            }
            // Else just update address
            else
            {
                m_data.component[i].firstChild = newAddress;
            }
        }
        if (m_data.component[i].nextSibling == oldAddress)
        {
            // Case: child of many siblings is about to be removed.
            if (newAddress == -1)
            {
                m_data.component[i].nextSibling =
                    m_data.component[oldAddress.index].nextSibling;
            }
            else
            {
                m_data.component[i].nextSibling = newAddress;
            }
        }
        if (m_data.component[i].prevSibling == oldAddress)
        {
            // Case: child of many siblings is about to be removed.
            if (newAddress == -1)
            {
                m_data.component[i].prevSibling =
                    m_data.component[oldAddress.index].prevSibling;
            }
            else
            {
                m_data.component[i].prevSibling = newAddress;
            }
        }
    }
}

// void TransformComponentManager::UpdateWorldTransform(
//     Instance i, const ds_math::Matrix4 &parentTransform)
// {
//     // Parent transform then local transform
//     m_data.component[i.index].worldTransform =
//         m_data.component[i.index].localTransform * parentTransform;

//     Instance child = m_data.component[i.index].firstChild;
//     while (child.IsValid())
//     {
//         UpdateWorldTransform(child,
//         m_data.component[i.index].worldTransform);
//         child = m_data.component[child.index].nextSibling;
//     }
// }
}
