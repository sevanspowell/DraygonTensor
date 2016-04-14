#define _USE_MATH_DEFINES
#include <cmath>

#include "MathHelper.h"

namespace ds_math
{
scalar MathHelper::ToDegrees(scalar radians)
{
    return ((scalar)180.0f * (radians / MathHelper::PI));
}

scalar MathHelper::ToRadians(scalar degrees)
{
    return (MathHelper::PI * (degrees / (scalar)180.0f));
}

scalar MathHelper::PI = M_PI;
}
