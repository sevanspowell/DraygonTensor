#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>

#include "engine/system/script/Script.h"
#include "engine/system/script/LuaEnvironment.h"
#include "math/Vector3.h"

namespace ds_lua
{
static int spawnUnit(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    const char *unitFile = luaL_checklstring(L, 1, NULL);

    // Push script system pointer to stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *p = (ds::Script *)lua_touserdata(L, -1);

        assert(p != NULL &&
               "spawnUnit: Tried to deference userdata pointer which was null");

        p->SpawnUnit(unitFile);
    }

    // Pop arguments
    lua_pop(L, 1);
    // Pop script system pointer
    lua_pop(L, 1);

    // Ensure stack is clean
    assert(lua_gettop(L) == 0);

    return 0;
}

static int LVector3Ctor(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 4)
    {
        return luaL_error(L, "Got %d arguments, expected 4.", n);
    }

    // Allocate memory for Vector3
    ds_math::Vector3 *v =
        (ds_math::Vector3 *)lua_newuserdata(L, sizeof(ds_math::Vector3));

    ds_math::scalar x = (ds_math::scalar)luaL_checknumber(L, 2);
    ds_math::scalar y = (ds_math::scalar)luaL_checknumber(L, 3);
    ds_math::scalar z = (ds_math::scalar)luaL_checknumber(L, 4);

    *v = ds_math::Vector3(x, y, z);

    // Get Vector3 metatable
    luaL_getmetatable(L, "Vector3");
    // Set it as metatable of new user data (the Vector3)
    lua_setmetatable(L, -2);

    // userdata that called this method, x, y, z values and Vector3 constructed
    assert(lua_gettop(L) == 5);

    return 1;
}

static int LVector3New(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 3)
    {
        return luaL_error(L, "Got %d arguments, expected 3.", n);
    }

    // Allocate memory for Vector3
    ds_math::Vector3 *v =
        (ds_math::Vector3 *)lua_newuserdata(L, sizeof(ds_math::Vector3));

    ds_math::scalar x = (ds_math::scalar)luaL_checknumber(L, 1);
    ds_math::scalar y = (ds_math::scalar)luaL_checknumber(L, 2);
    ds_math::scalar z = (ds_math::scalar)luaL_checknumber(L, 3);

    *v = ds_math::Vector3(x, y, z);

    // Get Vector3 metatable
    luaL_getmetatable(L, "Vector3");
    // Set it as metatable of new user data (the Vector3)
    lua_setmetatable(L, -2);

    // userdata that called this method, x, y, z values and Vector3 constructed
    assert(lua_gettop(L) == 4);

    return 1;
}

static int LVector3ToString(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Vector3 *v = NULL;

    v = (ds_math::Vector3 *)luaL_checkudata(L, 1, "Vector3");

    if (v != NULL)
    {
        std::stringstream ss;
        ss << *v;
        lua_pushstring(L, ss.str().c_str());
    }

    // user data that called this method, string representation
    assert(lua_gettop(L) == 2);

    return 1;
}

static int LVector3GetX(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Vector3 *v = NULL;

    v = (ds_math::Vector3 *)luaL_checkudata(L, 1, "Vector3");

    if (v != NULL)
    {
        lua_pushnumber(L, v->x);
    }

    // user data that called this method, x member value
    assert(lua_gettop(L) == 2);

    return 1;
}

static int LVector3SetX(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2.", n);
    }

    ds_math::Vector3 *v = NULL;

    v = (ds_math::Vector3 *)luaL_checkudata(L, 1, "Vector3");

    if (v != NULL)
    {
        v->x = (ds_math::scalar)luaL_checknumber(L, 2);
    }

    // user data that called this method, x member value given
    assert(lua_gettop(L) == 2);

    return 0;
}

static const luaL_Reg vector3Methods[] = {{"__tostring", LVector3ToString},
                                          {"get_x", LVector3GetX},
                                          {"set_x", LVector3SetX},
                                          {NULL, NULL}};

static const luaL_Reg vector3Functions[] = {
    {"new", LVector3New}, {NULL, NULL},
};

static const luaL_Reg vector3Special[] = {
    {"__call", LVector3Ctor}, {NULL, NULL},
};

void LoadMathAPI(LuaEnvironment &luaEnv)
{
    luaEnv.RegisterCFunction("World.spawn_unit", spawnUnit);

    luaEnv.RegisterClass("Vector3", vector3Methods, vector3Functions,
                         vector3Special);
}
}
