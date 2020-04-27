## Installation
### Install Cppm
cppm install command install defualt prefix is **$HOME/.cppm/bin** 
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
# Add Windows System Path /.cppm/bin
```



