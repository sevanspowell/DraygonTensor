#pragma once

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
     *  Get the transform (position, orientation, scale) matrix
     *  of an object relative to it's parent.
     *
     *  @param  i   Instance, component instance to get transform matrix of.
     *  @return     const ds_math::Matrix4 &, local transform matrix.
     */
    const ds_math::Matrix4 &GetLocalTransform(Instance i) const;

    /**
     *  Set the transform of an object relative to it's parent. This
     *  method also updates the transforms of the object's children
     *  (if the object has children).
     *
     *  @param  i       Instance, component instance to set the transform
     *  matrix of.
     *  @param  matrix  const ds_math::Matrix4 &, new transform matrix.
     */
    void SetLocalTransform(Instance i, const ds_math::Matrix4 &matrix);

    /**
     *  Get the world transform of the component instance.
     *
     *  @param  i   Instance, component instance to get the world transform
     *  of.
     *  @return     const ds_math::Matrix4 &, world transform of the component
     *  instance.
     */
    const ds_math::Matrix4 &GetWorldTransform(Instance i) const;

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
    void UpdateWorldTransform(Instance i,
                              const ds_math::Matrix4 &parentTransform);
};
}
