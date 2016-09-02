#include <iostream>

#include "math/Vector3.h"

#include "engine/system/physics/Physics.h"

#define META_NAME "Physics"

namespace ds_lua
{
const char *physicsSystemLuaName = META_NAME;

static int l_SetGravity(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 1;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push physics system pointer to stack
    lua_getglobal(L, "__" META_NAME);

    // If first item on stack isn't user data (our physics system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Physics *p = (ds::Physics *)lua_touserdata(L, -1);

        assert(p != NULL);

        // Pop physics system pointer off lua stack
        lua_pop(L, 1);

        ds_math::Vector3 *gravity = NULL;

        gravity = (ds_math::Vector3 *)luaL_checkudata(L, 1, "Vector3");

        if (gravity != NULL)
        {
            p->SetGravity(*gravity);
        }
    }

    lua_pop(L, 1);

    assert(lua_gettop(L) == 0);

    return 0;
}

static int l_AddForceGenerator(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 1;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push physics system pointer to stack
    lua_getglobal(L, "__" META_NAME);

    // If first item on stack isn't user data (our physics system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Physics *p = (ds::Physics *)lua_touserdata(L, -1);

        assert(p != NULL);

        // Pop physics system pointer off lua stack
        lua_pop(L, 1);

        ds::Entity *entity = NULL;

        entity = (ds::Entity *)luaL_checkudata(L, 1, "Entity");

        if (entity != NULL)
        {
            p->AddForceGenerator(*entity);
        }
    }

    lua_pop(L, 1);

    assert(lua_gettop(L) == 0);

    return 0;
}

ds::ScriptBindingSet LoadPhysicsScriptBindings()
{
    ds::ScriptBindingSet scriptBindings;
    scriptBindings.AddFunction("add_force_generator", l_AddForceGenerator);
    scriptBindings.AddFunction("set_gravity", l_SetGravity);

    return scriptBindings;
}
}
