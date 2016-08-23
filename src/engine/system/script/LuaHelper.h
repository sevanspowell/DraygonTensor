#pragma once

extern "C" {
#define LUA_COMPAT_5_1
#define LUA_COMPAT_5_2

#ifdef DS_LUAJIT_WORKAROUND
#	include "luajit-2.0/lua.h"
#	include "luajit-2.0/lauxlib.h"
#	include "luajit-2.0/lualib.h"
#else
#	include "lua.h"
#	include "lauxlib.h"
#	include "lualib.h"
#endif
}

namespace ds_lua
{
/**
 * Register a C function that can be called from the given lua_State.
 *
 * The function name may in-fact be specified in table access form
 * (i.e. 'Math.sin' or even 'MyMath.Math.sin'). This allows the programmer
 * to effectively namespace functions or register member functions. The
 * function name is always assumed to be the final identifier in the sequence
 * ('sin' in the previous example). The other identifiers are registered
 * as metatables. The first identifier in the sequence ('MyMath' in the previous
 * example) is always registered as a global. It is assumed that all identifiers
 * are valid Lua identifiers (http://www.lua.org/manual/5.3/manual.html#3.1).
 *
 * @pre funcName is a period-seperated list of valid identifiers, the last
 * being the function name and the optional others being table names.
 *
 * @param L        lua_State *, lua_State to register the C function with.
 * @param funcName const char *, function name.
 * @param func     lua_CFunction, function pointer.
 */
void RegisterCFunction(lua_State *L, const char *funcName, lua_CFunction func);

/**
 * Push a Lua function onto the stack.
 *
 * The requested Lua function must be accessible from the given lua_State.
 * The function name may in-fact be specified in table access form
 * (for example, 'MyTable.sin' or 'MyTable.MyFuncs.Goodbye'). In this case,
 * the function name is considered a period seperated sequence of Lua
 * identifiers. Each identifier must be a valid Lua identifier. The final
 * identifier is assumed to be a function, the first identifier is assumed to be
 * globally accessible from the lua state.
 *
 * Will push an error string to the stack if an error occurs.
 *
 * @pre The Lua function is accessible from the given lua_State.
 *
 * @param  L        lua_State *, lua_State to retrieve lua function from.
 * @param  funcName const char *, function name.
 * @return          int, 0 if no errors, other if errors.
 */
int PushLuaFunction(lua_State *L, const char *funcName);

/**
 * Register a light user data that can be called from the given lua state.
 *
 * The userdata name may in-fact be specified in table access form
 * (i.e. 'Math.sin' or even 'MyMath.Math.sin'). This allows the programmer
 * to effectively namespace userdatas or register member userdatas. The
 * userdata name is always assumed to be the final identifier in the sequence
 * ('sin' in the previous example). The other identifiers are registered
 * as metatables. The first identifier in the sequence ('MyMath' in the previous
 * example) is always registered as a global. It is assumed that all identifiers
 * are valid Lua identifiers (http://www.lua.org/manual/5.3/manual.html#3.1).
 *
 * @pre userDataName is a period-seperated list of valid identifiers, the last
 * being the function name and the optional others being table names.
 *
 * @param L             lua_State *, lua_State to register the C function with.
 * @param userDataName  const char *, userdata name.
 * @param p             void *, userdata pointer.
 */
void RegisterLightUserData(lua_State *L, const char *userDataName, void *p);

/**
 * Register a "class" with the given lua state.
 *
 * A class is composed of methods and metamethods. The metamethods are special
 * methods called by lua when events occur. See
 * (http://lua-users.org/wiki/MetatableEvents) for a list of possibilities.
 *
 * @param  L            lua_Stat *, lua_State to register the C function with.
 * @param  className    const char *, name of the class to register.
 * @param  methods      const luaL_reg *, class methods to be registered with
 * the class, must be a NULL terminated table of luaL_Reg
 * (http://www.lua.org/manual/5.3/manual.html#luaL_Reg).
 * @param  functions    const luaL_reg *, class static methods to be registered
 * with the class, must be a NULL terminated table of luaL_Reg
 * (http://www.lua.org/manual/5.3/manual.html#luaL_Reg).
 * @param  special      const luaL_reg *, class special methods to be registered
 * with the class, must be a NULL terminated table of luaL_Reg
 * (http://www.lua.org/manual/5.3/manual.html#luaL_Reg). Special methods are
 * filled in the metable of the static methods table. This is useful for
 * constructors (__call).
 */
void RegisterClass(lua_State *L,
                   const char *className,
                   const luaL_Reg *methods,
                   const luaL_Reg *functions,
                   const luaL_Reg *special);
}
