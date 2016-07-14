# Get and build LuaJIT

if (UNIX)
  ExternalProject_Add(LuaJIT
    GIT_REPOSITORY http://luajit.org/git/luajit-2.0.git
    GIT_TAG master
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make PREFIX=${LUAJIT_DIR}
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND make install PREFIX=${LUAJIT_DIR}
    )
endif (UNIX)

if (WIN32)
endif (WIN32)
