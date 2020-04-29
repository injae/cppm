## First Steps with Cppm
To start a new package with Cppm, use ```cppm init```  
~~~console
$ cppm init -h  
Usage:
   cppm init [--verbose]

Option:
   --bin [-b] {name}               :initialize new c++ binary project
   --help [-h]                     :show cppm commands and options
   --lib [-l] {name}               :initialize new c++ library project
~~~

New Binary Project
~~~console
$ cppm init --bin hello_world
~~~

Generated project
~~~console
$ cd hello_world
$ tree .  
.
|-- build  # Build directory
|   |-- Debug   # Debug mode target directory 
|   `-- Release # Release mode target directory
|-- cmake    # cmake module path
|   |-- Modules  # cmake Find*.cmake path
|   `-- cppm_tool.cmake # cppm_tool load tool
|-- cppm.toml
|-- include   # public header
|-- src       # private header and sources
|   `-- main.cpp
`-- thirdparty # cppkg config and file path
    `-- ${cppkg_name}/${cppkg_version}/cppkg.toml  # cppkg config file
~~~

Cppm project config file 
~~~toml
# cppm.toml
[package]
   name = "hello_world"
   version = "0.0.1"
   description = ""

[[bin]]
   name = "hello_world"
   source = ["src/.*"]
~~~

Generated defualt cpp file
~~~cpp
// src/main.cpp
#include <iostream>
int main(int argc, char* argv[]) {
    std::cout<<"hello world"<<std::endl;
    return 0; 
} 
~~~

Compile "hello_world" project with ```cppm build```, this command generate ```CMakeLists.txt```
~~~console
$ cppm build
[cppm] Generate CMakeLists.txt
From https://github.com/injae/cppm_tools
   1c79dca..00fb374  0.0.9      -> origin/0.0.9
-- [cppm] cppm_tools-0.0.9 download to /home-path/.cppm/src/cppm_tools/0.0.9
-- The C compiler identification is Clang 9.0.0
-- The CXX compiler identification is Clang 9.0.0
-- Check for working C compiler: /usr/local/opt/llvm/bin/clang
-- Check for working C compiler: /usr/local/opt/llvm/bin/clang -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/local/opt/llvm/bin/clang++
-- Check for working CXX compiler: /usr/local/opt/llvm/bin/clang++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- [cppm] Target: hello_world [Type:debug, Cppm:0.0.9, CMake:3.16.2]
-- [cppm] System: x86_64-Darwin-19.3.0
-- [cppm] Compiler: Clang-9.0.0
-- [cppm] Generator: Unix Makefiles
-- [cppm] Build Cache: ccache
-- [cppm] cppm_root: /path/to/.cppm
-- [cppm] c++ version: 17
-- [cppm] Compiler Option: -std=c++17 -Wall -fPIC -O0 -g
-- Configuring done
-- Generating done
-- Build files have been written to: /path/to/hello_world/build
Scanning dependencies of target hello_world
[ 50%] Building CXX object CMakeFiles/hello_world.dir/src/main.cpp.o
[100%] Linking CXX executable hello_world
[100%] Built target hello_world
~~~
And the run it.  
~~~console
$ cd build/Debug
$ ./hello_world
hello world
~~~
Easy run command ```cppm run```, default binary name ${[package.name]}   
~~~console
$ cppm run
hello world
~~~
Generated ```CMakeLists.txt```
~~~cmake
cmake_minimum_required(VERSION 3.12)

include(cmake/cppm_tool.cmake)
cppm_project()
project(hello_world VERSION 0.0.1 LANGUAGES C CXX)
cppm_setting()

cppm_cxx_standard(17)
cppm_compiler_option(DEFAULT)

cppm_target_define(hello_world BINARY 
SOURCES 
        src/main.cpp
)

cppm_target_dependencies(hello_world)

cppm_target_install(hello_world)
~~~

Changed project Directory
~~~console
$ tree .
.
├── CMakeLists.txt
├── build
│   ├── CMakeCache.txt
│   ├── CMakeFiles
│   │   ├── 3.17.1
│   │   │   ├── CMakeCCompiler.cmake
│   │   │   ├── CMakeCXXCompiler.cmake
│   │   │   ├── CMakeDetermineCompilerABI_C.bin
│   │   │   ├── CMakeDetermineCompilerABI_CXX.bin
│   │   │   ├── CMakeSystem.cmake
│   │   │   ├── CompilerIdC
│   │   │   │   ├── CMakeCCompilerId.c
│   │   │   │   ├── a.out
│   │   │   │   └── tmp
│   │   │   └── CompilerIdCXX
│   │   │       ├── CMakeCXXCompilerId.cpp
│   │   │       ├── a.out
│   │   │       └── tmp
│   │   ├── CMakeDirectoryInformation.cmake
│   │   ├── CMakeOutput.log
│   │   ├── CMakeTmp
│   │   ├── Makefile.cmake
│   │   ├── Makefile2
│   │   ├── TargetDirectories.txt
│   │   ├── cmake.check_cache
│   │   ├── progress.marks
│   │   ├── hello_world.dir
│   │   │   ├── CXX.includecache
│   │   │   ├── DependInfo.cmake
│   │   │   ├── build.make
│   │   │   ├── cmake_clean.cmake
│   │   │   ├── depend.internal
│   │   │   ├── depend.make
│   │   │   ├── flags.make
│   │   │   ├── link.txt
│   │   │   ├── progress.make
│   │   │   └── src
│   │   │       └── main.cpp.o
│   │   └── hello_world_info.dir
│   │       ├── DependInfo.cmake
│   │       ├── build.make
│   │       ├── cmake_clean.cmake
│   │       └── progress.make
│   ├── Debug
│   │   └── hello_world  # Binary or Library export Directory
│   ├── Makefile
│   ├── cmake_install.cmake
│   └── compile_commands.json
├── cmake
│   ├── Modules
│   └── cppm_tool.cmake
├── cppm.toml
├── include
├── src
│   └── main.cpp
└── thirdparty 
~~~


