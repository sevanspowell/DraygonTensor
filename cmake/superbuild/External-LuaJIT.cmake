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
  ExternalProject_Add(LuaJIT
    URL "https://www.dropbox.com/s/e81864ppbh5xrle/LuaJIT-2.0.zip"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_DIR "${LUAJIT_DIR}"
    INSTALL_COMMAND 
        ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/lib <INSTALL_DIR>/lib 
        COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/include <INSTALL_DIR>/include
        COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/bin <INSTALL_DIR>/bin
    CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    )
endif (WIN32)
