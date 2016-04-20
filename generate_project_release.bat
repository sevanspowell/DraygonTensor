mkdir build
cd build
mkdir superbuild
cd superbuild
cmake.exe ..\..\cmake\superbuild\ -DCMAKE_BUILD_TYPE=Release
cmake.exe --build ./ --config Release
cd ..\..\