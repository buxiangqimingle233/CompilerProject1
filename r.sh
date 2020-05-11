rm -rf build/*
cd build
cmake ..
make -j 4
cd test
./demo