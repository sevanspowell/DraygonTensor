#pragma once

namespace bb_render
{
/**
 * Abstract render class.
 *
 * Abstracts away implementation details of any specific graphics API.
 */
class IRenderer
{
public:
    /**
     * Allows renderer to perform any necessary initialization.
     *
     * Should be called before any other renderer methods.
     *
     * @return bool, TRUE if renderer initialized successfully, FALSE otherwise.
     */
    virtual bool Init() = 0;

private:
};
}
