include(ExternalProject)

if (MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif (MSVC)

set(DRUNKEN_SAILOR_ENGINE_DEPENDENCIES)

find_package(Git REQUIRED)

# Try to find GTest package
set(GTEST_ROOT ${CMAKE_SOURCE_DIR}/../../external/GoogleTest)
find_package(GTest)
# If not found, download
if (NOT GTEST_FOUND)
	message("Will download Google Test...")
	include(${CMAKE_SOURCE_DIR}/External-GoogleTest.cmake)
	list(APPEND DRUNKEN_SAILOR_ENGINE_DEPENDENCIES googletest)
else (NOT GTEST_FOUND)
	set(GTEST_ROOT ${GTEST_INCLUDE_DIRS}/..)
endif (NOT GTEST_FOUND)

# Try to find Lua package
# Look in LUAJIT_DIR
set(LUAJIT_DIR ${CMAKE_SOURCE_DIR}/../../external/LuaJIT-2.0)
find_package(LuaJIT)
# If not found, download
if (NOT LUAJIT_FOUND)
	message("Will download LuaJIT...")
	include(${CMAKE_SOURCE_DIR}/External-LuaJIT.cmake)
	list(APPEND DRUNKEN_SAILOR_ENGINE_DEPENDENCIES LuaJIT)
else (NOT LUAJIT_FOUND)
	set(LUA_DIR ${LUAJIT_INCLUDE_DIR}/../..)
endif (NOT LUAJIT_FOUND)

# Try to find SDL2 package
set(SDL2DIR ${CMAKE_SOURCE_DIR}/../../external/SDL2-2.0.4)
if (WIN32)
  find_package(SDL2)
else (WIN32)
  find_package(SDL2 REQUIRED)
endif (WIN32)
# If not found, download
if (NOT SDL2_FOUND)
  message("Will download SDL2...")
  include(${CMAKE_SOURCE_DIR}/External-SDL2.cmake)
  list(APPEND DRUNKEN_SAILOR_ENGINE_DEPENDENCIES SDL2)
else (NOT SDL2_FOUND)
  set(SDL2_DIR ${SDL2_INCLUDE_DIR}/../)
endif (NOT SDL2_FOUND)

# Try to find RapidJSON package
set(RAPIDJSON_INCLUDEDIR ${CMAKE_SOURCE_DIR}/../../external/rapidjson/include)
find_package(rapidjson)
# If not found, download
if (NOT RAPIDJSON_FOUND)
  message("Will download rapidjson...")
  include(${CMAKE_SOURCE_DIR}/External-RapidJSON.cmake)
  list(APPEND DRUNKEN_SAILOR_ENGINE_DEPENDENCIES rapidjson)
else (NOT RAPIDJSON_FOUND)
  set(RAPIDJSON_INCLUDEDIR ${RAPIDJSON_INCLUDE_DIRS})
endif (NOT RAPIDJSON_FOUND)

# Try to find GLEW package
set(GLEW_DIR ${CMAKE_SOURCE_DIR}/../../external/GLEW)
find_package(GLEW)
# If not found, download
if (NOT GLEW_FOUND)
  message("Will download GLEW...")
  include(${CMAKE_SOURCE_DIR}/External-GLEW.cmake)
  list(APPEND DRUNKEN_SAILOR_ENGINE_DEPENDENCIES GLEW)
else (NOT GLEW_FOUND)
  set (GLEW_DIR ${GLEW_INCLUDE_DIR}/../)
endif (NOT GLEW_FOUND)

# Try to find SFML package
set(SFML_ROOT ${CMAKE_SOURCE_DIR}/../../external/SFML)
find_package(SFML COMPONENTS audio)
if (NOT SFML_FOUND)
  message("Will download SFML..")
  include(${CMAKE_SOURCE_DIR}/External-SFML.cmake)
  list(APPEND DRUNKEN_SAILOR_ENGINE_DEPENDENCIES sfml)
else (NOT SFML_FOUND)
  set (SFML_ROOT ${SFML_INCLUDE_DIR}/..)
endif (NOT SFML_FOUND)

# Try to find stb
set(STB_BASE_DIR ${CMAKE_SOURCE_DIR}/../../external/stb)
find_package(STB)
if (NOT STB_FOUND)
  message("Will download stb..")
  include(${CMAKE_SOURCE_DIR}/External-STB.cmake)
  list(APPEND DRUNKEN_SAILOR_ENGINE_DEPENDENCIES stb)
else (NOT STB_FOUND)
  set (STB_BASE_DIR ${STB_INCLUDE_DIR})
endif (NOT STB_FOUND)

# Try to find assimp
set(ASSIMP_ROOT_DIR ${CMAKE_SOURCE_DIR}/../../external/assimp)
find_package(assimp)
if (NOT assimp_FOUND)
  message("Will download assimp..")
  include(${CMAKE_SOURCE_DIR}/External-ASSIMP.cmake)
  list(APPEND DRUNKEN_SAILOR_ENGINE_DEPENDENCIES assimp)
else (NOT assimp_FOUND)
  set (ASSIMP_ROOT_DIR ${ASSIMP_INCLUDE_DIRS}/..)
endif (NOT assimp_FOUND)

# Try to find LuaBridge
set(LUABRIDGE_ROOT_DIR ${CMAKE_SOURCE_DIR}/../../external/LuaBridge)
find_package(LuaBridge)
if (NOT LUABRIDGE_FOUND)
  message("Will download LuaBridge..")
  include(${CMAKE_SOURCE_DIR}/External-LuaBridge.cmake)
  list(APPEND DRUNKEN_SAILOR_ENGINE_DEPENDENCIES LuaBridge)
else (NOT LUABRIDGE_FOUND)
  set (LUABRIDGE_ROOT_DIR ${LUABRIDGE_INCLUDE_DIR}/..)
endif (NOT LUABRIDGE_FOUND)

ExternalProject_Add(
	drunken_sailor_engine
	DEPENDS ${DRUNKEN_SAILOR_ENGINE_DEPENDENCIES}
	DOWNLOAD_COMMAND ""
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../
	BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../build
  INSTALL_DIR ${CMAKE_SOURCE_DIR}/../../bin
	CMAKE_ARGS
    # Set variables for various packages to find
		-DLUAJIT_DIR:PATH=${LUAJIT_DIR}
		-DGTEST_ROOT:PATH=${GTEST_ROOT}
    -DSDL2DIR=${SDL2DIR}
    -DRAPIDJSON_INCLUDEDIR=${RAPIDJSON_INCLUDEDIR}
    -DGLEW_DIR=${GLEW_DIR}
    -DSFML_ROOT=${SFML_ROOT}
    -DASSIMP_ROOT_DIR=${ASSIMP_ROOT_DIR}
    -DSTB_BASE_DIR=${STB_BASE_DIR}
    -DLUABRIDGE_ROOT_DIR=${LUABRIDGE_ROOT_DIR}
		-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
	)
