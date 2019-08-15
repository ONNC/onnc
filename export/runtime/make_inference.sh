cd build/
sudo cmake ../
sudo make
cd ..
sudo gcc -o build/example/inference build/example/libinference.a include/operators.o -lm
