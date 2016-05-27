#pragma once

#include "engine/entity/ComponentManager.h"
#include "engine/system/render/ButtonComponent.h"

namespace ds_render
{
/**
 * Manages render components.
 */
class ButtonComponentManager : public ds::ComponentManager<ButtonComponent>
{
public:
    /**
     * Get the default material of the given component instance.
     *
     * @param   i  ds::Instance, component instance to get mesh of.
     * @return     const Material &, default material of given component
     * instance.
     */
    const Material &GetDefaultMaterial(ds::Instance i) const;

    /**
     * Set the default material of the given component instance.
     *
     * @param   i  ds::Instance, component instance to set the material of.
     * @param      const Material &, material to set.
     */
    void SetDefaultMaterial(ds::Instance i, const Material &material);

    /**
     * Get the pressed material of the given component instance.
     *
     * @param   i  ds::Instance, component instance to get mesh of.
     * @return     const Material &, pressed material of given component
     * instance.
     */
    const Material &GetPressedMaterial(ds::Instance i) const;

    /**
     * Set the pressed material of the given component instance.
     *
     * @param   i  ds::Instance, component instance to set the material of.
     * @param      const Material &, material to set.
     */
    void SetPressedMaterial(ds::Instance i, const Material &material);

    /**
     * Get the hover material of the given component instance.
     *
     * @param   i  ds::Instance, component instance to get mesh of.
     * @return     const Material &, hover material of given component
     * instance.
     */
    const Material &GetHoverMaterial(ds::Instance i) const;

    /**
     * Set the hover material of the given component instance.
     *
     * @param   i  ds::Instance, component instance to set the material of.
     * @param      const Material &, material to set.
     */
    void SetHoverMaterial(ds::Instance i, const Material &material);

    /**
     * Get the starting x co-ordinate of the given component instance.
     *
     * @param   i  ds::Instance, component instance to get mesh of.
     * @return     float, starting x co-ordinate of the given component
     * instance.
     */
    float GetStartXCoordinate(ds::Instance i) const;

    /**
     * Set the starting x co-ordinate of the given component instance.
     *
     * @param   i       ds::Instance, component instance to get starting x
     * co-ordinate of.
     * @param   startX  float, starting x co-ordinate to set.
     */
    void SetStartXCoordinate(ds::Instance i, float startX);

    /**
     * Get the starting y co-ordinate of the given component instance.
     *
     * @param   i  ds::Instance, component instance to get mesh of.
     * @return     float, starting y co-ordinate of the given component
     * instance.
     */
    float GetStartYCoordinate(ds::Instance i) const;

    /**
     * Set the starting y co-ordinate of the given component instance.
     *
     * @param   i       ds::Instance, component instance to get starting y
     * co-ordinate of.
     * @param   startY  float, starting y co-ordinate to set.
     */
    void SetStartYCoordinate(ds::Instance i, float startY);

    /**
     * Get the ending x co-ordinate of the given component instance.
     *
     * @param   i  ds::Instance, component instance to get mesh of.
     * @return     float, ending x co-ordinate of the given component
     * instance.
     */
    float GetEndXCoordinate(ds::Instance i) const;

    /**
     * Set the ending x co-ordinate of the given component instance.
     *
     * @param   i       ds::Instance, component instance to get starting x
     * co-ordinate of.
     * @param   endX    float, ending x co-ordinate to set.
     */
    void SetEndXCoordinate(ds::Instance i, float endX);

    /**
     * Get the ending y co-ordinate of the given component instance.
     *
     * @param   i  ds::Instance, component instance to get mesh of.
     * @return     float, ending y co-ordinate of the given component
     * instance.
     */
    float GetEndYCoordinate(ds::Instance i) const;

    /**
     * Set the starting y co-ordinate of the given component instance.
     *
     * @param   i       ds::Instance, component instance to get starting y
     * co-ordinate of.
     * @param   endY    float, ending y co-ordinate to set.
     */
    void SetEndYCoordinate(ds::Instance i, float endY);

private:
};
}
