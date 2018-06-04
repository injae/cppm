#!/bin/bash

script_dir=$(cd "$(dirname "$0")" && pwd)

export CXXC_PATH="$script_dir"
export PATH="$PATH:$script_dir/bin"

echo $CXXCPATH
echo $PATH 
echo "done"

./thirdparty/protobuf/bin/bin/protoc -I=./src/protocal --cpp_out=./src/protocal ./src/protocal/addressbook.proto

#exit 0
