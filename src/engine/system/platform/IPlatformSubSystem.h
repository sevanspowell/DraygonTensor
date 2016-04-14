#pragma once

#include "engine/Config.h"
#include "engine/message/Message.h"

namespace ds
{
/**
 * Although this mimics the ISystem interface exactly, it is useful to have a
 * seperate interface to distinguish a high-level system from a low-level
 * system.
 */
class IPlatformSubSystem
{
    /**
     * Virtual destructor. Required so that the destructor of derived classes is
     * called properly.
     */
    virtual ~IPlatformSubSystem()
    {
    }

    /**
     * Perform any necessary initialization of the system.
     */
    virtual bool Initialize(const Config &config) = 0;
};
}
