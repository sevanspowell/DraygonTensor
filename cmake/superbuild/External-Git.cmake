# Get and build Git

if (WIN32)
  ExternalProject_Add(
    Git
    URL "https://github.com/git-for-windows/git/releases/download/v2.7.2.windows.1/PortableGit-2.7.2-64-bit.7z.exe"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_DIR "${GITDIR}"
    CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    )
endif(WIN32)
