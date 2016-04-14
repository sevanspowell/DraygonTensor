#pragma once

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
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
}
