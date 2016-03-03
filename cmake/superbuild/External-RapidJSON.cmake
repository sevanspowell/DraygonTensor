# Get and build Rapid JSON

ExternalProject_Add(rapidjson
  GIT_REPOSITORY https://github.com/miloyip/rapidjson.git
  GIT_TAG master
  INSTALL_DIR "${RAPIDJSON_INCLUDEDIR}/.."
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
	  -DRAPIDJSON_BUILD_DOC=OFF
	  -DRAPIDJSON_BUILD_EXAMPLES=OFF
	  -DRAPIDJSON_BUILD_TESTS=OFF
  )
