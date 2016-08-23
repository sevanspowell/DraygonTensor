# Locate LuaBridge headers
# This module defines
#  LUABRIDGE_FOUND, 
#  LUABRIDGE_INCLUDE_DIR, where to find LuaBridge.h 
#
# Hints: LUABRIDGE_ROOT_DIR.

FIND_PATH(LUABRIDGE_INCLUDE_DIR LuaBridge.h
  HINTS
  ${LUABRIDGE_ROOT_DIR}
  PATH_SUFFIXES Source/LuaBridge/ Source/ LuaBridge/
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
)

INCLUDE(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LUAJIT_FOUND to TRUE if 
# all listed variables are TRUE

FIND_PACKAGE_HANDLE_STANDARD_ARGS(LuaBridge DEFAULT_MSG LUABRIDGE_INCLUDE_DIR)

MARK_AS_ADVANCED(LUABRIDGE_INCLUDE_DIR)
