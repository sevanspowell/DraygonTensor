include_directories(${CMAKE_SOURCE_DIR}/test ${CMAKE_SOURCE_DIR}/src/)

set(TEST_SUITE_INCLUDE_FILES
)

set(TEST_SUITE_SRC_FILES
	main.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${TEST_SUITE_INCLUDE_FILES} ${TEST_SUITE_SRC_FILES})

# Link third-party libraries
target_link_libraries(${PROJECT_NAME} ${LIBS} drunken_sailor_engine)

# Setup project executable directory
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)

get_target_property(PROJECT_EXECUTABLE_DIR ${PROJECT_NAME} RUNTIME_OUTPUT_DIRECTORY)
set(PROJECT_EXECUTABLE_DIR ${PROJECT_EXECUTABLE_DIR}/${CMAKE_BUILD_TYPE})

# Copy assets to executable directory
add_copy_files_target(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/assets/ * ${PROJECT_EXECUTABLE_DIR})

# Copy DLLS to executable directory
foreach(DLL ${REQUIRED_DLLS})
	message(${DLL})
    add_custom_command(
      TARGET ${PROJECT_NAME}
      COMMAND ${CMAKE_COMMAND} -E copy ${DLL} ${PROJECT_EXECUTABLE_DIR}
      )
endforeach(DLL ${REQUIRED_DLLS})
