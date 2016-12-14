cmake_minimum_required(VERSION 2.8)

project(DrunkenSailorEngine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

if (UNIX)
	set(CMAKE_CXX_FLAGS "-Wall -Werror -std=c++11")
	# set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
endif (UNIX)

if (MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")
endif (MSVC)

set(REQUIRED_DLLS)

# Find GTest
find_package(GTest REQUIRED)
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
set(LIBS ${LIBS} ${GTEST_LIBRARIES})

# Find Lua
find_package(LuaJIT REQUIRED)
include_directories(SYSTEM ${LUAJIT_INCLUDE_DIR})
set(LIBS ${LIBS} ${LUAJIT_LIBRARIES})
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_SIZEOF_VOID_P EQUAL 8)
  # Required for LuaJIT and 64-bit OSX
  set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -pagezero_size 10000 -image_base 100000000")
  set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -image_base 100000000")
  set(CMAKE_MODULE_LINKER_FLAGS
    "${CMAKE_MODULE_LINKER_FLAGS} -image_base 100000000")
endif(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_SIZEOF_VOID_P EQUAL 8)
if (WIN32)
  list(APPEND REQUIRED_DLLS ${LUAJIT_DIR}/bin/lua51.dll)
endif (WIN32)

# Find RapidJson
find_package(rapidjson REQUIRED)
include_directories(SYSTEM ${RAPIDJSON_INCLUDE_DIRS})

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(SYSTEM ${SDL2_INCLUDE_DIR})
set(LIBS ${LIBS} ${SDL2_LIBRARY})
if (WIN32)
  list(APPEND REQUIRED_DLLS ${SDL2DIR}/lib/x86/SDL2.dll)
endif (WIN32)

# Find OpenGL
find_package(OpenGL REQUIRED)
include_directories(SYSTEM ${OPENGL_INCLUDE_DIR})
set(LIBS ${LIBS} ${OPENGL_LIBRARIES})

# Find GLEW
find_package(GLEW REQUIRED)
include_directories(SYSTEM ${GLEW_INCLUDE_DIRS})
set(LIBS ${LIBS} ${GLEW_LIBRARIES})
if (WIN32)
  list(APPEND REQUIRED_DLLS ${GLEW_DIR}/bin/glew32.dll)
endif (WIN32)

# Find SFML
find_package(SFML COMPONENTS audio REQUIRED)
include_directories(SYSTEM ${SFML_INCLUDE_DIR})
message(${SFML_LIBRARIES})
set (LIBS ${LIBS} ${SFML_LIBRARIES})
if (WIN32)
	file(GLOB SFML_DLLS ${SFML_ROOT}/bin/*)
	foreach(DLL ${SFML_DLLS})
		list(APPEND REQUIRED_DLLS ${DLL})
	endforeach(DLL ${SFML_DLLS})
	list (APPEND REQUIRED_DLLS ${SFML_ROOT}/bin/openal32.dll)
	list (APPEND REQUIRED_DLLS ${SFML_ROOT}/bin/sfml-audio-d-2.dll)
	list (APPEND REQUIRED_DLLS ${SFML_ROOT}/bin/sfml-system-d-2.dll)
endif (WIN32)

# Find STB
find_package(STB REQUIRED)
include_directories(SYSTEM ${STB_INCLUDE_DIR})

# Find Assimp
find_package(assimp REQUIRED)
include_directories(SYSTEM ${ASSIMP_INCLUDE_DIRS})
set(LIBS ${LIBS} ${ASSIMP_LIBRARIES})
if (WIN32)
  list(APPEND REQUIRED_DLLS ${ASSIMP_ROOT_DIR}/bin/assimp-${ASSIMP_MSVC_VERSION}-mt.dll)
endif(WIN32)

# Find LuaBridge
find_package(LuaBridge REQUIRED)
include_directories(SYSTEM ${LUABRIDGE_INCLUDE_DIR})

if (DS_LUAJIT_WORKAROUND)
	add_definitions(-DDS_LUAJIT_WORKAROUND)
endif (DS_LUAJIT_WORKAROUND)


subdirs(src test project)
