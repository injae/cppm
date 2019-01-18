# Welcome the Cppm

Welcome to the Cppm Documentation

Cppm is a program that manages the C/C++ Cmake project using cppm.toml.  
Cmake is complex and difficult for beginners to use.  
So I wrapped Cmake once more to make it easier to use as Rust Cargo.  
Because I created it with reference to Cargo, the general configuration file is similar to Cargo.toml.  
Cppm works only with Cmake, so you can build with Cmake.

Cppkg is the dependency manager for Cppm. Since cppkg uses Cmake's `Exteranl_Porject_Add`,  
it supports cross-platfrom install.

Cppm supports [Hunter](https://github.com/ruslo/hunter) Package manager. So cppm dependency can be either hunter or cppkg.

## Commands
* `cppm run    {options} {commands}`  
* `cppm init   {options} {commands}`  
* `cppm cppkg  {options} {commands}`  
* `cppm build  {options} {commands}`  
* `cppm config {options} {commands}`  
* `cppm update {options} {commands}`  

## Project layout

    cppm.toml             # Cppm configuration file. Generate CMakeLists.txt
    build/                # build directory
    cmake/                # cmake files
        Modules/          # Find cmake files
        cppm_tool.cmake   # cppm cmake tools
        HunterGate.cmake  # Hunter package manager cmake tools
    include/              # include directory
    src/                  # source directory
    thirdparty/           # cppkg script install path


