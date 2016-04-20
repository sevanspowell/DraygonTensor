# Get and build SFML

if (APPLE)
  ExternalProject_Add(sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG master
    INSTALL_DIR "${SFML_ROOT}"
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
      -DCMAKE_INSTALL_FRAMEWORK_PREFIX=<INSTALL_DIR>/Frameworks
    )
else (APPLE)
  ExternalProject_Add(sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG master
    INSTALL_DIR "${SFML_ROOT}"
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    )
endif (APPLE)
