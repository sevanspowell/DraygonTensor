#pragma once

extern "C" {
#define LUA_COMPAT_5_1
#define LUA_COMPAT_5_2

#ifdef DS_LUAJIT_WORKAROUND
#	include "luajit-2.0/luaconf.h"
#	include "luajit-2.0/lualib.h"
#	include "luajit-2.0/lauxlib.h"
#	include "luajit-2.0/lua.h"
#else
#	include "luaconf.h"
#	include "lualib.h"
#	include "lauxlib.h"
#	include "lua.h"
#endif
}

#ifndef LUALIB_API
#	define LUALIB_API      LUA_API
#endif

#ifndef LUAMOD_API
#	define LUAMOD_API      LUALIB_API
#endif 

#include "LuaBridge.h"
