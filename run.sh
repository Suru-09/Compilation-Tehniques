#Delete old folder, in case it was compiled on a different arhitecture
rm -rf build/;
mkdir build;

current_dir=$PWD;

cd build/;

cmake ../;
cmake --build .;

./CT-compiler
