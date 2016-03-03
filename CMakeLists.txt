cmake_minimum_required(VERSION 2.8)

project(DrunkenSailorEngine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

if (UNIX)
	set(CMAKE_CXX_FLAGS "-Wall -Werror -std=c++11")
endif (UNIX)

if (MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif (MSVC)

set(REQUIRED_DLLS)

find_package(GTEST REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
set(LIBS ${LIBS} ${GTEST_LIBRARIES})

find_package(LUA REQUIRED)
include_directories(${LUA_INCLUDE_DIR})
set(LIBS ${LIBS} ${LUA_LIBRARIES})
if (WIN32)
  list(APPEND REQUIRED_DLLS ${LUA_DIR}/lua53.dll)
endif (WIN32)

find_package(rapidjson REQUIRED)
include_directories(${RAPIDJSON_INCLUDE_DIRS})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
set(LIBS ${LIBS} ${SDL2_LIBRARY})
if (WIN32)
  list(APPEND REQUIRED_DLLS ${SDL2DIR}/lib/x86/SDL2.dll)
endif (WIN32)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
set(LIBS ${LIBS} ${OPENGL_LIBRARIES})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
set(LIBS ${LIBS} ${GLEW_LIBRARIES})
if (WIN32)
  list(APPEND REQUIRED_DLLS ${GLEW_DIR}/bin/glew32.dll)
endif (WIN32)

find_package(SFML COMPONENTS audio REQUIRED)
include_directories(${SFML_INCLUDE_DIR})
set (LIBS ${LIBS} ${SFML_LIBRARIES})
if (WIN32)
#	file(GLOB SFML_DLLS ${SFML_ROOT}/bin/*)
#	foreach(DLL ${SFML_DLLS})
#		list(APPEND REQUIRED_DLLS ${DLL})
#	endforeach(DLL ${SFML_DLLS})
	list (APPEND REQUIRED_DLLS ${SFML_ROOT}/bin/openal32.dll)
	list (APPEND REQUIRED_DLLS ${SFML_ROOT}/bin/sfml-audio-d-2.dll)
	list (APPEND REQUIRED_DLLS ${SFML_ROOT}/bin/sfml-system-d-2.dll)
endif (WIN32)

subdirs(src test project)
