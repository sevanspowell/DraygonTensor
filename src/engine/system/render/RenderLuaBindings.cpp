#include "engine/system/render/Render.h"

namespace ds_lua
{
static int l_GetCameraOrientation(lua_State *L)
{
    int n = lua_gettop(L);
    int expected = 1;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push render system pointer onto stack
    lua_getglobal(L, "__Render");

    // If first item on stack isn't user data (our input system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Render *renderPtr = (ds::Render *)lua_touserdata(L, -1);

        assert(renderPtr != NULL);

        ds::Entity *entity = NULL;

        entity = (ds::Entity *)lua_touserdata(L, 1);

        if (entity != NULL)
        {
            ds_math::Quaternion cameraOrientation =
                renderPtr->GetCameraOrientation(*entity);

            // Pop user data off stack now that we are done with it
            lua_pop(L, 1);

            // Allocate memory for Quaternion
            ds_math::Quaternion *orientation =
                (ds_math::Quaternion *)lua_newuserdata(
                    L, sizeof(ds_math::Quaternion));

            *orientation = cameraOrientation;

            // Get Quaternion metatable and put on top of stack
            luaL_getmetatable(L, "Quaternion");
            // Set it as metatable of new user data (the Quaternion - second
            // from top on stack)
            lua_setmetatable(L, -2);
        }
    }

    // Entity passed in and orientation result
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_SetCameraOrientation(lua_State *L)
{
    int n = lua_gettop(L);
    int expected = 2;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push render system pointer onto stack
    lua_getglobal(L, "__Render");

    // If first item on stack isn't user data (our input system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Render *renderPtr = (ds::Render *)lua_touserdata(L, -1);

        assert(renderPtr != NULL);

        ds::Entity *entity = NULL;
        ds_math::Quaternion *orientation = NULL;

        entity = (ds::Entity *)lua_touserdata(L, 1);
        orientation = (ds_math::Quaternion *)lua_touserdata(L, 2);

        if (entity != NULL && orientation != NULL)
        {
            renderPtr->SetCameraOrientation(*entity, *orientation);
        }

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);
    }

    // Entity and orientation passed in
    assert(lua_gettop(L) == 2);

    return 0;
}

ds::ScriptBindingSet LoadRenderScriptBindings()
{
    ds::ScriptBindingSet scriptBindings;
    scriptBindings.AddFunction("get_camera_orientation",
                               l_GetCameraOrientation);
    scriptBindings.AddFunction("set_camera_orientation",
                               l_SetCameraOrientation);

    return scriptBindings;
}
}
