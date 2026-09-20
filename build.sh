mkdir -p ./build
cd ./build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=$1
ninja
cd ..
