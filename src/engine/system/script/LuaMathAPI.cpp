#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>

#include "engine/system/script/Script.h"
#include "engine/system/script/LuaEnvironment.h"
#include "math/Vector3.h"
#include "math/Quaternion.h"
#include "math/Matrix4.h"

namespace ds_lua
{
static int l_Vector3Ctor(lua_State *L)
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

static int l_Vector3New(lua_State *L)
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

static int l_Vector3ToString(lua_State *L)
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

static int l_Vector3GetX(lua_State *L)
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

static int l_Vector3SetX(lua_State *L)
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

static int l_Vector3GetY(lua_State *L)
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
        lua_pushnumber(L, v->y);
    }

    // user data that called this method, y member value
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Vector3SetY(lua_State *L)
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
        v->y = (ds_math::scalar)luaL_checknumber(L, 2);
    }

    // user data that called this method, y member value given
    assert(lua_gettop(L) == 2);

    return 0;
}

static int l_Vector3GetZ(lua_State *L)
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
        lua_pushnumber(L, v->z);
    }

    // user data that called this method, z member value
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Vector3SetZ(lua_State *L)
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
        v->z = (ds_math::scalar)luaL_checknumber(L, 2);
    }

    // user data that called this method, z member value given
    assert(lua_gettop(L) == 2);

    return 0;
}

static int l_Vector3Magnitude(lua_State *L)
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
        lua_pushnumber(L, ds_math::Vector3::Magnitude(*v));
    }

    // user data that called this method, magnitude
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Vector3Normalize(lua_State *L)
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
        // Allocate memory for Vector3
        ds_math::Vector3 *normalizedV =
            (ds_math::Vector3 *)lua_newuserdata(L, sizeof(ds_math::Vector3));

        *normalizedV = ds_math::Vector3::Normalize(*v);

        // Get Vector3 metatable and put on top of stack
        luaL_getmetatable(L, "Vector3");
        // Set it as metatable of new user data (the Vector3 - second from top
        // on stack)
        lua_setmetatable(L, -2);
    }

    // userdata that called this method, normalized Vector3
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Vector3Invert(lua_State *L)
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
        // Allocate memory for Vector3
        ds_math::Vector3 *invertedV =
            (ds_math::Vector3 *)lua_newuserdata(L, sizeof(ds_math::Vector3));

        *invertedV = ds_math::Vector3::Invert(*v);

        // Get Vector3 metatable and put on top of stack
        luaL_getmetatable(L, "Vector3");
        // Set it as metatable of new user data (the Vector3 - second from top
        // on stack)
        lua_setmetatable(L, -2);
    }

    // userdata that called this method, inverted Vector3
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Vector3UnaryInvert(lua_State *L)
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
        // Allocate memory for Vector3
        ds_math::Vector3 *invertedV =
            (ds_math::Vector3 *)lua_newuserdata(L, sizeof(ds_math::Vector3));

        *invertedV = ds_math::Vector3::Invert(*v);

        // Get Vector3 metatable and put on top of stack
        luaL_getmetatable(L, "Vector3");
        // Set it as metatable of new user data (the Vector3 - second from top
        // on stack)
        lua_setmetatable(L, -2);
    }

    // userdata that called this method * 2, inverted Vector3
    // See: http://lua-users.org/lists/lua-l/2010-10/msg00783.html
    assert(lua_gettop(L) == 3);

    return 1;
}

static int l_Vector3Subtract(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2.", n);
    }

    ds_math::Vector3 *v1 = NULL;
    ds_math::Vector3 *v2 = NULL;

    v1 = (ds_math::Vector3 *)luaL_checkudata(L, 1, "Vector3");
    v2 = (ds_math::Vector3 *)luaL_checkudata(L, 2, "Vector3");

    if (v1 != NULL && v2 != NULL)
    {
        // Allocate memory for Vector3 result
        ds_math::Vector3 *sub =
            (ds_math::Vector3 *)lua_newuserdata(L, sizeof(ds_math::Vector3));

        // Subtract two arguments
        *sub = *v1 - *v2;

        // Get Vector3 metatable and put on top of stack
        luaL_getmetatable(L, "Vector3");
        // Set it as metatable of new user data (the Vector3 - second from top
        // on stack)
        lua_setmetatable(L, -2);
    }

    // Two Vector3 arguments, subtraction result
    assert(lua_gettop(L) == 3);

    return 1;
}

static int l_Vector3Dot(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2.", n);
    }

    ds_math::Vector3 *v1 = NULL;
    ds_math::Vector3 *v2 = NULL;

    v1 = (ds_math::Vector3 *)luaL_checkudata(L, 1, "Vector3");
    v2 = (ds_math::Vector3 *)luaL_checkudata(L, 2, "Vector3");

    if (v1 != NULL && v2 != NULL)
    {
        lua_pushnumber(L, ds_math::Vector3::Dot(*v1, *v2));
    }

    // Two Vector3 arguments, dot product result
    assert(lua_gettop(L) == 3);

    return 1;
}

static int l_Vector3Cross(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2.", n);
    }

    ds_math::Vector3 *v1 = NULL;
    ds_math::Vector3 *v2 = NULL;

    v1 = (ds_math::Vector3 *)luaL_checkudata(L, 1, "Vector3");
    v2 = (ds_math::Vector3 *)luaL_checkudata(L, 2, "Vector3");

    if (v1 != NULL && v2 != NULL)
    {
        // Allocate memory for Vector3
        ds_math::Vector3 *cross =
            (ds_math::Vector3 *)lua_newuserdata(L, sizeof(ds_math::Vector3));

        *cross = ds_math::Vector3::Cross(*v1, *v2);

        // Get Vector3 metatable and put on top of stack
        luaL_getmetatable(L, "Vector3");
        // Set it as metatable of new user data (the Vector3 - second from top
        // on stack)
        lua_setmetatable(L, -2);
    }

    // Two Vector3 arguments, cross product result
    assert(lua_gettop(L) == 3);

    return 1;
}

static int l_Vector3UnitX(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 0)
    {
        return luaL_error(L, "Got %d arguments, expected 0.", n);
    }


    // Allocate memory for Vector3
    ds_math::Vector3 *unitX =
        (ds_math::Vector3 *)lua_newuserdata(L, sizeof(ds_math::Vector3));

    *unitX = ds_math::Vector3::UnitX;

    // Get Vector3 metatable and put on top of stack
    luaL_getmetatable(L, "Vector3");
    // Set it as metatable of new user data (the Vector3 - second from top
    // on stack)
    lua_setmetatable(L, -2);

    // Unit vector result
    assert(lua_gettop(L) == 1);

    return 1;
}

static int l_Vector3UnitY(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 0)
    {
        return luaL_error(L, "Got %d arguments, expected 0.", n);
    }


    // Allocate memory for Vector3
    ds_math::Vector3 *unitX =
        (ds_math::Vector3 *)lua_newuserdata(L, sizeof(ds_math::Vector3));

    *unitX = ds_math::Vector3::UnitY;

    // Get Vector3 metatable and put on top of stack
    luaL_getmetatable(L, "Vector3");
    // Set it as metatable of new user data (the Vector3 - second from top
    // on stack)
    lua_setmetatable(L, -2);

    // Unit vector result
    assert(lua_gettop(L) == 1);

    return 1;
}

static int l_Vector3UnitZ(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 0)
    {
        return luaL_error(L, "Got %d arguments, expected 0.", n);
    }

    // Allocate memory for Vector3
    ds_math::Vector3 *unitX =
        (ds_math::Vector3 *)lua_newuserdata(L, sizeof(ds_math::Vector3));

    *unitX = ds_math::Vector3::UnitZ;

    // Get Vector3 metatable and put on top of stack
    luaL_getmetatable(L, "Vector3");
    // Set it as metatable of new user data (the Vector3 - second from top
    // on stack)
    lua_setmetatable(L, -2);

    // Unit vector result
    assert(lua_gettop(L) == 1);

    return 1;
}

static int l_Vector3MatrixTransform(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2", n);
    }

    // Get vector and matrix arguments
    ds_math::Vector3 *v = NULL;
    ds_math::Matrix4 *m = NULL;

    v = (ds_math::Vector3 *)luaL_checkudata(L, 1, "Vector3");
    m = (ds_math::Matrix4 *)luaL_checkudata(L, 2, "Matrix4");

    if (v != NULL && m != NULL)
    {
        // Allocate memory for transformed Vector3
        ds_math::Vector3 *transformedV =
            (ds_math::Vector3 *)lua_newuserdata(L, sizeof(ds_math::Vector3));

        *transformedV = ds_math::Vector3::Transform(*v, *m);

        // Get Vector3 metatable and put on top of stack
        luaL_getmetatable(L, "Vector3");
        // Set it as metatable of new user data (the Vector3 - second from top
        // on stack)
        lua_setmetatable(L, -2);
    }

    // Vector3 and Matrix4 arguments and transformed vector result
    assert(lua_gettop(L) == 3);

    return 1;
}

static int l_Vector3GetRotationFromTo(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2.", n);
    }

    ds_math::Vector3 *v1 = NULL;
    ds_math::Vector3 *v2 = NULL;

    v1 = (ds_math::Vector3 *)luaL_checkudata(L, 1, "Vector3");
    v2 = (ds_math::Vector3 *)luaL_checkudata(L, 2, "Vector3");

    if (v1 != NULL && v2 != NULL)
    {
        // Allocate memory for Quaternion result
        ds_math::Quaternion *rotation =
            (ds_math::Quaternion *)lua_newuserdata(L, sizeof(ds_math::Quaternion));

        // Get rotation from first argument vector to second
        *rotation = ds_math::Vector3::GetRotationFromTo(*v1, *v2);

        // Get Quaternion metatable and put on top of stack
        luaL_getmetatable(L, "Quaternion");
        // Set it as metatable of new user data (the Quaternion - second from top
        // on stack)
        lua_setmetatable(L, -2);
    }

    // Two Vector3 arguments, Quaternion rotation result
    assert(lua_gettop(L) == 3);

    return 1;
}

static int l_QuaternionNew(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 4)
    {
        return luaL_error(L, "Got %d arguments, expected 4.", n);
    }

    // Allocate memory for Quaternion
    ds_math::Quaternion *q =
        (ds_math::Quaternion *)lua_newuserdata(L, sizeof(ds_math::Quaternion));

    ds_math::scalar x = (ds_math::scalar)luaL_checknumber(L, 1);
    ds_math::scalar y = (ds_math::scalar)luaL_checknumber(L, 2);
    ds_math::scalar z = (ds_math::scalar)luaL_checknumber(L, 3);
    ds_math::scalar w = (ds_math::scalar)luaL_checknumber(L, 4);

    *q = ds_math::Quaternion(x, y, z, w);

    // Get Vector3 metatable
    luaL_getmetatable(L, "Quaternion");
    // Set it as metatable of new user data (the Vector3)
    lua_setmetatable(L, -2);

    // x, y, z and w values and Quaternion
    // constructed
    assert(lua_gettop(L) == 5);

    return 1;
}

static int l_QuaternionToString(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Quaternion *q = NULL;

    q = (ds_math::Quaternion *)luaL_checkudata(L, 1, "Quaternion");

    if (q != NULL)
    {
        std::stringstream ss;
        ss << *q;
        lua_pushstring(L, ss.str().c_str());
    }

    // user data passed to this method, string representation
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_QuaternionCtor(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 5)
    {
        return luaL_error(L, "Got %d arguments, expected 5.", n);
    }

    // Allocate memory for Quaternion
    ds_math::Quaternion *q =
        (ds_math::Quaternion *)lua_newuserdata(L, sizeof(ds_math::Quaternion));

    ds_math::scalar x = (ds_math::scalar)luaL_checknumber(L, 2);
    ds_math::scalar y = (ds_math::scalar)luaL_checknumber(L, 3);
    ds_math::scalar z = (ds_math::scalar)luaL_checknumber(L, 4);
    ds_math::scalar w = (ds_math::scalar)luaL_checknumber(L, 5);

    *q = ds_math::Quaternion(x, y, z, w);

    // Get Quaternion metatable
    luaL_getmetatable(L, "Quaternion");
    // Set it as metatable of new user data (the Quaternion)
    lua_setmetatable(L, -2);

    // userdata passed to this method, x, y, z, w values and Quaternion
    // constructed
    assert(lua_gettop(L) == 6);

    return 1;
}

static int l_QuaternionMagnitude(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Quaternion *q = NULL;

    q = (ds_math::Quaternion *)luaL_checkudata(L, 1, "Quaternion");

    if (q != NULL)
    {
        lua_pushnumber(L, ds_math::Quaternion::Magnitude(*q));
    }

    // user data that called this method, magnitude
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_QuaternionNormalize(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Quaternion *q = NULL;

    q = (ds_math::Quaternion *)luaL_checkudata(L, 1, "Quaternion");

    if (q != NULL)
    {
        // Allocate memory for Quaternion
        ds_math::Quaternion *normalizedQ =
            (ds_math::Quaternion *)lua_newuserdata(L,
                                                   sizeof(ds_math::Quaternion));

        *normalizedQ = ds_math::Quaternion::Normalize(*q);

        // Get Quaternion metatable and put on top of stack
        luaL_getmetatable(L, "Quaternion");
        // Set it as metatable of new user data (the Quaternion - second from
        // top
        // on stack)
        lua_setmetatable(L, -2);
    }

    // userdata that called this method, normalized Quaternion
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_QuaternionInvert(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Quaternion *q = NULL;

    q = (ds_math::Quaternion *)luaL_checkudata(L, 1, "Quaternion");

    if (q != NULL)
    {
        // Allocate memory for Quaternion
        ds_math::Quaternion *invertedQ = (ds_math::Quaternion *)lua_newuserdata(
            L, sizeof(ds_math::Quaternion));

        *invertedQ = ds_math::Quaternion::Invert(*q);

        // Get Quaternion metatable and put on top of stack
        luaL_getmetatable(L, "Quaternion");
        // Set it as metatable of new user data (the Quaternion - second from
        // top
        // on stack)
        lua_setmetatable(L, -2);
    }

    // userdata that called this method, inverted Quaternion
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_QuaternionDot(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2.", n);
    }

    ds_math::Quaternion *q1 = NULL;
    ds_math::Quaternion *q2 = NULL;

    q1 = (ds_math::Quaternion *)luaL_checkudata(L, 1, "Quaternion");
    q2 = (ds_math::Quaternion *)luaL_checkudata(L, 2, "Quaternion");

    if (q1 != NULL && q2 != NULL)
    {
        lua_pushnumber(L, ds_math::Quaternion::Dot(*q1, *q2));
    }

    // Two Quaternion arguments, dot product result
    assert(lua_gettop(L) == 3);

    return 1;
}

static int l_QuaternionMul(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2.", n);
    }

    ds_math::Quaternion *q1 = NULL;
    ds_math::Quaternion *q2 = NULL;

    q1 = (ds_math::Quaternion *)luaL_checkudata(L, 1, "Quaternion");
    q2 = (ds_math::Quaternion *)luaL_checkudata(L, 2, "Quaternion");

    if (q1 != NULL && q2 != NULL)
    {
        // Allocate memory for Quaternion
        ds_math::Quaternion *mul = (ds_math::Quaternion *)lua_newuserdata(
            L, sizeof(ds_math::Quaternion));

        *mul = (*q1) * (*q2);

        // Get Quaternion metatable and put on top of stack
        luaL_getmetatable(L, "Quaternion");
        // Set it as metatable of new user data (the Quaternion - second from
        // top
        // on stack)
        lua_setmetatable(L, -2);
    }

    // Two Quaternion arguments, quaternion multiplication result
    assert(lua_gettop(L) == 3);

    return 1;
}

static int l_QuaternionFromAxisAngle(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2.", n);
    }

    ds_math::Vector3 *axis = NULL;

    axis = (ds_math::Vector3 *)luaL_checkudata(L, 1, "Vector3");

    if (axis != NULL)
    {
        ds_math::scalar angle = (ds_math::scalar)luaL_checknumber(L, 2);

        // Allocate memory for Quaternion
        ds_math::Quaternion *q = (ds_math::Quaternion *)lua_newuserdata(
            L, sizeof(ds_math::Quaternion));

        *q = ds_math::Quaternion::CreateFromAxisAngle(*axis, angle);

        // Get Quaternion metatable and put on top of stack
        luaL_getmetatable(L, "Quaternion");
        // Set it as metatable of new user data (the Quaternion - second from
        // top
        // on stack)
        lua_setmetatable(L, -2);
    }

    // Axis and angle arguments and quaternion created
    assert(lua_gettop(L) == 3);

    return 1;
}

static int l_Matrix4Ctor(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2.", n);
    }

    // Allocate memory for Matrix4
    ds_math::Matrix4 *m =
        (ds_math::Matrix4 *)lua_newuserdata(L, sizeof(ds_math::Matrix4));

    ds_math::scalar leading = (ds_math::scalar)luaL_checknumber(L, 2);

    *m = ds_math::Matrix4(leading);

    // Get Matrix4 metatable
    luaL_getmetatable(L, "Matrix4");
    // Set it as metatable of new user data (the Matrix4)
    lua_setmetatable(L, -2);

    // userdata passed to this method, leading value argument and Matrix4
    // constructed.
    assert(lua_gettop(L) == 3);

    return 1;
}

static int l_Matrix4ToString(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Matrix4 *m = NULL;

    m = (ds_math::Matrix4 *)luaL_checkudata(L, 1, "Matrix4");

    if (m != NULL)
    {
        std::stringstream ss;
        ss << *m;
        lua_pushstring(L, ss.str().c_str());
    }

    // user data passed to this method, string representation
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Matrix4Transpose(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Matrix4 *m = NULL;

    // Get matrix from Lua
    m = (ds_math::Matrix4 *)luaL_checkudata(L, 1, "Matrix4");

    if (m != NULL)
    {
        // Allocate memory for Matrix4
        ds_math::Matrix4 *transposed =
            (ds_math::Matrix4 *)lua_newuserdata(L, sizeof(ds_math::Matrix4));

        // Transpose matrix
        *transposed = ds_math::Matrix4::Transpose(*m);

        // Get Matrix4 metatable
        luaL_getmetatable(L, "Matrix4");
        // Set it as metatable of new user data (the Matrix4)
        lua_setmetatable(L, -2);
    }

    // user data passed to this method, transposed matrix
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Matrix4Invert(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Matrix4 *m = NULL;

    // Get matrix from Lua
    m = (ds_math::Matrix4 *)luaL_checkudata(L, 1, "Matrix4");

    if (m != NULL)
    {
        // Allocate memory for Matrix4
        ds_math::Matrix4 *inverse =
            (ds_math::Matrix4 *)lua_newuserdata(L, sizeof(ds_math::Matrix4));

        // Transpose matrix
        *inverse = ds_math::Matrix4::Inverse(*m);

        // Get Matrix4 metatable
        luaL_getmetatable(L, "Matrix4");
        // Set it as metatable of new user data (the Matrix4)
        lua_setmetatable(L, -2);
    }

    // user data passed to this method, inverted matrix
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Matrix4CreateTranslation(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Vector3 *v = NULL;

    // Get matrix from Lua
    v = (ds_math::Vector3 *)luaL_checkudata(L, 1, "Vector3");

    if (v != NULL)
    {
        // Allocate memory for Matrix4
        ds_math::Matrix4 *m =
            (ds_math::Matrix4 *)lua_newuserdata(L, sizeof(ds_math::Matrix4));

        // Transpose matrix
        *m = ds_math::Matrix4::CreateTranslationMatrix(*v);

        // Get Matrix4 metatable
        luaL_getmetatable(L, "Matrix4");
        // Set it as metatable of new user data (the Matrix4)
        lua_setmetatable(L, -2);
    }

    // user data passed to this method, constructed matrix
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Matrix4CreateScale(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Vector3 *v = NULL;

    // Get matrix from Lua
    v = (ds_math::Vector3 *)luaL_checkudata(L, 1, "Vector3");

    if (v != NULL)
    {
        // Allocate memory for Matrix4
        ds_math::Matrix4 *m =
            (ds_math::Matrix4 *)lua_newuserdata(L, sizeof(ds_math::Matrix4));

        // Transpose matrix
        *m = ds_math::Matrix4::CreateScaleMatrix(*v);

        // Get Matrix4 metatable
        luaL_getmetatable(L, "Matrix4");
        // Set it as metatable of new user data (the Matrix4)
        lua_setmetatable(L, -2);
    }

    // user data passed to this method, constructed matrix
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Matrix4CreateFromQuaternion(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Quaternion *q = NULL;

    // Get matrix from Lua
    q = (ds_math::Quaternion *)luaL_checkudata(L, 1, "Quaternion");

    if (q != NULL)
    {
        // Allocate memory for Matrix4
        ds_math::Matrix4 *m =
            (ds_math::Matrix4 *)lua_newuserdata(L, sizeof(ds_math::Matrix4));

        // Transpose matrix
        *m = ds_math::Matrix4::CreateFromQuaternion(*q);

        // Get Matrix4 metatable
        luaL_getmetatable(L, "Matrix4");
        // Set it as metatable of new user data (the Matrix4)
        lua_setmetatable(L, -2);
    }

    // user data passed to this method, constructed matrix
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Matrix4Mul(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2.", n);
    }

    ds_math::Matrix4 *m1 = NULL;
    ds_math::Matrix4 *m2 = NULL;

    // Get two matrices
    m1 = (ds_math::Matrix4 *)luaL_checkudata(L, 1, "Matrix4");
    m2 = (ds_math::Matrix4 *)luaL_checkudata(L, 2, "Matrix4");

    if (m1 != NULL && m2 != NULL)
    {
        // Allocate memory for Matrix4
        ds_math::Matrix4 *mul =
            (ds_math::Matrix4 *)lua_newuserdata(L, sizeof(ds_math::Matrix4));

        // Multiply them
        *mul = (*m1) * (*m2);

        // Get Matrix4 metatable and put on top of stack
        luaL_getmetatable(L, "Matrix4");
        // Set it as metatable of new user data (the Matrix4 result - second
        // from top of stack)
        lua_setmetatable(L, -2);
    }

    // Two Matrix4 arguments, matrix multiplication result
    assert(lua_gettop(L) == 3);

    return 1;
}

static int l_Matrix4GetTranslation(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 1;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", expected);
    }

    ds_math::Matrix4 *matrix = NULL;

    // Get matrix
    matrix = (ds_math::Matrix4 *)luaL_checkudata(L, 1, "Matrix4");

    if (matrix != NULL)
    {
        // Allocate memory for translation vector
        ds_math::Vector3 *translation =
            (ds_math::Vector3 *)lua_newuserdata(L, sizeof(ds_math::Vector3));

        *translation =
            ds_math::Vector3((*matrix)[3].x, (*matrix)[3].y, (*matrix)[3].z);

        // Get Vector3 metatable and put on top of stack
        luaL_getmetatable(L, "Vector3");
        // Set it as metatable of new user data (the Vector3 result - second
        // from top of stack)
        lua_setmetatable(L, -2);
    }

    // Vector3 result
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Vector4Ctor(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 5)
    {
        return luaL_error(L, "Got %d arguments, expected 5.", n);
    }

    // Allocate memory for Vector3
    ds_math::Vector4 *v =
        (ds_math::Vector4 *)lua_newuserdata(L, sizeof(ds_math::Vector4));

    // Get arguments
    ds_math::scalar x = (ds_math::scalar)luaL_checknumber(L, 2);
    ds_math::scalar y = (ds_math::scalar)luaL_checknumber(L, 3);
    ds_math::scalar z = (ds_math::scalar)luaL_checknumber(L, 4);
    ds_math::scalar w = (ds_math::scalar)luaL_checknumber(L, 5);

    *v = ds_math::Vector4(x, y, z, w);

    // Get Vector3 metatable
    luaL_getmetatable(L, "Vector4");
    // Set it as metatable of new user data (the Vector3)
    lua_setmetatable(L, -2);

    // userdata that called this method, x, y, z, w values and Vector4
    // constructed
    assert(lua_gettop(L) == 6);

    return 1;
}

static int l_Vector4New(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 4)
    {
        return luaL_error(L, "Got %d arguments, expected 5.", n);
    }

    // Allocate memory for Vector3
    ds_math::Vector4 *v =
        (ds_math::Vector4 *)lua_newuserdata(L, sizeof(ds_math::Vector4));

    // Get arguments
    ds_math::scalar x = (ds_math::scalar)luaL_checknumber(L, 1);
    ds_math::scalar y = (ds_math::scalar)luaL_checknumber(L, 2);
    ds_math::scalar z = (ds_math::scalar)luaL_checknumber(L, 3);
    ds_math::scalar w = (ds_math::scalar)luaL_checknumber(L, 4);

    *v = ds_math::Vector4(x, y, z, w);

    // Get Vector3 metatable
    luaL_getmetatable(L, "Vector4");
    // Set it as metatable of new user data (the Vector3)
    lua_setmetatable(L, -2);

    //  x, y, z, w values and Vector4 constructed
    assert(lua_gettop(L) == 5);

    return 1;
}

static int l_Vector4ToString(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Vector4 *v = NULL;

    v = (ds_math::Vector4 *)luaL_checkudata(L, 1, "Vector4");

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

static int l_Vector4Magnitude(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Vector4 *v = NULL;

    v = (ds_math::Vector4 *)luaL_checkudata(L, 1, "Vector4");

    if (v != NULL)
    {
        lua_pushnumber(L, ds_math::Vector4::Magnitude(*v));
    }

    // user data that called this method, magnitude
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Vector4Normalize(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Vector4 *v = NULL;

    v = (ds_math::Vector4 *)luaL_checkudata(L, 1, "Vector4");

    if (v != NULL)
    {
        // Allocate memory for Vector4
        ds_math::Vector4 *normalizedV =
            (ds_math::Vector4 *)lua_newuserdata(L, sizeof(ds_math::Vector4));

        *normalizedV = ds_math::Vector4::Normalize(*v);

        // Get Vector4 metatable and put on top of stack
        luaL_getmetatable(L, "Vector4");
        // Set it as metatable of new user data (the Vector4 - second from top
        // on stack)
        lua_setmetatable(L, -2);
    }

    // userdata that called this method, normalized Vector3
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Vector4Invert(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Vector4 *v = NULL;

    v = (ds_math::Vector4 *)luaL_checkudata(L, 1, "Vector4");

    if (v != NULL)
    {
        // Allocate memory for Vector4
        ds_math::Vector4 *invertedV =
            (ds_math::Vector4 *)lua_newuserdata(L, sizeof(ds_math::Vector4));

        *invertedV = ds_math::Vector4::Invert(*v);

        // Get Vector4 metatable and put on top of stack
        luaL_getmetatable(L, "Vector4");
        // Set it as metatable of new user data (the Vector4 - second from top
        // on stack)
        lua_setmetatable(L, -2);
    }

    // userdata that called this method, inverted Vector3
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Vector4UnaryInvert(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2.", n);
    }

    ds_math::Vector4 *v = NULL;

    v = (ds_math::Vector4 *)luaL_checkudata(L, 1, "Vector4");

    if (v != NULL)
    {
        // Allocate memory for Vector4
        ds_math::Vector4 *invertedV =
            (ds_math::Vector4 *)lua_newuserdata(L, sizeof(ds_math::Vector4));

        *invertedV = ds_math::Vector4::Invert(*v);

        // Get Vector4 metatable and put on top of stack
        luaL_getmetatable(L, "Vector4");
        // Set it as metatable of new user data (the Vector4 - second from top
        // on stack)
        lua_setmetatable(L, -2);
    }

    // userdata that called this method * 2, inverted Vector4
    // See: http://lua-users.org/lists/lua-l/2010-10/msg00783.html
    assert(lua_gettop(L) == 3);

    return 1;
}

static int l_Vector4Dot(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2.", n);
    }

    ds_math::Vector4 *v1 = NULL;
    ds_math::Vector4 *v2 = NULL;

    v1 = (ds_math::Vector4 *)luaL_checkudata(L, 1, "Vector4");
    v2 = (ds_math::Vector4 *)luaL_checkudata(L, 2, "Vector4");

    if (v1 != NULL && v2 != NULL)
    {
        lua_pushnumber(L, ds_math::Vector4::Dot(*v1, *v2));
    }

    // Two Vector4 arguments, dot product result
    assert(lua_gettop(L) == 3);

    return 1;
}

static int l_Vector4UnitX(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 0)
    {
        return luaL_error(L, "Got %d arguments, expected 0.", n);
    }

    // Allocate memory for Vector3
    ds_math::Vector4 *unitX =
        (ds_math::Vector4 *)lua_newuserdata(L, sizeof(ds_math::Vector4));

    *unitX = ds_math::Vector4::UnitX;

    // Get Vector4 metatable and put on top of stack
    luaL_getmetatable(L, "Vector4");
    // Set it as metatable of new user data (the Vector4 - second from top
    // on stack)
    lua_setmetatable(L, -2);

    // Unit vector result
    assert(lua_gettop(L) == 1);

    return 1;
}

static int l_Vector4UnitY(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 0)
    {
        return luaL_error(L, "Got %d arguments, expected 0.", n);
    }

    // Allocate memory for Vector3
    ds_math::Vector4 *unitY =
        (ds_math::Vector4 *)lua_newuserdata(L, sizeof(ds_math::Vector4));

    *unitY = ds_math::Vector4::UnitY;

    // Get Vector4 metatable and put on top of stack
    luaL_getmetatable(L, "Vector4");
    // Set it as metatable of new user data (the Vector4 - second from top
    // on stack)
    lua_setmetatable(L, -2);

    // Unit vector result
    assert(lua_gettop(L) == 1);

    return 1;
}

static int l_Vector4UnitZ(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 0)
    {
        return luaL_error(L, "Got %d arguments, expected 0.", n);
    }

    // Allocate memory for Vector3
    ds_math::Vector4 *unitZ =
        (ds_math::Vector4 *)lua_newuserdata(L, sizeof(ds_math::Vector4));

    *unitZ = ds_math::Vector4::UnitZ;

    // Get Vector4 metatable and put on top of stack
    luaL_getmetatable(L, "Vector4");
    // Set it as metatable of new user data (the Vector4 - second from top
    // on stack)
    lua_setmetatable(L, -2);

    // Unit vector result
    assert(lua_gettop(L) == 1);

    return 1;
}

static int l_Vector4UnitW(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 0)
    {
        return luaL_error(L, "Got %d arguments, expected 0.", n);
    }

    // Allocate memory for Vector3
    ds_math::Vector4 *unitW =
        (ds_math::Vector4 *)lua_newuserdata(L, sizeof(ds_math::Vector4));

    *unitW = ds_math::Vector4::UnitW;

    // Get Vector4 metatable and put on top of stack
    luaL_getmetatable(L, "Vector4");
    // Set it as metatable of new user data (the Vector4 - second from top
    // on stack)
    lua_setmetatable(L, -2);

    // Unit vector result
    assert(lua_gettop(L) == 1);

    return 1;
}

static int l_Vector4MatrixTransform(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2", n);
    }

    // Get vector and matrix arguments
    ds_math::Vector4 *v = NULL;
    ds_math::Matrix4 *m = NULL;

    v = (ds_math::Vector4 *)luaL_checkudata(L, 1, "Vector4");
    m = (ds_math::Matrix4 *)luaL_checkudata(L, 2, "Matrix4");

    if (v != NULL && m != NULL)
    {
        // Allocate memory for transformed Vector4
        ds_math::Vector4 *transformedV =
            (ds_math::Vector4 *)lua_newuserdata(L, sizeof(ds_math::Vector4));

        *transformedV = ds_math::Vector4::Transform(*v, *m);

        // Get Vector4 metatable and put on top of stack
        luaL_getmetatable(L, "Vector4");
        // Set it as metatable of new user data (the Vector4 - second from top
        // on stack)
        lua_setmetatable(L, -2);
    }

    // Vector4 and Matrix4 arguments and transformed vector result
    assert(lua_gettop(L) == 3);

    return 1;
}

static int l_Vector4GetX(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Vector4 *v = NULL;

    v = (ds_math::Vector4 *)luaL_checkudata(L, 1, "Vector4");

    if (v != NULL)
    {
        lua_pushnumber(L, v->x);
    }

    // user data that called this method, x member value
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Vector4SetX(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2.", n);
    }

    ds_math::Vector4 *v = NULL;

    v = (ds_math::Vector4 *)luaL_checkudata(L, 1, "Vector4");

    if (v != NULL)
    {
        v->x = (ds_math::scalar)luaL_checknumber(L, 2);
    }

    // user data that called this method, x member value given
    assert(lua_gettop(L) == 2);

    return 0;
}

static int l_Vector4GetY(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Vector4 *v = NULL;

    v = (ds_math::Vector4 *)luaL_checkudata(L, 1, "Vector4");

    if (v != NULL)
    {
        lua_pushnumber(L, v->y);
    }

    // user data that called this method, x member value
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Vector4SetY(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2.", n);
    }

    ds_math::Vector4 *v = NULL;

    v = (ds_math::Vector4 *)luaL_checkudata(L, 1, "Vector4");

    if (v != NULL)
    {
        v->y = (ds_math::scalar)luaL_checknumber(L, 2);
    }

    // user data that called this method, x member value given
    assert(lua_gettop(L) == 2);

    return 0;
}

static int l_Vector4GetZ(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Vector4 *v = NULL;

    v = (ds_math::Vector4 *)luaL_checkudata(L, 1, "Vector4");

    if (v != NULL)
    {
        lua_pushnumber(L, v->z);
    }

    // user data that called this method, x member value
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Vector4SetZ(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2.", n);
    }

    ds_math::Vector4 *v = NULL;

    v = (ds_math::Vector4 *)luaL_checkudata(L, 1, "Vector4");

    if (v != NULL)
    {
        v->z = (ds_math::scalar)luaL_checknumber(L, 2);
    }

    // user data that called this method, x member value given
    assert(lua_gettop(L) == 2);

    return 0;
}

static int l_Vector4GetW(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 1)
    {
        return luaL_error(L, "Got %d arguments, expected 1.", n);
    }

    ds_math::Vector4 *v = NULL;

    v = (ds_math::Vector4 *)luaL_checkudata(L, 1, "Vector4");

    if (v != NULL)
    {
        lua_pushnumber(L, v->w);
    }

    // user data that called this method, x member value
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_Vector4SetW(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2.", n);
    }

    ds_math::Vector4 *v = NULL;

    v = (ds_math::Vector4 *)luaL_checkudata(L, 1, "Vector4");

    if (v != NULL)
    {
        v->w = (ds_math::scalar)luaL_checknumber(L, 2);
    }

    // user data that called this method, x member value given
    assert(lua_gettop(L) == 2);

    return 0;
}

// static int l_Vector3Scale(lua_State *L)
// {

// }

static const luaL_Reg vector3Methods[] = {{"__tostring", l_Vector3ToString},
                                          {"__unm", l_Vector3UnaryInvert},
                                          {"__sub", l_Vector3Subtract},
                                          {"get_x", l_Vector3GetX},
                                          {"set_x", l_Vector3SetX},
                                          {"get_y", l_Vector3GetY},
                                          {"set_y", l_Vector3SetY},
                                          {"get_z", l_Vector3GetZ},
                                          {"set_z", l_Vector3SetZ},
                                          {NULL, NULL}};

static const luaL_Reg vector3Functions[] = {
    {"new", l_Vector3New},
    {"magnitude", l_Vector3Magnitude},
    {"normalize", l_Vector3Normalize},
    {"invert", l_Vector3Invert},
    {"dot", l_Vector3Dot},
    {"cross", l_Vector3Cross},
    {"unit_x", l_Vector3UnitX},
    {"unit_y", l_Vector3UnitY},
    {"unit_z", l_Vector3UnitZ},
    {"transform", l_Vector3MatrixTransform},
    {"get_rotation_from_to", l_Vector3GetRotationFromTo},
    // {"scale", l_Vector3Scale},
    {NULL, NULL},
};

static const luaL_Reg vector3Special[] = {
    {"__call", l_Vector3Ctor}, {NULL, NULL},
};

static const luaL_Reg vector4Methods[] = {{"__tostring", l_Vector4ToString},
                                          {"__unm", l_Vector4UnaryInvert},
                                          {"get_x", l_Vector4GetX},
                                          {"set_x", l_Vector4SetX},
                                          {"get_y", l_Vector4GetY},
                                          {"set_y", l_Vector4SetY},
                                          {"get_z", l_Vector4GetZ},
                                          {"set_z", l_Vector4SetZ},
                                          {"get_w", l_Vector4GetW},
                                          {"set_w", l_Vector4SetW},
                                          {NULL, NULL}};

static const luaL_Reg vector4Functions[] = {
    {"new", l_Vector4New},
    {"magnitude", l_Vector4Magnitude},
    {"normalize", l_Vector4Normalize},
    {"invert", l_Vector4Invert},
    {"dot", l_Vector4Dot},
    {"unit_x", l_Vector4UnitX},
    {"unit_y", l_Vector4UnitY},
    {"unit_z", l_Vector4UnitZ},
    {"unit_w", l_Vector4UnitW},
    {"transform", l_Vector4MatrixTransform},
    {NULL, NULL}};

static const luaL_Reg vector4Special[] = {
    {"__call", l_Vector4Ctor}, {NULL, NULL},
};
static const luaL_Reg quaternionMethods[] = {
    {"__tostring", l_QuaternionToString},
    {"__mul", l_QuaternionMul},
    {NULL, NULL}};

static const luaL_Reg quaternionFunctions[] = {
    {"new", l_QuaternionNew},
    {"magnitude", l_QuaternionMagnitude},
    {"normalize", l_QuaternionNormalize},
    {"invert", l_QuaternionInvert},
    {"dot", l_QuaternionDot},
    {"create_from_axis_angle", l_QuaternionFromAxisAngle},
    {NULL, NULL}};

static const luaL_Reg quaternionSpecial[] = {{"__call", l_QuaternionCtor},
                                             {NULL, NULL}};

static const luaL_Reg matrix4Methods[] = {
    {"__tostring", l_Matrix4ToString},
    {"__mul", l_Matrix4Mul},
    {"get_translation", l_Matrix4GetTranslation},
    {NULL, NULL}};

static const luaL_Reg matrix4Functions[] = {
    {"transpose", l_Matrix4Transpose},
    {"inverse", l_Matrix4Invert},
    {"create_translation_matrix", l_Matrix4CreateTranslation},
    {"create_scale_matrix", l_Matrix4CreateScale},
    {"create_from_quaternion", l_Matrix4CreateFromQuaternion},
    {NULL, NULL}};

static const luaL_Reg matrix4Special[] = {{"__call", l_Matrix4Ctor},
                                          {NULL, NULL}};

void LoadMathAPI(LuaEnvironment &luaEnv)
{
    luaEnv.RegisterClass("Vector3", vector3Methods, vector3Functions,
                         vector3Special);
    luaEnv.RegisterClass("Vector4", vector4Methods, vector4Functions,
                         vector4Special);
    luaEnv.RegisterClass("Quaternion", quaternionMethods, quaternionFunctions,
                         quaternionSpecial);
    luaEnv.RegisterClass("Matrix4", matrix4Methods, matrix4Functions,
                         matrix4Special);
}
}
