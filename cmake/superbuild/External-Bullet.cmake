# Get and build Bullet

ExternalProject_Add(Bullet
  GIT_REPOSITORY https://github.com/bulletphysics/bullet3.git
  GIT_TAG master
  CONFIGURE_COMMAND ${CMAKE_COMMAND} <SOURCE_DIR> -DCMAKE_BUILD_TYPE=Release
  BUILD_COMMAND ${CMAKE_COMMAND} --build ./ --config Release
  INSTALL_DIR "${BULLET_ROOT}"
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    -DCMAKE_BUILD_TYPE="Release"
    -DBUILD_BULLET2_DEMOS=OFFS
    -DBUILD_CPU_DEMOS=OFF
    # Build Extras to fix (avoid) S
    -DBUILD_EXTRAS=ON
    -DBUILD_OPENGL3_DEMOS=OFF
    -DBUILD_UNIT_TESTS=OFF
    -DINSTALL_LIBS=ON
  )
ExternalProject_Add_Step(Bullet bullet_copy_release_libs
  COMMAND ${CMAKE_COMMAND} -E copy_directory <BINARY_DIR>/lib/Release <INSTALL_DIR>/lib
)