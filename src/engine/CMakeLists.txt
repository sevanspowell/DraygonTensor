include_directories(..)

set(ENGINE_INCLUDE_FILES
  Config.h
  Engine.h
  common/Common.h
  common/StreamBuffer.h
  common/StreamBuffer.hpp
  common/StringIntern.h
  message/Message.h
  message/MessageBus.h
  message/MessageFactory.h
  message/MessageHelper.h
  resource/IResource.h
  resource/MaterialResource.h
  resource/MeshResource.h
  resource/ResourceFactory.h
  resource/ResourceFactory.hpp
  resource/ShaderResource.h
  resource/TextureResource.h
  system/Console.h
  system/Physics.h
  system/input/Input.h
  system/input/InputContext.h
  system/platform/Keyboard.h
  system/platform/Platform.h
  system/platform/Video.h
  system/platform/Window.h
  system/render/Render.h
  system/script/LuaEnvironment.h
  system/script/LuaHelper.h
  system/script/Script.h
  system/script/ScriptBindingSet.h
)

set(ENGINE_SRC_FILES
  Config.cpp
  Engine.cpp
  common/Common.cpp
  common/StreamBuffer.cpp
  common/StringIntern.cpp
  message/MessageBus.cpp
  message/MessageFactory.cpp
  message/MessageHelper.cpp
  resource/MaterialResource.cpp
  resource/MeshResource.cpp
  resource/ShaderResource.cpp
  resource/TextureResource.cpp
  system/Console.cpp
  system/Physics.cpp
  system/input/Input.cpp
  system/input/InputContext.cpp
  system/platform/Keyboard.cpp
  system/platform/Platform.cpp
  system/platform/Video.cpp
  system/platform/Window.cpp
  system/render/Render.cpp
  system/script/LuaMathAPI.cpp
  system/script/LuaEnvironment.cpp
  system/script/LuaHelper.cpp
  system/script/Script.cpp
  system/script/ScriptLuaBindings.cpp
  system/script/ScriptBindingSet.cpp
)

add_library(drunken_sailor_engine STATIC ${ENGINE_INCLUDE_FILES} ${ENGINE_SRC_FILES})

target_link_libraries(drunken_sailor_engine ${LIBS} ds_math)

install(TARGETS drunken_sailor_engine DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/ DESTINATION include
  FILES_MATCHING PATTERN "*.h")
