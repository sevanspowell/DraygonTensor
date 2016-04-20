mkdir build
cd build
mkdir superbuild
cd superbuild
cmake.exe ..\..\cmake\superbuild\ -DCMAKE_BUILD_TYPE=Debug
cmake.exe --build ./ --config Debug
cd ..\..\