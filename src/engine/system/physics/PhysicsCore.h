#pragma once

#include "math/Precision.h"

namespace ds_phys
{
/**
 * How close to zero an objects velocity has to be before it is put to sleep.
 */
ds_math::scalar sleepEpsilon = ((ds_math::scalar)0.01);
}
