# DrunkenSailorEngine

[![Built with Spacemacs](https://cdn.rawgit.com/syl20bnr/spacemacs/442d025779da2f62fc86c2082703697714db6514/assets/spacemacs-badge.svg)](http://github.com/syl20bnr/spacemacs)

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-generate-toc again -->
**Table of Contents**

  - [Prerequisites](#prerequisites)
      - [Windows](#windows)
      - [OSX](#osx)
      - [Linux](#linux)
  - [Building](#building)
      - [Visual Studio](#visual-studio)
      - [Xcode](#xcode)
      - [Makefile](#makefile)
  - [Contributing](#contributing)

<!-- markdown-toc end -->


## Prerequisites 

### Windows

- CMake 2.8 or greater
- Git

The included build scripts assume that `cmake.exe` can be found in the path, edit them to reflect the location of your cmake binary.

### OSX

- CMake 2.8 or greater
- Git
- SDL2-2.0.4 or greater

### Linux

- CMake 2.8 or greater
- Git
- SDL2-2.0.4 or greater

## Building

### Visual Studio 
  
    git clone https://github.com/Izabia/DrunkenSailorEngine.git
    cd bangbang
    generate_project_debug.bat OR generate_project_release.bat

The `generate_project_buildtype` batch scripts will create an out-of-source build, download all third-party libraries and generate project files for the bangbang code under build/. You can then use the `build_buildtype` scripts to configure and build the bangbang code exclusively or the `generate_project_buildtype` scripts to configure and build all third-party libraries as well as the bangbang code.

OR

    git clone https://github.com/Izabia/DrunkenSailorEngine.git
    cd DrunkenSailorEngine
    mkdir build\superbuild
    cd build\superbuild
    cmake.exe ..\..\cmake\superbuild -DCMAKE_BUILD_TYPE=<Debug OR Release>
    cmake.exe --build ./ --config Debug OR build in VS

### Xcode

    git clone https://github.com/samdelion/DrunkenSailorEngine.git
    cd DrunkenSailorEngine
    mkdir -p build/superbuild
    cd build/superbuild
    cmake ../../cmake/superbuild -G Xcode -DCMAKE_BUILD_TYPE=Debug
    cmake --build ./ --config Debug OR build in Xcode

### Makefile 

    git clone https://github.com/samdelion/DrunkenSailorEngine.git
    cd DrunkenSailorEngine
    mkdir -p build/superbuild
    cd build/superbuild
    cmake ../../cmake/superbuild
    make

If this series of build steps feels burdensome, consider writing a shell script.

Also, please note that the first build may take upwards of 15 minutes as the dependencies are downloaded and built.

## Contributing

Before contributing, consult the [contribution guidelines](https://github.com/Izabia/DrunkenSailorEngine/blob/develop/CONTRIBUTING.md).
    
