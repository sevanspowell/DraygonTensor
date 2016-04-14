# Get and build STB
ExternalProject_Add(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_DIR "${STB_BASE_DIR}"
  INSTALL_COMMAND
    #${CMAKE_COMMAND} -E echo <SOURCE_DIR>
    ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/../stb <INSTALL_DIR>/../stb
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
  )
