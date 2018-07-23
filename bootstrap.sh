#!/bin/bash

# clang-6 install ubuntu 14.04
#sudo apt-add-repository "deb http://apt.llvm.org/trusty/ llvm-toolchain-trusty-6.0 main"
# clang-6 install ubuntu 16.04
sudo apt-add-repository "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-6.0 main"
sudo apt-get install clang-6.0 lldb-6.0 lld-6.0

cd thirdparty

sudo wget https://cmake.org/files/v3.12/cmake-3.12.0.tar.gz

tar -zxvf cmake-3.12.0.tar.gz
cd cmake-3.12.0
./bootstrap
make 
sudo make install
cd ..
sudo rm -rf cmake-3.12.0

sudo apt install ccache
export PATH=/usr/lib/ccache:$PATH


git clone https://github.com/INJAE/nieel.git

cd nieel
cmake .
make 
sudo make install
sudo rm -rf nieel

cd ..

cd bin
cmake ..
make 
sudo make install






