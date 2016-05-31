#include "engine/entity/Entity.h"
#include "engine/common/StringIntern.h"
#include "engine/system/script/Script.h"

namespace ds_lua
{
static int l_SpawnPrefab(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    if (n != 2)
    {
        return luaL_error(L, "Got %d arguments, expected 2.", n);
    }

    const char *prefabFile = luaL_checklstring(L, 1, NULL);

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
        // Get vector position from argument
        ds_math::Vector3 *v = NULL;

        v = (ds_math::Vector3 *)luaL_checkudata(L, 2, "Vector3");

        if (v != NULL)
        {
            ds::Script *p = (ds::Script *)lua_touserdata(L, -1);

            assert(p != NULL && "spawnPrefab: Tried to deference userdata "
                                "pointer which was null");

            // Pop script system pointer
            lua_pop(L, 1);

            // Allocate space for entity handle
            ds::Entity *entity =
                (ds::Entity *)lua_newuserdata(L, sizeof(ds::Entity));

            *entity = p->SpawnPrefab(prefabFile, *v);

            // Get Entity metatable
            luaL_getmetatable(L, "Entity");
            // Set it as metatable of new user data
            lua_setmetatable(L, -2);
        }
    }

    // Ensure stack is clean
    assert(lua_gettop(L) == 3);

    return 1;
}

static int l_IsNextMessage(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 0;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push script system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        lua_pushboolean(L, scriptPtr->IsNextScriptMessage());
    }

    // Return boolean
    assert(lua_gettop(L) == 1);

    return 1;
}

static int l_GetNextMessage(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 0;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push script system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        // Get message
        ds_msg::MessageStream msg = scriptPtr->GetNextScriptMessage();

        // Push a new table onto the stack to hold the message
        lua_newtable(L);

        if (msg.AvailableBytes() > 0)
        {
            // Extract header
            ds_msg::MessageHeader header;
            msg >> header;

            // Extract payload and create table
            switch (header.type)
            {
            case ds_msg::MessageType::ScriptInterpret:
                ds_msg::ScriptInterpret scriptMsg;
                msg >> scriptMsg;

                // Create payload table
                lua_pushliteral(L, "script_interpret");
                lua_setfield(L, -2, "type"); // table.type = script_interpret

                lua_pushstring(L, ds::StringIntern::Instance()
                                      .GetString(scriptMsg.stringId)
                                      .c_str());
                lua_setfield(L, -2, "script"); // table.script = script message

                break;
            case ds_msg::MessageType::ButtonFired:
            {
                ds_msg::ButtonFired buttonFiredMsg;
                msg >> buttonFiredMsg;

                // Create payload table
                lua_pushliteral(L, "button_fired");
                lua_setfield(L, -2, "type"); // table.type = button_fired

                // Push entity to stack
                ds::Entity *entity =
                    (ds::Entity *)lua_newuserdata(L, sizeof(ds::Entity));
                *entity = buttonFiredMsg.entity;

                // Get Entity metatable
                luaL_getmetatable(L, "Entity");
                // Set it as metatable of new user data
                lua_setmetatable(L, -2);

                // Set entity field
                lua_setfield(L, -2, "entity"); // table.entity = entity

                break;
            }
            case ds_msg::MessageType::PhysicsCollision:
            {
                ds_msg::PhysicsCollision collisionMsg;
                msg >> collisionMsg;

                // Create payload table
                lua_pushliteral(L, "physics_collision");
                lua_setfield(L, -2, "type"); // table.type = physics_collision

                // Create entityA and push to stack
                ds::Entity *entityA =
                    (ds::Entity *)lua_newuserdata(L, sizeof(ds::Entity));
                *entityA = collisionMsg.entityA;

                // Get Entity metatable
                luaL_getmetatable(L, "Entity");
                // Set it as metatable of new user data
                lua_setmetatable(L, -2);

                // Set entityA field
                lua_setfield(L, -2, "entityA"); // table.entityA = entityA

                // Create entityB and push to stack
                ds::Entity *entityB =
                    (ds::Entity *)lua_newuserdata(L, sizeof(ds::Entity));
                *entityB = collisionMsg.entityB;

                // Get Entity metatable
                luaL_getmetatable(L, "Entity");
                // Set it as metatable of new user data
                lua_setmetatable(L, -2);

                // Set entityB field
                lua_setfield(L, -2, "entityB"); // table.entityB = entityB

                // Create Vector3 and push to stack
                ds_math::Vector3 *pointA = (ds_math::Vector3 *)lua_newuserdata(
                    L, sizeof(ds_math::Vector3));
                *pointA = collisionMsg.pointWorldOnA;

                // Get Vector3 metatable
                luaL_getmetatable(L, "Vector3");
                // Set it as metatable of new user data
                lua_setmetatable(L, -2);

                // Set pointWorldOnA field
                lua_setfield(L, -2,
                             "pointWorldOnA"); // table.pointWorldOnA = pointA

                // Create Vector3 and push to stack
                ds_math::Vector3 *pointB = (ds_math::Vector3 *)lua_newuserdata(
                    L, sizeof(ds_math::Vector3));
                *pointB = collisionMsg.pointWorldOnB;

                // Get Vector3 metatable
                luaL_getmetatable(L, "Vector3");
                // Set it as metatable of new user data
                lua_setmetatable(L, -2);

                // Set pointWorldOnB field
                lua_setfield(L, -2,
                             "pointWorldOnB"); // table.pointWorldOnB = pointB

                // Create Vector3 and push to stack
                ds_math::Vector3 *normalB = (ds_math::Vector3 *)lua_newuserdata(
                    L, sizeof(ds_math::Vector3));
                *normalB = collisionMsg.normalWorldOnB;

                // Get Vector3 metatable
                luaL_getmetatable(L, "Vector3");
                // Set it as metatable of new user data
                lua_setmetatable(L, -2);

                // Set normalWorldOnB field
                lua_setfield(
                    L, -2,
                    "normalWorldOnB"); // table.normalWorldOnB = normalB

                break;
            }
            default:
                assert(false && "l_GetNextMessage should handle all received "
                                "message types!");
                break;
            }
        }
    }

    // Return table
    assert(lua_gettop(L) == 1);

    return 1;
}

static int l_GetWorldTransform(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 1;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push script system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        ds::Entity *entity = NULL;

        entity = (ds::Entity *)luaL_checkudata(L, 1, "Entity");

        if (entity != NULL)
        {
            // Allocate memory for Matrix4
            ds_math::Matrix4 *worldTransform =
                (ds_math::Matrix4 *)lua_newuserdata(L,
                                                    sizeof(ds_math::Matrix4));

            // Get world transform
            *worldTransform = scriptPtr->GetWorldTransform(*entity);

            // Get Matrix4 metatable and put on top of stack
            luaL_getmetatable(L, "Matrix4");
            // Set it as metatable of new user data (the Matrix4 result - second
            // from top of stack)
            lua_setmetatable(L, -2);
        }
    }

    // Entity argument, matrix4 result
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_GetLocalTransform(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 1;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push script system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        ds::Entity *entity = NULL;

        entity = (ds::Entity *)luaL_checkudata(L, 1, "Entity");

        if (entity != NULL)
        {
            // Allocate memory for Matrix4
            ds_math::Matrix4 *localTransform =
                (ds_math::Matrix4 *)lua_newuserdata(L,
                                                    sizeof(ds_math::Matrix4));

            // Get world transform
            *localTransform = scriptPtr->GetLocalTransform(*entity);

            // Get Matrix4 metatable and put on top of stack
            luaL_getmetatable(L, "Matrix4");
            // Set it as metatable of new user data (the Matrix4 result - second
            // from top of stack)
            lua_setmetatable(L, -2);
        }
        else
        {
            return luaL_error(L, "Given entity is not valid.");
        }
    }

    // Entity argument, matrix4 result
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_SetEntityAnimationIndex(lua_State *L)
{
    int n = lua_gettop(L);
    int expected = 2;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push script system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        ds::Entity *entity = NULL;

        entity = (ds::Entity *)luaL_checkudata(L, 1, "Entity");

        ds_math::scalar animationIndex =
            (ds_math::scalar)luaL_checknumber(L, 2);

        if (entity != NULL)
        {
            scriptPtr->SetAnimationIndex(*entity, animationIndex);
        }
    }

    // Entity and animation index passed in
    assert(lua_gettop(L) == 2);

    return 0;
}

static int l_SetSkyboxMaterial(lua_State *L)
{
    int n = lua_gettop(L);
    int expected = 1;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push render system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our input system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        const char *skyboxMaterialPath = NULL;

        skyboxMaterialPath = luaL_checkstring(L, 1);

        if (skyboxMaterialPath != NULL)
        {
            scriptPtr->SetSkyboxMaterial(skyboxMaterialPath);
        }
    }

    // String passed in
    assert(lua_gettop(L) == 1);

    return 0;
}

static int l_CreateGUIPanel(lua_State *L)
{
    int n = lua_gettop(L);
    int expected = 5;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push render system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our input system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        const char *materialPath = NULL;

        float startX = (float)luaL_checknumber(L, 1);
        float startY = (float)luaL_checknumber(L, 2);
        float endX = (float)luaL_checknumber(L, 3);
        float endY = (float)luaL_checknumber(L, 4);
        materialPath = luaL_checkstring(L, 5);

        if (materialPath != NULL)
        {
            ds::Entity *entity =
                (ds::Entity *)lua_newuserdata(L, sizeof(ds::Entity));

            *entity = scriptPtr->CreateGUIPanel(startX, startY, endX, endY,
                                                materialPath);

            // Get Entity metatable
            luaL_getmetatable(L, "Entity");
            // Set it as metatable of new user data
            lua_setmetatable(L, -2);
        }
    }

    // startX, startY, endX, endY, material path arguments and entity created
    assert(lua_gettop(L) == 6);

    return 1;
}

static int l_CreateGUIButton(lua_State *L)
{
    int n = lua_gettop(L);
    int expected = 7;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push render system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our input system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        const char *defaultMaterialPath = NULL;
        const char *pressedMaterialPath = NULL;
        const char *hoverMaterialPath = NULL;

        float startX = (float)luaL_checknumber(L, 1);
        float startY = (float)luaL_checknumber(L, 2);
        float endX = (float)luaL_checknumber(L, 3);
        float endY = (float)luaL_checknumber(L, 4);
        defaultMaterialPath = luaL_checkstring(L, 5);
        pressedMaterialPath = luaL_checkstring(L, 6);
        hoverMaterialPath = luaL_checkstring(L, 7);

        if (defaultMaterialPath != NULL && pressedMaterialPath != NULL &&
            hoverMaterialPath != NULL)
        {
            ds::Entity *entity =
                (ds::Entity *)lua_newuserdata(L, sizeof(ds::Entity));

            *entity = scriptPtr->CreateGUIButton(
                startX, startY, endX, endY, defaultMaterialPath,
                pressedMaterialPath, hoverMaterialPath);

            // Get Entity metatable
            luaL_getmetatable(L, "Entity");
            // Set it as metatable of new user data
            lua_setmetatable(L, -2);
        }
    }

    // startX, startY, endX, endY, material path arguments (default, pressed,
    // hover) and entity created
    assert(lua_gettop(L) == 8);

    // Return entity created
    return 1;
}

static int l_GetLocalTranslation(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 1;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push script system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        ds::Entity *entity = NULL;

        entity = (ds::Entity *)luaL_checkudata(L, 1, "Entity");

        if (entity != NULL)
        {
            // Allocate memory for Vector3
            ds_math::Vector3 *localTranslation =
                (ds_math::Vector3 *)lua_newuserdata(L,
                                                    sizeof(ds_math::Vector3));

            // Get world transform
            *localTranslation = scriptPtr->GetLocalTranslation(*entity);

            // Get Matrix4 metatable and put on top of stack
            luaL_getmetatable(L, "Vector3");
            // Set it as metatable of new user data (the Matrix4 result - second
            // from top of stack)
            lua_setmetatable(L, -2);
        }
        else
        {
            return luaL_error(L, "Given entity is not valid.");
        }
    }

    // Entity argument, Vector3 result
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_GetWorldTranslation(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 1;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push script system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        ds::Entity *entity = NULL;

        entity = (ds::Entity *)luaL_checkudata(L, 1, "Entity");

        if (entity != NULL)
        {
            // Allocate memory for Vector3
            ds_math::Vector3 *worldTranslation =
                (ds_math::Vector3 *)lua_newuserdata(L,
                                                    sizeof(ds_math::Vector3));

            // Get world transform
            *worldTranslation = scriptPtr->GetWorldTranslation(*entity);

            // Get Vector3 metatable and put on top of stack
            luaL_getmetatable(L, "Vector3");
            // Set it as metatable of new user data (the Vector3 result - second
            // from top of stack)
            lua_setmetatable(L, -2);
        }
        else
        {
            return luaL_error(L, "Given entity is not valid.");
        }
    }

    // Entity argument, Vector3 result
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_GetLocalScale(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 1;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push script system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        ds::Entity *entity = NULL;

        entity = (ds::Entity *)luaL_checkudata(L, 1, "Entity");

        if (entity != NULL)
        {
            // Allocate memory for Vector3
            ds_math::Vector3 *localScale = (ds_math::Vector3 *)lua_newuserdata(
                L, sizeof(ds_math::Vector3));

            // Get world scale
            *localScale = scriptPtr->GetLocalScale(*entity);

            // Get Vector3 metatable and put on top of stack
            luaL_getmetatable(L, "Vector3");
            // Set it as metatable of new user data (the Vector3 result - second
            // from top of stack)
            lua_setmetatable(L, -2);
        }
        else
        {
            return luaL_error(L, "Given entity is not valid.");
        }
    }

    // Entity argument, Vector3 result
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_GetWorldScale(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 1;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push script system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        ds::Entity *entity = NULL;

        entity = (ds::Entity *)luaL_checkudata(L, 1, "Entity");

        if (entity != NULL)
        {
            // Allocate memory for Vector3
            ds_math::Vector3 *worldScale = (ds_math::Vector3 *)lua_newuserdata(
                L, sizeof(ds_math::Vector3));

            // Get world scale
            *worldScale = scriptPtr->GetWorldScale(*entity);

            // Get Vector3 metatable and put on top of stack
            luaL_getmetatable(L, "Vector3");
            // Set it as metatable of new user data (the Vector3 result - second
            // from top of stack)
            lua_setmetatable(L, -2);
        }
        else
        {
            return luaL_error(L, "Given entity is not valid.");
        }
    }

    // Entity argument, Vector3 result
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_GetLocalOrientation(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 1;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push script system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        ds::Entity *entity = NULL;

        entity = (ds::Entity *)luaL_checkudata(L, 1, "Entity");

        if (entity != NULL)
        {
            // Allocate memory for Quaternion
            ds_math::Quaternion *localScale =
                (ds_math::Quaternion *)lua_newuserdata(
                    L, sizeof(ds_math::Quaternion));

            // Get local orientation
            *localScale = scriptPtr->GetLocalOrientation(*entity);

            // Get Quaternion metatable and put on top of stack
            luaL_getmetatable(L, "Quaternion");
            // Set it as metatable of new user data (the Quaternion result -
            // second
            // from top of stack)
            lua_setmetatable(L, -2);
        }
        else
        {
            return luaL_error(L, "Given entity is not valid.");
        }
    }

    // Entity argument, Vector3 result
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_SetLocalTranslation(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 2;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push script system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        ds::Entity *entity = NULL;
        ds_math::Vector3 *translation = NULL;

        entity = (ds::Entity *)luaL_checkudata(L, 1, "Entity");
        translation = (ds_math::Vector3 *)luaL_checkudata(L, 2, "Vector3");

        if (entity != NULL && translation != NULL)
        {
            // Set local translation
            scriptPtr->SetLocalTranslation(*entity, *translation);
        }
        else
        {
            return luaL_error(L, "Given entity or translation is not valid.");
        }
    }

    // Entity argument, translation result
    assert(lua_gettop(L) == 2);

    return 0;
}

static int l_SetLocalScale(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 2;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push script system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        ds::Entity *entity = NULL;
        ds_math::Vector3 *scale = NULL;

        entity = (ds::Entity *)luaL_checkudata(L, 1, "Entity");
        scale = (ds_math::Vector3 *)luaL_checkudata(L, 2, "Vector3");

        if (entity != NULL && scale != NULL)
        {
            // Set local scale
            scriptPtr->SetLocalScale(*entity, *scale);
        }
        else
        {
            return luaL_error(L, "Given entity or scale is not valid.");
        }
    }

    // Entity argument, scale result
    assert(lua_gettop(L) == 2);

    return 0;
}

static int l_SetLocalOrientation(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 2;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push script system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        ds::Entity *entity = NULL;
        ds_math::Quaternion *orientation = NULL;

        entity = (ds::Entity *)luaL_checkudata(L, 1, "Entity");
        orientation =
            (ds_math::Quaternion *)luaL_checkudata(L, 2, "Quaternion");

        if (entity != NULL && orientation != NULL)
        {
            // Set local orientation
            scriptPtr->SetLocalOrientation(*entity, *orientation);
        }
        else
        {
            return luaL_error(L, "Given entity or orientation is not valid.");
        }
    }

    // Entity argument, quaternion result
    assert(lua_gettop(L) == 2);

    return 0;
}

static int l_GetWorldOrientation(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 1;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push script system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        ds::Entity *entity = NULL;

        entity = (ds::Entity *)luaL_checkudata(L, 1, "Entity");

        if (entity != NULL)
        {
            // Allocate memory for Quaternion
            ds_math::Quaternion *worldScale =
                (ds_math::Quaternion *)lua_newuserdata(
                    L, sizeof(ds_math::Quaternion));

            // Get world orientation
            *worldScale = scriptPtr->GetWorldOrientation(*entity);

            // Get Quaternion metatable and put on top of stack
            luaL_getmetatable(L, "Quaternion");
            // Set it as metatable of new user data (the Quaternion result -
            // second
            // from top of stack)
            lua_setmetatable(L, -2);
        }
        else
        {
            return luaL_error(L, "Given entity is not valid.");
        }
    }

    // Entity argument, Vector3 result
    assert(lua_gettop(L) == 2);

    return 1;
}

static int l_DestroyEntity(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 1;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push script system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        ds::Entity *entity = NULL;

        entity = (ds::Entity *)luaL_checkudata(L, 1, "Entity");

        if (entity != NULL)
        {
            // Send destroy entity message
            scriptPtr->DestroyEntity(*entity);
        }
    }

    // Entity argument
    assert(lua_gettop(L) == 1);

    return 0;
}

static int l_SetMaterialParameter(lua_State *L)
{
    int n = lua_gettop(L);
    int expected = 4;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push render system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our input system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        const char *materialResourceFilePath = NULL;
        const char *materialParameterName = NULL;
        const char *materialParameterType = NULL;

        materialResourceFilePath = luaL_checkstring(L, 1);
        materialParameterName = luaL_checkstring(L, 2);
        materialParameterType = luaL_checkstring(L, 3);

        if (materialResourceFilePath != NULL && materialParameterName != NULL &&
            materialParameterType != NULL)
        {
            std::string type = std::string(materialParameterType);
            if (type == "float")
            {
                // Get data
                ds_math::scalar data = (ds_math::scalar)luaL_checknumber(L, 4);

                scriptPtr->SetMaterialParameterFloat(materialResourceFilePath,
                                                     materialParameterName,
                                                     (float)data);
            }
            else if (type == "int")
            {
                ds_math::scalar data = (ds_math::scalar)luaL_checknumber(L, 4);

                scriptPtr->SetMaterialParameterInt(
                    materialResourceFilePath, materialParameterName, (int)data);
            }
            else if (type == "mat4")
            {
                ds_math::Matrix4 *data = NULL;
                data = (ds_math::Matrix4 *)luaL_checkudata(L, 4, "Matrix4");

                if (data != NULL)
                {
                    scriptPtr->SetMaterialParameterMatrix4(
                        materialResourceFilePath, materialParameterName, *data);
                }
            }
            else if (type == "vec3")
            {
                ds_math::Vector3 *data = NULL;
                data = (ds_math::Vector3 *)luaL_checkudata(L, 4, "Vector3");

                if (data != NULL)
                {
                    scriptPtr->SetMaterialParameterVector3(
                        materialResourceFilePath, materialParameterName, *data);
                }
            }
            else if (type == "vec4")
            {
                ds_math::Vector4 *data = NULL;
                data = (ds_math::Vector4 *)luaL_checkudata(L, 4, "Vector4");

                if (data != NULL)
                {
                    scriptPtr->SetMaterialParameterVector4(
                        materialResourceFilePath, materialParameterName, *data);
                }
            }
        }
    }

    // Four strings passed in
    assert(lua_gettop(L) == 4);

    return 0;
}

static int l_SetMouseLock(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 1;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push script system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        // Check for boolean argument
        if (lua_isboolean(L, 1) == 1)
        {
            bool enableMouseLock = (bool)lua_toboolean(L, 1);

            scriptPtr->SetMouseLock(enableMouseLock);
        }
        else
        {
            return luaL_error(L, "Expected boolean argument.");
        }
    }

    // Boolean argument 
    assert(lua_gettop(L) == 1);

    return 0;
}

static int l_Quit(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 0;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push script system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        scriptPtr->Quit();
    }

    assert(lua_gettop(L) == 0);

    return 0;
}

static int l_Pause(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 1;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push script system pointer onto stack
    lua_getglobal(L, "__Script");

    // If first item on stack isn't user data (our script system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::Script *scriptPtr = (ds::Script *)lua_touserdata(L, -1);
        assert(scriptPtr != NULL);

        // Pop user data off stack now that we are done with it
        lua_pop(L, 1);

        // Check for boolean argument
        if (lua_isboolean(L, 1) == 1)
        {
            bool shouldPause = (bool)lua_toboolean(L, 1);

            scriptPtr->SetPause(shouldPause);
        }
        else
        {
            return luaL_error(L, "Expected boolean argument.");
        }
    }

    // Boolean argument 
    assert(lua_gettop(L) == 1);

    return 0;
}

ds::ScriptBindingSet LoadScriptBindings()
{
    ds::ScriptBindingSet scriptBindings;
    scriptBindings.AddFunction("is_next_message", l_IsNextMessage);
    scriptBindings.AddFunction("get_next_message", l_GetNextMessage);
    scriptBindings.AddFunction("spawn_prefab", l_SpawnPrefab);
    scriptBindings.AddFunction("get_world_transform", l_GetWorldTransform);
    scriptBindings.AddFunction("get_local_transform", l_GetLocalTransform);
    scriptBindings.AddFunction("set_entity_animation_index",
                               l_SetEntityAnimationIndex);
    scriptBindings.AddFunction("set_skybox_material", l_SetSkyboxMaterial);
    scriptBindings.AddFunction("create_gui_panel", l_CreateGUIPanel);
    scriptBindings.AddFunction("create_gui_button", l_CreateGUIButton);
    scriptBindings.AddFunction("get_local_translation", l_GetLocalTranslation);
    scriptBindings.AddFunction("set_local_translation", l_SetLocalTranslation);
    scriptBindings.AddFunction("get_world_translation", l_GetWorldTranslation);
    scriptBindings.AddFunction("get_local_scale", l_GetLocalScale);
    scriptBindings.AddFunction("set_local_scale", l_SetLocalScale);
    scriptBindings.AddFunction("get_world_scale", l_GetWorldScale);
    scriptBindings.AddFunction("get_local_orientation", l_GetLocalOrientation);
    scriptBindings.AddFunction("set_local_orientation", l_SetLocalOrientation);
    scriptBindings.AddFunction("get_world_orientation", l_GetWorldOrientation);
    scriptBindings.AddFunction("destroy_entity", l_DestroyEntity);
    scriptBindings.AddFunction("set_material_parameter",
                               l_SetMaterialParameter);
    scriptBindings.AddFunction("set_mouse_lock", l_SetMouseLock);
    scriptBindings.AddFunction("quit", l_Quit);
    scriptBindings.AddFunction("set_pause", l_Pause);

    return scriptBindings;
}
}
