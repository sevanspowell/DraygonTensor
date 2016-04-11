include_directories(.)

set(MATH_INCLUDE_FILES
  MathHelper.h
  Matrix4.h
  Precision.h
  Quaternion.h
  Vector3.h
  Vector4.h
)

set(MATH_SRC_FILES
  MathHelper.cpp
  Matrix4.cpp
  Quaternion.cpp
  Vector3.cpp
  Vector4.cpp
)

add_library(ds_math STATIC ${MATH_INCLUDE_FILES} ${MATH_SRC_FILES})

install(TARGETS ds_math DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/ DESTINATION include
  FILES_MATCHING PATTERN "*.h")
