#include "engine/system/physics/Physics.h"
#include "math/Vector3.h"

namespace ds_lua
{
static int l_GetTerrainHeight(lua_State *L)
{
    int n = lua_gettop(L);
    int expected = 2;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push render system pointer onto stack
    lua_getglobal(L, "__Physics");

    // If the first item on stack isn't user data (out physics system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Physics *physicsPtr = (ds::Physics *)lua_touserdata(L, -1);
        assert(physicsPtr != NULL);

        // Pop user data off stack now that we are done with it.
        lua_pop(L, 1);

        ds::Entity *entity = NULL;
        entity = (ds::Entity *)lua_touserdata(L, 1);

        ds_math::Vector3 *pos = NULL;
        pos = (ds_math::Vector3 *)luaL_checkudata(L, 2, "Vector3");

        if (entity != NULL && pos != NULL)
        {
            // Get terrain height
            float height = physicsPtr->GetTerrainHeight(*entity, *pos);
            lua_pushnumber(L, height);
        }
    }

    // Two arguments provided and float returned
    assert(lua_gettop(L) == 3);

    return 1;
}

ds::ScriptBindingSet LoadPhysicsScriptBindings()
{
    ds::ScriptBindingSet scriptBindings;
    scriptBindings.AddFunction("get_terrain_height", l_GetTerrainHeight);

    return scriptBindings;
}
}
