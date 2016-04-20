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

                // Transform position, rotation and scale into a single
                // matrix
                ds_math::Matrix4 mat =
                    ds_math::Matrix4::CreateTranslationMatrix(
                        position[0], position[1], position[2]) *
                    ds_math::Matrix4::CreateFromQuaternion(
                        ds_math::Quaternion(orientation[0], orientation[1],
                                            orientation[2], orientation[3])) *
                    ds_math::Matrix4::CreateScaleMatrix(scale[0], scale[1],
                                                        scale[2]);

                transformComponentManager->SetLocalTransform(instance, mat);
            }
        }
    }

    return instance;
}

const ds_math::Matrix4 &
TransformComponentManager::GetLocalTransform(Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "TransformComponentManager::GetLocalTransform tried to get invalid "
           "instance");

    return m_data.component[i.index].localTransform;
}

void TransformComponentManager::SetLocalTransform(
    Instance i, const ds_math::Matrix4 &matrix)
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "TransformComponentManager::SetLocalTransform tried to set invalid "
           "instance");

    // Set local transform
    m_data.component[i.index].localTransform = matrix;

    // Get parent
    Instance parent = m_data.component[i.index].parent;
    // Get world transform of parent
    ds_math::Matrix4 parentTransform =
        parent.IsValid() ? m_data.component[parent.index].worldTransform
                         : ds_math::Matrix4();

    // Update Instance i's world transform and all it's children
    UpdateWorldTransform(i, parentTransform);
}

const ds_math::Matrix4 &
TransformComponentManager::GetWorldTransform(Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "TransformComponentManager::GetWorldTransform tried to get invalid "
           "instance");

    return m_data.component[i.index].worldTransform;
}

const Instance &TransformComponentManager::GetParent(Instance i) const
{
    assert(
        i.index >= 0 && i.index < GetNumInstances() &&
        "TransformComponentManager::GetParent tried to get invalid instance");

    return m_data.component[i.index].parent;
}

void TransformComponentManager::SetParent(Instance i, Instance parent)
{
    assert(
        i.index >= 0 && i.index < GetNumInstances() &&
        "TransformComponentManager::SetParent tried to set invalid instance");

    // Set child's parent
    m_data.component[i.index].parent = parent;

    // Update child's local transform -- is this correct?
    m_data.component[i.index].localTransform =
        m_data.component[i.index].worldTransform *
        ds_math::Matrix4::Inverse(
            m_data.component[parent.index].worldTransform);

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
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "TransformComponentManager::GetFirstChild tried to get invalid "
           "instance");

    return m_data.component[i.index].firstChild;
}

const Instance &TransformComponentManager::GetNextSibling(Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
           "TransformComponentManager::GetNextSibling tried to get invalid "
           "instance");

    return m_data.component[i.index].nextSibling;
}

const Instance &TransformComponentManager::GetPrevSibling(Instance i) const
{
    assert(i.index >= 0 && i.index < GetNumInstances() &&
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

void TransformComponentManager::UpdateWorldTransform(
    Instance i, const ds_math::Matrix4 &parentTransform)
{
    // Parent transform then local transform
    m_data.component[i.index].worldTransform =
        m_data.component[i.index].localTransform * parentTransform;

    Instance child = m_data.component[i.index].firstChild;
    while (child.IsValid())
    {
        UpdateWorldTransform(child, m_data.component[i.index].worldTransform);
        child = m_data.component[child.index].nextSibling;
    }
}
}
