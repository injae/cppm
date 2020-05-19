## Cppm Structure
cppm use cmake toolchain feature
cppm build command use CMakeCache.txt 

***cppm build*** command search cppm.toml or CMakeLists.txt
```console
$ cppm build --detail 
cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/.cppm/cppkg/cppm-tools-0.0.11/toolchain.cmake -DUSE_CPPM_PATH=ON -DCMAKE_BUILD_TYPE=Debug .. && cmake --build . --config Debug --  -j{your cpu core}
```

~~~console
~/.cppm/
├── bin  # cppm installed binary path (symbolic links)
│   └── cppm # -> ../share/cppm-0.0.10/bin/cppm
├── cache ... # cmake build cache
├── cmake  # cppm module path
│   ├── HunterGate.cmake # if you use hunter package this file copy your package cmake/HunterGate.cmake
│   └── cppm_tool.cmake 
├── repo # cppkg regiested package installer path 
│   └── cppkg
│       ├── Catch2
│       │   ├── 2.9.1
│       │   │   ├── Catch2.cmake.in
│       │   │   └── cppkg.toml
│       │   └── git
│       │       ├── Catch2.cmake.in
│       │       └── cppkg.toml
│       ├── cppm
│       │   └── git
│       .       ├── cppkg.toml
│       .       └── cppm.cmake.in 
└── cppkg  # cppkg install path
    ├── cppm-0.0.11
    .   ├── bin ...
    .   ├── include ...
    .   └── lib ...
 
~~~
