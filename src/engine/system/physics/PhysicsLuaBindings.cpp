#include <iostream>

#include "engine/system/physics/Physics.h"

#define META_NAME "Physics"

namespace ds_lua
{
const char *physicsSystemLuaName = META_NAME;

static int l_AddForceGenerator(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 0.", n);
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

ds_phys::CollisionPrimitiveID l_AddPlane(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expect = 2;
    if (n != expect)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expect);
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

        ds_math::Vector3 *normal = NULL;
        normal = (ds_math::Vector3 *)luaL_checkudata(L, 1, "Vector3");

        ds_math::scalar offset = (ds_math::scalar)luaL_checknumber(L, 2);

        if (normal != NULL)
        {
            p->addPlane(*normal, offset);
        }
    }

    assert(lua_gettop(L) == 2);

    return 0;
}

ds_phys::CollisionPrimitiveID l_GetRigidBody(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expect = 1;
    if (n != expect)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expect);
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

        ds::Entity *e = NULL;
        e = (ds::Entity *)luaL_checkudata(L, 1, "Entity");

        if (e != NULL)
        {

            ds::Entity *rb = (ds::Entity *)lua_newuserdata(L, sizeof(ds::Entity));

            ds_phys::RigidBody *body = p->getRigidBody(*e);

            if (body == nullptr) {
                return luaL_error(L, "Entity %d has no rigid body.", e->id);
            }

            *rb = *e;
            luaL_getmetatable(L, "RigidBody");
            lua_setmetatable(L, -2);
        }
    }

    assert(lua_gettop(L) == 2);

    return 1;
}

ds::ScriptBindingSet LoadPhysicsScriptBindings()
{
    ds::ScriptBindingSet scriptBindings;
    scriptBindings.AddFunction("add_force_generator", l_AddForceGenerator);
    scriptBindings.AddFunction("add_plane", l_AddPlane);
    scriptBindings.AddFunction("get_rigid_body", l_GetRigidBody);

    return scriptBindings;
}
}
