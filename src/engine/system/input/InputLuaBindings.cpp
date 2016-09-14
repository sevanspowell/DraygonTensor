#include "engine/system/input/Input.h"

#define META_NAME "Input"

namespace ds_lua
{
const char *inputSystemLuaName = META_NAME;

static int l_GetMouseDeltaXY(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 0)
    {
        return luaL_error(L, "Got %d arguments, expected 0.", n);
    }

    // Push input system pointer to stack
    lua_getglobal(L, "__" META_NAME);

    // If first item on stack isn't user data (our input system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Input *p = (ds::Input *)lua_touserdata(L, -1);

        assert(p != NULL && "spawnPrefab: Tried to deference userdata "
                            "pointer which was null");

        // Pop input system pointer
        lua_pop(L, 1);

        // Is key pressed?
        int deltaX = 0;
        int deltaY = 0;

        p->GetMouseDeltaXY(&deltaX, &deltaY);

        lua_pushnumber(L, deltaX);
        lua_pushnumber(L, deltaY);
    }

    // Delta x and y values
    assert(lua_gettop(L) == 2);

    return 2;
}

static int l_IsKeyPressed(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    const char *key = luaL_checklstring(L, 1, NULL);

    // Push input system pointer to stack
    lua_getglobal(L, "__" META_NAME);

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Input *p = (ds::Input *)lua_touserdata(L, -1);

        assert(p != NULL && "spawnPrefab: Tried to deference userdata "
                            "pointer which was null");

        // Pop input system pointer
        lua_pop(L, 1);

        // Is key pressed?
        bool isKeyPressed = p->IsKeyPressed(key);
        lua_pushboolean(L, isKeyPressed);
    }

    // Passed key and bool return
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_WasKeyReleased(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    const char *key = luaL_checklstring(L, 1, NULL);

    // Push input system pointer to stack
    lua_getglobal(L, "__" META_NAME);

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Input *p = (ds::Input *)lua_touserdata(L, -1);

        assert(p != NULL && "spawnPrefab: Tried to deference userdata "
                            "pointer which was null");

        // Pop input system pointer
        lua_pop(L, 1);

        // Is key pressed?
        bool wasKeyReleased = p->WasKeyReleased(key);
        lua_pushboolean(L, wasKeyReleased);
    }

    // Passed key and bool return
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_GetMousePos(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 0)
    {
        return luaL_error(L, "Got %d arguments, expected 0.", n);
    }

    // Push input system pointer to stack
    lua_getglobal(L, "__" META_NAME);

    // If first item on stack isn't user data (our input system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Input *p = (ds::Input *)lua_touserdata(L, -1);

        assert(p != NULL && "spawnPrefab: Tried to deference userdata "
                            "pointer which was null");

        // Pop input system pointer
        lua_pop(L, 1);

        // Is key pressed?
        int mX = 0;
        int mY = 0;

        p->GetMouseState(&mX, &mY);

        lua_pushnumber(L, mX);
        lua_pushnumber(L, mY);
    }

    // Delta x and y values
    assert(lua_gettop(L) == 2);

    return 2;
}

static int l_GetMouseButtonState(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    // Get button
    int button = (int)luaL_checknumber(L, 1);

    // Push input system pointer to stack
    lua_getglobal(L, "__" META_NAME);

    // If first item on stack isn't user data (our input system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Input *p = (ds::Input *)lua_touserdata(L, -1);

        assert(p != NULL && "spawnPrefab: Tried to deference userdata "
                            "pointer which was null");

        // Pop input system pointer
        lua_pop(L, 1);

        ds::Input::ButtonState buttonState = p->GetMouseState(nullptr, nullptr);

        bool result = false;
        switch (button)
        {
        case 0:
        {
            result = buttonState.left;
            break;
        }
        case 1:
        {
            result = buttonState.middle;
            break;
        }
        case 2:
        {
            result = buttonState.right;
            break;
        }
        default:
        {
            break;
        }
        }

        lua_pushboolean(L, result);
    }

    // Given number and boolean result
    assert(lua_gettop(L) == 2);

    return 1;
}

ds::ScriptBindingSet LoadInputScriptBindings()
{
    ds::ScriptBindingSet scriptBindings;
    scriptBindings.AddFunction("is_key_pressed", l_IsKeyPressed);
    scriptBindings.AddFunction("was_key_released", l_WasKeyReleased);
    scriptBindings.AddFunction("get_mouse_delta_xy", l_GetMouseDeltaXY);
    scriptBindings.AddFunction("get_mouse_pos", l_GetMousePos);
    scriptBindings.AddFunction("get_mouse_button_state", l_GetMouseButtonState);

    return scriptBindings;
}
}
