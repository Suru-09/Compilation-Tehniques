#Delete old folder, in case it was compiled on a different arhitecture
rm -rf build/;
mkdir build;

current_dir=$PWD;

cd build/;

cmake ../;

if cmake --build .; then 
echo "Build succeded!";
else 
echo "Buil failed, operation will be aborted!";
exit
fi


./CT-compiler
