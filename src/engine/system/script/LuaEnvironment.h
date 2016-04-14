/**
 * Based off the class of the same name by Daniele Bartolini:
 * https://github.com/taylor001/crown/blob/master/src/lua/lua_environment.h
 * Copyright (c) 2012-2016 Daniele Bartolini and individual contributors.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
  */
#pragma once

#include <cstdarg>

#include "engine/system/script/LuaHelper.h"

namespace ds_lua
{
/**
 * Encapsulates lua state, handles lua errors, loads the lua/C++ API.
 */
class LuaEnvironment
{
public:
    /**
     * Type of argument to pass to lua function.
     */
    enum ArgumentType
    {
        ARGUMENT_FLOAT
    };

    /**
     * Default constructor.
     */
    LuaEnvironment();

    /**
     * Initialize the Lua environment and load the lua/C++ API
     * (engine/system/script/LuaAPI.cpp).
     *
     * @return  bool, TRUE if initialization was successful, FALSE otherwise.
     */
    bool Init();

    /**
     * Shutdown the lua environment, close the lua state.
     */
    void Shutdown();

    /**
     * Execute a file and add it to the lua state.
     *
     * @pre  Init has been called successfully.
     *
     * @param   filePath  const char *, path to file to execute.
     * @return            bool, TRUE if the file executed successfully, FALSE
     *                    otherwise.
     */
    bool ExecuteFile(const char *filePath);

    /**
     * Execute a lua string.
     *
     * @pre  Init has been called successfully.
     *
     * @param   string  const char *, lua string to execute.
     * @return          bool, TRUE if the file executed successfully, FALSE
     *                  otherwise.
     */
    bool ExecuteString(const char *string);

    /**
     * Call a lua function from C++.
     *
     * The requested Lua function must be globally accessible. The function name
     * may in-fact be specified in table access form (for example, 'MyTable.sin'
     * or 'MyTable.MyFuncs.Goodbye'). In this case, the function name is
     * considered a period seperated sequence of Lua identifiers. Each
     * identifier must be a valid Lua identifier. The final identifier is
     * assumed to be a function.
     *
     * @pre  Init has been called successfully.
     *
     * @param  funcName  const char *, name of the lua function.
     * @param  argc      unsigned int, number of arguments to pass to function.
     * @param  ...       arguments to pass to function. Each argument must be
     *                   provided as a pair: (LuaEnvironment::ArgumentType,
     *                   value).
     *                   Eg: CallLuaFunction("Math.sinf", 1,
     *                    LuaEnvironment::ArgumentType::ARGUMENT_FLOAT,
     *                    3.14159f);
     */
    void CallLuaFunction(const char *funcName, unsigned int argc, ...);

    /**
     * Register a C function with the lua state.
     *
     * The function name may in-fact be specified in table access form
     * (for example, 'MyTable.sin' or 'MyTable.MyFuncs.Goodbye'). In this case,
     * the function name is considered a period seperated sequence of Lua
     * identifiers. Each identifier must be a valid Lua identifier. The final
     * identifier is assumed to be a function, the first identifier is assumed
     * to be globally accessible from the lua state.
     *
     * @pre  Init has been called successfully.
     *
     * @param  funcName  const char *, name of the lua function.
     * @param  func      lua_CFunction, function pointer.
     */
    void RegisterCFunction(const char *funcName, lua_CFunction func);

    /**
     * Push a light userdata global onto the lua stack of this class's lua
     * state.
     *
     * From Lua 5.3 manual
     * (http://www.lua.org/manual/5.3/manual.html#lua_pushlightuserdata):
     * "Userdata represent C values in Lua. A light userdata represents a
     * pointer, a void*. It is a value (like a number): you do not create it, it
     * has no individual metatable, and it is not collected (as it was never
     * created). A light userdata is equal to "any" light userdata with the same
     * C address."
     *
     * The user data name may in-fact be specified in table access form
     * (for example, 'MyTable.sin' or 'MyTable.MyFuncs.Goodbye'). In this case,
     * the user data name is considered a period seperated sequence of Lua
     * identifiers. Each identifier must be a valid Lua identifier. The final
     * identifier is assumed to be a user data, the first identifier is assumed
     * to be a table globally accessible from the lua state.
     *
     * @pre  Init has been called successfully.
     *
     * @param  userDataName  const char *, user data name.
     * @param  p             void *, pointer to light userdata.
     */
    void RegisterLightUserData(const char *userDataName, void *p);

    /**
     * Register a "class" with the given lua state.
     *
     * A class is composed of methods and metamethods. The metamethods are
     * special
     * methods called by lua when events occur. See
     * (http://lua-users.org/wiki/MetatableEvents) for a list of possibilities.
     *
     * @param  className    const char *, name of the class to register.
     * @param  metaMethods  const luaL_reg *, metamethods to be registered with
     * the class, must be a NULL terminated table of luaL_Reg
     * (http://www.lua.org/manual/5.3/manual.html#luaL_Reg).
     * @param  metaMethods  const luaL_reg *, methods to be registered with the
     * class, must be a NULL terminated table of luaL_Reg
     * (http://www.lua.org/manual/5.3/manual.html#luaL_Reg).
     */
    void RegisterClass(const char *className,
                       const luaL_Reg *metaMethods,
                       const luaL_Reg *methods);

private:
    // Prevent the LuaEnvironment from being copied.
    LuaEnvironment(const LuaEnvironment &);
    LuaEnvironment &operator=(const LuaEnvironment &);

    lua_State *m_lua;
};
}
