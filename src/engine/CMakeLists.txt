include_directories(..)

set(ENGINE_INCLUDE_FILES
  Config.h
  Engine.h
  common/Common.h
  common/Handle.h
  common/HandleManager.h
  common/StreamBuffer.h
  common/StreamBuffer.hpp
  common/StringIntern.h
  entity/ComponentManager.h
  entity/ComponentManager.hpp
  entity/Entity.h
  entity/EntityManager.h
  entity/IComponentManager.h
  entity/Instance.h
  message/Message.h
  message/MessageBus.h
  message/MessageFactory.h
  message/MessageHelper.h
  resource/IResource.h
  resource/MaterialResource.h
  resource/MaterialResourceManager.h
  resource/MeshResource.h
  resource/ResourceFactory.h
  resource/ResourceFactory.hpp
  resource/ShaderResource.h
  resource/TerrainResource.h
  resource/TextureResource.h
  resource/TextureResourceManager.h
  system/Console.h
  system/input/Input.h
  system/input/InputContext.h

  system/physics/Physics.h
  system/physics/PhysicsWorld.h
  system/physics/PhysicsComponentManager.h
  system/physics/ForceGenerator.h
  system/physics/RigidBody.h
  system/physics/CollisionFine.h

  system/platform/Keyboard.h
  system/platform/Mouse.h
  system/platform/Platform.h
  system/platform/Video.h
  system/platform/Window.h
  system/render/ButtonComponent.h
  system/render/ButtonComponentManager.h
  system/render/CameraComponent.h
  system/render/CameraComponentManager.h
  system/render/ConstantBuffer.h
  system/render/ConstantBufferDescription.h
  system/render/GLRenderer.h
  system/render/IRenderer.h
  system/render/Material.h
  system/render/Mesh.h
  system/render/Render.h
  system/render/RenderComponent.h
  system/render/RenderComponentManager.h
  system/render/ShaderParameter.h
  system/render/Skybox.h
  system/render/SubMesh.h
  system/render/TerrainComponent.h
  system/render/TerrainComponentManager.h
  system/render/Texture.h
  system/render/VertexBufferDescription.h
  system/scene/TransformComponent.h
  system/scene/TransformComponentManager.h
  system/script/LuaEnvironment.h
  system/script/LuaHelper.h
  system/script/Script.h
  system/script/ScriptBindingSet.h
)

set(ENGINE_SRC_FILES
  Config.cpp
  Engine.cpp
  common/Common.cpp
  common/HandleManager.cpp
  common/StreamBuffer.cpp
  common/StringIntern.cpp
  entity/Entity.cpp
  entity/EntityManager.cpp
  message/MessageBus.cpp
  message/MessageFactory.cpp
  message/MessageHelper.cpp
  resource/MaterialResource.cpp
  resource/MaterialResourceManager.cpp
  resource/MeshResource.cpp
  resource/ShaderResource.cpp
  resource/TerrainResource.cpp
  resource/TextureResource.cpp
  resource/TextureResourceManager.cpp
  system/Console.cpp

  system/input/Input.cpp
  system/input/InputContext.cpp
  system/input/InputLuaBindings.cpp

  system/physics/Physics.cpp
  system/physics/PhysicsWorld.cpp
  system/physics/PhysicsComponentManager.cpp
  system/physics/PhysicsLuaBindings.cpp
  system/physics/ForceGenerator.cpp
  system/physics/RigidBody.cpp
  system/physics/Contacts.cpp
  system/physics/CollisionFine.cpp

  system/platform/Keyboard.cpp
  system/platform/Platform.cpp
  system/platform/Video.cpp
  system/platform/Window.cpp
  system/render/ButtonComponentManager.cpp
  system/render/CameraComponentManager.cpp
  system/render/ConstantBuffer.cpp
  system/render/ConstantBufferDescription.cpp
  system/render/GLRenderer.cpp
  system/render/Material.cpp
  system/render/Mesh.cpp
  system/render/Render.cpp
  system/render/RenderComponentManager.cpp
  system/render/RenderLuaBindings.cpp
  system/render/ShaderParameter.cpp
  system/render/Skybox.cpp
  system/render/TerrainComponentManager.cpp
  system/render/Texture.cpp
  system/render/VertexBufferDescription.cpp
  system/scene/TransformComponentManager.cpp
  system/script/LuaEnvironment.cpp
  system/script/LuaHelper.cpp
  system/script/LuaMathAPI.cpp
  system/script/Script.cpp
  system/script/ScriptBindingSet.cpp
  system/script/ScriptLuaBindings.cpp
)

add_library(drunken_sailor_engine STATIC ${ENGINE_INCLUDE_FILES} ${ENGINE_SRC_FILES})

target_link_libraries(drunken_sailor_engine ${LIBS} ds_math)

install(TARGETS drunken_sailor_engine DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/ DESTINATION include
  FILES_MATCHING PATTERN "*.h")
