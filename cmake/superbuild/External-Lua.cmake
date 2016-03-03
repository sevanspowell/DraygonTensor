# Get and build Lua

if (WIN32)
  ExternalProject_Add(
	  Lua
	  URL "http://tenet.dl.sourceforge.net/project/luabinaries/5.3.2/Windows%20Libraries/Dynamic/lua-5.3.2_Win32_dllw4_lib.zip"
	  CONFIGURE_COMMAND ""
	  BUILD_COMMAND ""
    INSTALL_DIR "${LUA_DIR}"
    INSTALL_COMMAND  ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR> <INSTALL_DIR>
    CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
	  )
endif	(WIN32)

# If Unix-style (Linux and MacOSX)
if (UNIX)
  if (APPLE)
    # If MacOSX
    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
      ExternalProject_Add(
        Lua
        URL http://www.lua.org/ftp/lua-5.3.2.tar.gz
        URL_MD5 33278c2ab5ee3c1a875be8d55c1ca2a1
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make macosx
        BUILD_IN_SOURCE 1
        INSTALL_COMMAND make INSTALL_TOP=${LUA_DIR} install 
        )  
    # If __APPLE__ is defined, but is not MacOSX, use Linux config
    else()
      ExternalProject_Add(
        Lua
        URL http://www.lua.org/ftp/lua-5.3.2.tar.gz
        URL_MD5 33278c2ab5ee3c1a875be8d55c1ca2a1
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make linux 
        BUILD_IN_SOURCE 1
        INSTALL_COMMAND make INSTALL_TOP=${LUA_DIR} install 
        )  
    endif()
  # Not __APPLE__, use Linux config
  else (APPLE)
      ExternalProject_Add(
        Lua
        URL http://www.lua.org/ftp/lua-5.3.2.tar.gz
        URL_MD5 33278c2ab5ee3c1a875be8d55c1ca2a1
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make linux 
        BUILD_IN_SOURCE 1
        INSTALL_COMMAND make INSTALL_TOP=${LUA_DIR} install 
        )  
  endif (APPLE)
endif(UNIX)
