#include <iostream>

#include "engine/system/custom_physics/CustomPhysics.h"

#define META_NAME "CustomPhysics"

namespace ds_lua
{
const char *customPhysicsSystemLuaName = META_NAME;

static int l_SetParticleMass(lua_State *L)
{
    // Expect start and end points for ray
    int n = lua_gettop(L);
    int expected = 2;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push render system pointer onto stack
    lua_getglobal(L, "__" META_NAME);

    // If the first item on stack isn't user data (out physics system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::CustomPhysics *physicsPtr = (ds::CustomPhysics *)lua_touserdata(L, -1);
        assert(physicsPtr != NULL);

        // Pop user data off stack now that we are done with it.
        lua_pop(L, 1);

        ds::Entity *entity = NULL;

        entity = (ds::Entity *)lua_touserdata(L, 1);
        ds_math::scalar mass = (ds_math::scalar)luaL_checknumber(L, 2);

        if (entity != NULL)
        {
            physicsPtr->SetParticleMass(*entity, mass);
        }
    }

    // Entity argument and mass argument
    assert(lua_gettop(L) == 2);

    return 0;
}

static int l_SetParticleVelocity(lua_State *L)
{
    // Expect start and end points for ray
    int n = lua_gettop(L);
    int expected = 2;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push render system pointer onto stack
    lua_getglobal(L, "__" META_NAME);

    // If the first item on stack isn't user data (out physics system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::CustomPhysics *physicsPtr = (ds::CustomPhysics *)lua_touserdata(L, -1);
        assert(physicsPtr != NULL);

        // Pop user data off stack now that we are done with it.
        lua_pop(L, 1);

        ds::Entity *entity = NULL;
        ds_math::Vector3 *velocity = NULL;

        entity = (ds::Entity *)lua_touserdata(L, 1);
        velocity = (ds_math::Vector3 *)lua_touserdata(L, 2);

        if (entity != NULL && velocity != NULL)
        {
            physicsPtr->SetParticleVelocity(*entity, *velocity);
        }
    }

    // Entity argument and velocity argument
    assert(lua_gettop(L) == 2);

    return 0;
}

static int l_SetParticleAcceleration(lua_State *L)
{
    // Expect start and end points for ray
    int n = lua_gettop(L);
    int expected = 2;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push render system pointer onto stack
    lua_getglobal(L, "__" META_NAME);

    // If the first item on stack isn't user data (out physics system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::CustomPhysics *physicsPtr = (ds::CustomPhysics *)lua_touserdata(L, -1);
        assert(physicsPtr != NULL);

        // Pop user data off stack now that we are done with it.
        lua_pop(L, 1);

        ds::Entity *entity = NULL;
        ds_math::Vector3 *acceleration = NULL;

        entity = (ds::Entity *)lua_touserdata(L, 1);
        acceleration = (ds_math::Vector3 *)lua_touserdata(L, 2);

        if (entity != NULL && acceleration != NULL)
        {
            physicsPtr->SetParticleAcceleration(*entity, *acceleration);
        }
    }

    // Entity argument and acceleration argument
    assert(lua_gettop(L) == 2);

    return 0;
}

static int l_SetParticleDamping(lua_State *L)
{
    // Expect start and end points for ray
    int n = lua_gettop(L);
    int expected = 2;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push render system pointer onto stack
    lua_getglobal(L, "__" META_NAME);

    // If the first item on stack isn't user data (out physics system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::CustomPhysics *physicsPtr = (ds::CustomPhysics *)lua_touserdata(L, -1);
        assert(physicsPtr != NULL);

        // Pop user data off stack now that we are done with it.
        lua_pop(L, 1);

        ds::Entity *entity = NULL;

        entity = (ds::Entity *)lua_touserdata(L, 1);
        ds_math::scalar damping = (ds_math::scalar)luaL_checknumber(L, 2);

        if (entity != NULL)
        {
            physicsPtr->SetParticleDamping(*entity, damping);
        }
    }

    // Entity argument and damping argument
    assert(lua_gettop(L) == 2);

    return 0;
}

ds::ScriptBindingSet LoadCustomPhysicsScriptBindings()
{
    ds::ScriptBindingSet scriptBindings;
    scriptBindings.AddFunction("set_particle_mass", l_SetParticleMass);
    scriptBindings.AddFunction("set_particle_velocity", l_SetParticleVelocity);
    scriptBindings.AddFunction("set_particle_acceleration",
                               l_SetParticleAcceleration);
    scriptBindings.AddFunction("set_particle_damping", l_SetParticleDamping);

    return scriptBindings;
}
}
