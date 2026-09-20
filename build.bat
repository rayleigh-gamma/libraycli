mkdir .\build
cd .\build
cmake.exe .. -G Ninja -DCMAKE_BUILD_TYPE=%1
ninja.exe
cd ..
