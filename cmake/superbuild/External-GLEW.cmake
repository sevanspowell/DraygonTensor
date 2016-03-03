# Get and build GLEW

if (WIN32)
  ExternalProject_Add(
    GLEW 
    URL "http://internode.dl.sourceforge.net/project/glew/glew/1.13.0/glew-1.13.0-win32.zip"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_DIR "${GLEW_DIR}"
    INSTALL_COMMAND 
        ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/lib/Release/x64 <INSTALL_DIR>/lib 
        COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/include <INSTALL_DIR>/include
        COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/bin/Release/x64 <INSTALL_DIR>/bin
    CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    )
endif(WIN32)

# If Unix-style (Linux and MacOSX)
if (UNIX)
  ExternalProject_Add(GLEW
    URL "https://sourceforge.net/projects/glew/files/glew/1.13.0/glew-1.13.0.tgz/download"
    CONFIGURE_COMMAND
      ${CMAKE_COMMAND} -E chdir <SOURCE_DIR>/build ${CMAKE_COMMAND} ./cmake 
    BINARY_DIR <SOURCE_DIR>/build
    BUILD_COMMAND
      ${CMAKE_COMMAND} -E chdir <SOURCE_DIR>/build make -j24 
    INSTALL_DIR "${GLEW_DIR}"
    INSTALL_COMMAND
      ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/build/lib <INSTALL_DIR>/lib
      COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/include <INSTALL_DIR>/include
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    )
endif (UNIX)
