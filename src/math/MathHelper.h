#pragma once

#include "Precision.h"

namespace ds_math
{
class MathHelper
{
public:
    /**
     * Convert given radian quantity to degrees.
     *
     * @author Samuel Evans-Powell
     *
     * @param   radians  scalar, radian quantity to convert.
     * @return           scalar, radian quantity in degrees.
     */
    static scalar ToDegrees(scalar radians);
    /**
     * Convert given degrees quantity to radians.
     *
     * @author Samuel Evans-Powell
     *
     * @param   radians  scalar, degrees quantity to convert.
     * @return           scalar, degrees quantity in radians.
     */
    static scalar ToRadians(scalar degrees);

    /** PI quantity */
    static scalar PI;
};
}
