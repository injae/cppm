## Cppm Base Project Layout
~~~console
.
|-- cppm.toml # cppm config file
|-- CMakeLists.txt # <-- generated from cppm.toml 
|-- build  # Build directory
|   |-- Debug   # Debug mode target directory 
|   `-- Release # Release mode target directory
|-- cmake    # cmake module path
|   |-- Modules  # cmake Find*.cmake path
|   `-- cppm_tool.cmake # cppm_tool load script (require)
|-- include   # public header (export header)
|-- src       # private header and sources
|   `-- main.cpp
|-- test      # test src directory 
|   `-- test01.cpp 
|-- examples  # example src directory
|   `-- example01.cpp 
`-- thirdparty # cppkg config and file path
    `-- ${cppkg_name}/${cppkg_version}/cppkg.toml  # cppkg config file
~~~

## Cppkg Structure
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
└── share  # cppkg install path
    ├── cppm-0.0.10
    .   ├── bin ...
    .   ├── include ...
    .   ├── lib ...
    .   └── src ...
 
~~~
