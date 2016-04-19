#include "engine/system/input/Input.h"

namespace ds_lua
{
static int l_IsKeyPressed(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    const char *key = luaL_checklstring(L, 1, NULL);

    // Push script system pointer to stack
    lua_getglobal(L, "__Input");

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

        // Pop script system pointer
        lua_pop(L, 1);

        // Is key pressed?
        bool isKeyPressed = p->IsKeyPressed(key);
        lua_pushboolean(L, isKeyPressed);
    }

    // Passed key and bool return
    assert(lua_gettop(L) == 2);

    return 1;
}

ds::ScriptBindingSet LoadInputScriptBindings()
{
    ds::ScriptBindingSet scriptBindings;
    scriptBindings.AddFunction("is_key_pressed", l_IsKeyPressed);

    return scriptBindings;
}
}
