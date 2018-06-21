#!/bin/bash

#cd build
#./build.sh

cd bin
cmake ..
make 
sudo make install

#cd ..
##./bin/DefaultCmakeProject

exit 0
