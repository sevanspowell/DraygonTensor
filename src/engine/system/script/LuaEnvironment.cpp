#include <cassert>
#include <iostream>

#include "engine/system/script/LuaEnvironment.h"

namespace ds_lua
{
extern void LoadAPI(LuaEnvironment &luaEnv);

LuaEnvironment::LuaEnvironment()
{
}

bool LuaEnvironment::Init()
{
    bool result = false;

    // Create lua state
    m_lua = luaL_newstate();

    // If successful
    if (m_lua)
    {
        int oldStackSize = lua_gettop(m_lua);

        // Load lua libraries
        luaL_openlibs(m_lua);

        LoadAPI(*this);

        // Ensure stack is clean
        assert(lua_gettop(m_lua) == oldStackSize);

        result = true;
    }

    return result;
}

void LuaEnvironment::Shutdown()
{
    lua_close(m_lua);
}

bool LuaEnvironment::ExecuteFile(const char *filePath)
{
    bool result = false;

    int oldStackSize = lua_gettop(m_lua);

    if (luaL_loadfile(m_lua, filePath))
    {
        std::cout << "Error: " << lua_tostring(m_lua, -1) << std::endl;
        lua_pop(m_lua, 1);
    }
    else
    {
        if (lua_pcall(m_lua, 0, 0, 0))
        {
            std::cout << "Error: " << lua_tostring(m_lua, -1) << std::endl;
            lua_pop(m_lua, 1);
        }
        else
        {
            result = true;
        }
    }

    // Ensure stack is clean
    assert(lua_gettop(m_lua) == oldStackSize);

    return result;
}

bool LuaEnvironment::ExecuteString(const char *string)
{
    bool result = false;

    int oldStackSize = lua_gettop(m_lua);

    if (luaL_loadstring(m_lua, string))
    {
        std::cout << "Error: " << lua_tostring(m_lua, -1) << std::endl;
        lua_pop(m_lua, 1);
    }
    else
    {
        if (lua_pcall(m_lua, 0, 0, 0))
        {
            std::cout << "Error: " << lua_tostring(m_lua, -1) << std::endl;
            lua_pop(m_lua, 1);
        }
        else
        {
            result = true;
        }
    }

    // Ensure stack is clean
    assert(lua_gettop(m_lua) == oldStackSize);

    return result;
}

void LuaEnvironment::CallLuaFunction(const char *funcName,
                                     unsigned int argc,
                                     ...)
{
    int oldStackSize = lua_gettop(m_lua);

    va_list argumentList;
    va_start(argumentList, argc);

    if (ds_lua::PushLuaFunction(m_lua, funcName) == 0)
    {
        for (unsigned int i = 0; i < argc; ++i)
        {
            const int type = va_arg(argumentList, int);

            switch (type)
            {
            case ArgumentType::ARGUMENT_FLOAT:
            {
                lua_pushnumber(m_lua, va_arg(argumentList, double));
                break;
            }
            default:
            {
                std::cout << "Argument type unknown" << std::endl;
                break;
            }
            }
        }

        // Call lua function
        if (lua_pcall(m_lua, argc, 0, 0) != LUA_OK)
        {
            // If error, print error
            std::cout << "Error: " << lua_tostring(m_lua, -1) << std::endl;
            // Pop off error message
            lua_pop(m_lua, 1);
        }
    }
    else
    {
        std::cout << "Error: " << lua_tostring(m_lua, -1) << std::endl;
        lua_pop(m_lua, 1);
    }

    va_end(argumentList);

    // Ensure stack is clean
    assert(lua_gettop(m_lua) == oldStackSize);
}

void LuaEnvironment::RegisterCFunction(const char *funcName, lua_CFunction func)
{
    ds_lua::RegisterCFunction(m_lua, funcName, func);
}

void LuaEnvironment::PushLightUserData(void *p)
{
    lua_pushlightuserdata(m_lua, p);
}
}
