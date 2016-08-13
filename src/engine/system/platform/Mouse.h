#pragma once

namespace ds_platform
{
class Mouse
{
public:
    struct ButtonState
    {
        // /**
        //  * uint8_t convertsion operator.
        //  *
        //  * @return  uint8_t, button states.
        //  */
        // inline operator uint8_t() const;

        bool left;
        bool middle;
        bool right;
        // uint8_t left : 1;
        // uint8_t middle : 1;
        // uint8_t right : 1;
    };
};

// Mouse::ButtonState::operator uint8_t() const
// {
//     return left << 2 | middle << 1 | right;
// }
}
