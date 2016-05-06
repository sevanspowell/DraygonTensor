include_directories(${CMAKE_SOURCE_DIR}/src)

# Compile game files
set(GAME_INCLUDE_FILES
)

set(GAME_SRC_FILES
    main.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${GAME_INCLUDE_FILE} ${GAME_SRC_FILES})

# Link third-party libraries
target_link_libraries(${PROJECT_NAME} ${LIBS} drunken_sailor_engine)

# Setup project executable directory
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)

get_target_property(PROJECT_EXECUTABLE_DIR ${PROJECT_NAME} RUNTIME_OUTPUT_DIRECTORY)
set(PROJECT_EXECUTABLE_DIR ${PROJECT_EXECUTABLE_DIR}/${CMAKE_BUILD_TYPE})

# Copy DLLS to executable directory
foreach(DLL ${REQUIRED_DLLS})
    add_custom_command(
      TARGET ${PROJECT_NAME}
      COMMAND ${CMAKE_COMMAND} -E copy ${DLL} ${PROJECT_SOURCE_DIR}/bin
      )
endforeach(DLL ${REQUIRED_DLLS})
