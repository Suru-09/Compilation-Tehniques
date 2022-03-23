
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

