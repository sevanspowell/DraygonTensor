#pragma once

#include <string>

namespace ds_platform
{
/**
 * The GraphicsContext class holds information regarding the different types
 * of graphics contexts that can exist in the engine.
 */
class GraphicsContext
{
public:
    /**
     * Context type: OpenGL, DirectX or None
     */
    enum class ContextType
    {
        None,
        OpenGL,
        // DirectX
    };

    /**
     * OpenGL context profile information
     */
    enum class ContextProfileOpenGL
    {
        Core,
        Compatability
    };

    /**
     * OpenGL specific context information
     */
    struct ContextInfoOpenGL
    {
        ContextType type;
        unsigned int majorVersion; // OpenGL major version
        unsigned int minorVersion; // OpenGL minor version
        ContextProfileOpenGL
            profile; // OpenGL profile - "core" or "compatability"
    };

    /**
     * Further information about the graphics context.
     */
    union ContextInfo
    {
        ContextType type;
        ContextInfoOpenGL openGL;
        // ContextInfoDirectX directX;
    };
};
}
