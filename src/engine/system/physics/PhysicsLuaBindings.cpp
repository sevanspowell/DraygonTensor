#include "engine/system/physics/Physics.h"
#include "math/Vector3.h"

namespace ds_lua
{
static int l_PerformRaycast(lua_State *L)
{
    // Expect start and end points for ray
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

        ds_math::Vector3 *rayStart = NULL;
        ds_math::Vector3 *rayEnd = NULL;

        rayStart = (ds_math::Vector3 *)luaL_checkudata(L, 1, "Vector3");
        rayEnd = (ds_math::Vector3 *)luaL_checkudata(L, 2, "Vector3");

        if (rayStart != NULL && rayEnd != NULL)
        {
            // Get raycast result struct
            ds::Physics::Raycast rayResult =
                physicsPtr->PerformRaycast(*rayStart, *rayEnd);

            // Push a new table to the stack to hold the raycast info
            lua_newtable(L);

            // table.rayStart = Vector3(rayResult.start)
            // Make Vector3
            ds_math::Vector3 *start = (ds_math::Vector3 *)lua_newuserdata(
                L, sizeof(ds_math::Vector3));
            *start = rayResult.start;
            // Get metatable for Vector3 and set it
            luaL_getmetatable(L, "Vector3");
            lua_setmetatable(L, -2);
            // Set start field to start vector
            lua_setfield(L, -2, "rayStart");

            // table.rayEnd = Vector3(rayResult.end)
            // Make Vector3
            ds_math::Vector3 *end = (ds_math::Vector3 *)lua_newuserdata(
                L, sizeof(ds_math::Vector3));
            *end = rayResult.end;
            // Get metatable for Vector3 and set it
            luaL_getmetatable(L, "Vector3");
            lua_setmetatable(L, -2);
            // Set end field to end vector
            lua_setfield(L, -2, "rayEnd");

            // table.rayNormal = Vector3(rayResult.normal)
            // Make Vector3
            ds_math::Vector3 *normal = (ds_math::Vector3 *)lua_newuserdata(
                L, sizeof(ds_math::Vector3));
            *normal = rayResult.normal;
            // Get metatable for Vector3 and set it
            luaL_getmetatable(L, "Vector3");
            lua_setmetatable(L, -2);
            // Set normal field to normal vector
            lua_setfield(L, -2, "rayNormal");

            // table.hasHit = rayResult.hasHit
            // Push boolean to stack
            lua_pushboolean(L, rayResult.hasHit);
            // Set hasHit field to hasHit boolean
            lua_setfield(L, -2, "hasHit");
        }
    }

    // Ray start, ray end arguments and raycast table result
    assert(lua_gettop(L) == 3);

    return 1;
}

ds::ScriptBindingSet LoadPhysicsScriptBindings()
{
    ds::ScriptBindingSet scriptBindings;
    scriptBindings.AddFunction("perform_raycast", l_PerformRaycast);

    return scriptBindings;
}
}
