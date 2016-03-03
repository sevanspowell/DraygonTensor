include_directories(..)

set(ENGINE_INCLUDE_FILES
  temp/Hello.h
)

set(ENGINE_SRC_FILES
  temp/Hello.cpp
)

add_library(drunken_sailor_engine STATIC ${ENGINE_INCLUDE_FILES} ${ENGINE_SRC_FILES})

target_link_libraries(drunken_sailor_engine ${LIBS})

install(TARGETS drunken_sailor_engine DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/ DESTINATION include
  FILES_MATCHING PATTERN "*.h")
