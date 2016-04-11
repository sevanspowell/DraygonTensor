#include <cmath>

#include "MathHelper.h"

namespace bb_math
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
