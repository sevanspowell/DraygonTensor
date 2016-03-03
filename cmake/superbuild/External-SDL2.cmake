# Get and build SDL2

if (WIN32)
  ExternalProject_Add(
    SDL2
    URL "https://www.libsdl.org/release/SDL2-devel-2.0.4-VC.zip"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_DIR "${SDL2DIR}"
    INSTALL_COMMAND 
        ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/lib <INSTALL_DIR>/lib 
        COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/include <INSTALL_DIR>/include/SDL2
    CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    )
endif(WIN32)
