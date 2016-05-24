#pragma once

#include "engine/Config.h"
#include "engine/entity/ComponentManager.h"
#include "engine/system/scene/TransformComponent.h"

namespace ds
{
/**
 * Transform Component Manager.
 *
 *  Stores the transforms of objects in the world.
 *  The transform component manager also forms the scenegraph of the world
 *  and so the transform component manager manages parent-child relations
 *  between objects.
 */
class TransformComponentManager : public ComponentManager<TransformComponent>
{
public:
    /**
     * Create a component for the given entity using a config file as a
     * description and return a component instance which can be used to refer to
     * that component.
     *
     * @param   transformComponentManager  TransformComponentManager *,
     * component manager to add component to.
     * @param   entity            Entity, entity to create component for. The
     * component will be associated with this entity.
     * @param   config            const Config &, config file containing
     * component description used to create component from.
     * @return                    Instance, the new component instance created
     * or an invalid instance in case of error.
     */
    static Instance CreateComponentForEntityFromConfig(
        TransformComponentManager *transformComponentManager,
        Entity entity,
        const Config &config);

    /**
     *  Get the transform (position, orientation, scale) matrix
     *  of an object relative to it's parent.
     *
     *  @param  i   Instance, component instance to get transform matrix of.
     *  @return     ds_math::Matrix4, local transform matrix.
     */
    ds_math::Matrix4 GetLocalTransform(Instance i) const;

    /**
     *  Set the transform of an object relative to it's parent. This
     *  method also updates the transforms of the object's children
     *  (if the object has children).
     *
     *  @param  i       Instance, component instance to set the transform
     *  matrix of.
     *  @param  matrix  const ds_math::Matrix4 &, new transform matrix.
     */
    // void SetLocalTransform(Instance i, const ds_math::Matrix4 &matrix);

    /**
     * Set the local translation of an object relative to it's parent. This
     * method also updates the transforms of the object's children (if the
     * object has children).
     *
     * @param  i            Instance, component instance to set the local
     *                      translation of.
     * @param  translation  const ds_math::Vector3 &, local translation to
     * set.
     */
    void SetLocalTranslation(Instance i, const ds_math::Vector3 &translation);

    /**
     * Set the local scale of an object relative to it's parent. The method also
     * updates the transforms of the object's children (if the object has
     * children).
     *
     * @param  i      Instance, component instance to set the local scale of.
     * @param  scale  const ds_math::Vector3 &, local scale to set.
     */
    void SetLocalScale(Instance i, const ds_math::Vector3 &scale);

    /**
     * Set the local orienatation of an object relative to it's parent. The
     * method also updates the transforms of the object's children (if the
     * object has any children).
     *
     * @param  i            Instance, component instance to set the local
     *                      orientation of.
     * @param  orientation  const ds_math::Quaternion &, local orientation to
     *                      set.
     */
    void SetLocalOrientation(Instance i,
                             const ds_math::Quaternion &orientation);

    /**
     * Get the local translation of the given component instance.
     *
     * @param   i  Instance, component instance to get the local translation of.
     * @return     const ds_math::Vector3 &, local translation of the component
     *             instance.
     */
    const ds_math::Vector3 &GetLocalTranslation(Instance i) const;

    /**
     * Get the local scale of the given component instance.
     *
     * @param   i  Instance, component instance to get the local scale of.
     * @return     const ds_math::Vector3 &, local scale of the component
     *             instance.
     */
    const ds_math::Vector3 &GetLocalScale(Instance i) const;

    /**
     * Get the local orientation of the given component instance.
     *
     * @param   i  Instance, component instance to get the local orientation of.
     * @return     const ds_math::Quaternion &, local orientation of the
     *             component instance.
     */
    const ds_math::Quaternion &GetLocalOrientation(Instance i) const;

    /**
     *  Get the world transform of the given component instance.
     *
     *  @param  i   Instance, component instance to get the world transform
     *  of.
     *  @return     ds_math::Matrix4, world transform of the component
     *  instance.
     */
    ds_math::Matrix4 GetWorldTransform(Instance i) const;

    /**
     * Get the world translation of the given component instance.
     *
     * @param   i  Instance, component instance to get the world translation of.
     * @return     const ds_math::Vector3 &, world translation of the component
     *             instance.
     */
    const ds_math::Vector3 &GetWorldTranslation(Instance i) const;

    /**
     * Get the world scale of the given component instance.
     *
     * @param   i  Instance, component instance to get the world scale of.
     * @return     const ds_math::Vector3 &, world scale of the component
     *             instance.
     */
    const ds_math::Vector3 &GetWorldScale(Instance i) const;

    /**
     * Get the world orientation of the given component instance.
     *
     * @param   i   Instance, component instance to get the world orientation
     *              of.
     * @return      const ds_math::Quaternion &, world orientation of the
     *              component instance.
     */
    const ds_math::Quaternion &GetWorldOrientation(Instance i) const;

    /**
     *  Get the component instance of the parent of the given component
     *  instance.
     *
     *  Returns an invalid instance if no parent.
     *
     *  @param  i   Instance, component instance to get parent of.
     *  @return     const Instance &, component instance of parent.
     */
    const Instance &GetParent(Instance i) const;

    /**
     *  Set the parent component instance of the given component instance.
     *  This method will set up all necessary sibling references.
     *
     *  To remove a parent, set parent to an invalid instance.
     *
     *  @param  i       Instance, component instance to set parent of.
     *  @param  parent  Instance, component instance of parent.
     */
    void SetParent(Instance i, Instance parent);

    /**
     *  Get the first child of the given component instance.
     *  Use Get(Next/Prev)Sibling(firstChild) to iterate thru children.
     *
     *  Returns an invalid instance if no children.
     *
     *  @param  i   Instance, component instance to get the first child of.
     *  @return     const Instance &, component instance of first child of
     *  the given component instance.
     */
    const Instance &GetFirstChild(Instance i) const;

    /**
     *  Get the next sibling of the given component instance.
     *
     *  Returns an invalid instance if no next sibling.
     *
     *  @param  i   Instance, component instance to get next sibling of.
     *  @return     const Instance &, component instance of next sibling.
     */
    const Instance &GetNextSibling(Instance i) const;

    /**
     *  Get the previous sibling of the given component instance.
     *
     *  Returns an invalid instance if no previous sibling.
     *
     *  @param  i   Instance, component instance to get previous sibling of.
     *  @return     const Instance &, component instance of previous sibling.
     */
    const Instance &GetPrevSibling(Instance i) const;

private:
    /**
     *  Updates parent, child, sibling references before an object is
     *  moved from one place into memory to another.
     *
     *  @param  oldAddress  const Instance &, old instance address.
     *  @param  newAddress  const Instance &, new instance address.
     */
    virtual void OnAddressChange(const Instance &oldAddress,
                                 const Instance &newAddress);

    /**
     *  Update the world transform of a given component instance with the
     *  new world transform of the given parent.
     *
     *  @param  i   Instance, component instance to update world transform of.
     *  @param  parentTransform     const ds_math::Matrix4 &, world transform
     *  of the parent to update the child with.
     */
    // void UpdateWorldTransform(Instance i,
    //                           const ds_math::Matrix4 &parentTransform);

    void UpdateWorldTranslation(Instance i,
                                const ds_math::Vector3 &parentTranslation);

    void UpdateWorldScale(Instance i, const ds_math::Vector3 &parentScale);

    void UpdateWorldOrientation(Instance i,
                                const ds_math::Quaternion &parentOrientation);
};
}
