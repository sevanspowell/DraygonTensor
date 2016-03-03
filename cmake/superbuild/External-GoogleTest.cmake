# Get and build Google Test

ExternalProject_Add(googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG master
	INSTALL_DIR "${GTEST_ROOT}"
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
	)