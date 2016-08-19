# Get and build LuaBridge
ExternalProject_Add(
  LuaBridge
  GIT_REPOSITORY https://github.com/samdelion/LuaBridge.git
  GIT_TAG master
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_DIR ${LUABRIDGE_ROOT_DIR}
  INSTALL_COMMAND 
      ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/ <INSTALL_DIR>/
  )
