## Installation
### Install Cppm
cppm install command install defualt prefix is **$HOME/.cppm/bin** 

## Require
- C++17 Compiler (AppleClang can't build)  
- CMake (Minumum 3.12)  
- git  
- ccache (optional)  

#### Linux and MacOS
```sh
git clone https://github.com/injae/cppm
cd cppm
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release.
cd build
cmake --build . --config Release
cd Release
./cppm build install
# Add to cppm path
export PATH=$PATH:$HOME/.cppm/bin
```

#### Windows
```sh
git clone https://github.com/injae/cppm
cd cppm
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release.
cd build
cmake --build . --config Release
cd Release
./cppm build install
# Add Windows System Path %USERPROFILE%/.cppm/bin
```



