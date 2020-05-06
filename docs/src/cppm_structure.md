## Cppm Structure
~~~console
~/.cppm/
├── bin  # cppm installed binary path
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
│       │   │   ├── cppkg.toml
│       │   │   └── dep.cmake
│       │   └── git
│       │       ├── Catch2.cmake.in
│       │       ├── cppkg.toml
│       │       └── dep.cmake
│       ├── cppm
│       │   └── git
│       .       ├── cppkg.toml
│       .       ├── cppm.cmake.in
│       .       └── dep.cmake
└── cppkg  # cppkg install path
    ├── cppm-0.0.10
    .   ├── bin ...
    .   ├── include ...
    .   ├── lib ...
    .   └── src ...
 
~~~
