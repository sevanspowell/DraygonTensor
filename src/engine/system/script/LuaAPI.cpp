#include <iostream>
#include <cmath>

#include "engine/system/script/LuaEnvironment.h"

namespace ds_lua
{
static int l_sin(lua_State *L)
{
    double d = luaL_checknumber(L, 1); // Get argument
    lua_pushnumber(L, sinf(d));        // Push result
    return 1;                          // Number of results
}

void LoadAPI(LuaEnvironment &luaEnv)
{
    luaEnv.RegisterCFunction("mysin", l_sin);
}
}
