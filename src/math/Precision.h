#pragma once

namespace ds_math
{
/**
 * This typedef is used throughout the math classes. It allows the programmer to
 * easily change the precision of math and physics calculations.
 */
typedef float scalar;

/**
 * How 'close' two floats need to be in order to be considered equal.
 */
const float FLOAT_ACCURACY = 10.0e-6f;
}
