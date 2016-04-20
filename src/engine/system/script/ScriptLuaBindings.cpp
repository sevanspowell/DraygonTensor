#include "engine/common/StringIntern.h"
#include "engine/system/script/Script.h"

namespace ds_lua
{
int l_IsNextMessage(lua_State *L)
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

        lua_pushboolean(L, scriptPtr->IsNextScriptMessage());
    }

    // Return boolean
    assert(lua_gettop(L) == 1);

    return 1;
}

int l_GetNextMessage(lua_State *L)
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
}
