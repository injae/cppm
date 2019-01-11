Cppm 
========
|Linux|Windows|
|:----|:------|
[![Linux build status][1]][2] | [![Windows build status][3]][4] |

[1]: https://travis-ci.com/injae/cppm.svg?branch=master
[2]: https://travis-ci.com/injae/cppm
[3]: https://ci.appveyor.com/api/projects/status/6ovjp02higajbxhm?svg=true
[4]: https://ci.appveyor.com/project/injae/cppm

[한국어](./document/README-ko.md)

Cppm 
========
> c++ cmake project helper
> cppm.toml 파일을 CMAKE 프로젝트로 변환시켜줍니다.   

Features
========
- [x] cmake dependencies auto installer (cppkg)
- [x] generate build command (cppm build {options})
- [x] Cppkg package search (cppm search)
- [x] cmake project initialize (cppm init {options} {name})
- [x] regist cppkg package in local repository (cppm add cppkg {name})
- [x] cppkg repository update (cppm update)
- [ ] Cppkg package search caching

## description
toml file convert CMakelists file

## Goals
like Rust Cargo 

## Dependencies
1. cmake-3.10.0
2. c++-17-compiler (g++7 or clang7)
3. boost
4. nlpo
5. cpptoml
6. fmt

## Installation
### Linux
dependencies is auto install but boost compile is too slow
if you want to fast install
use linux package manager
### Ubuntu example
```
sudo apt-get install liboost-all-dev #fast install, boost compiler very slow
```

```
git clone https://github.com/INJAE/cppm.git
cd cppm
sudo cmake -H. -Bbuild
sudo cmake --build . --target install
```

Document
========
# cppm.toml
## package
```
[package]
 name = "example"        # user package name
 version = "1.0.0"       # user package version
 description = "example" # package description
 ```

### cmake
cmake setting
ccache auto use
```
[cmake]
version  = "3.10"    # cmake minimum version
option   = ""        # cmake options
```

### compiler
compiler option setting
```
[compiler]
clang++ = {option = ""} ## compiler options
g++     = {option = ""} ## compiler options
```
### bin
make binary 
```
[[bin]]
name   = "cppm" # bin name
source = ["src/.*"] # source files
install = # true and false (default => true) value is defulat can't install
[[bin]]
name   = "tbin" # bin name
source = ["src/.*"] # source files
install = # true and false (default => true) value is defulat can't install
```

### lib
make library
```
[[lib]]
name   = "nlpo"   # lib name
type   = "static" # lib type , static or shared or header-only
source = ["src/.*"] # source files 
install = # true and false (default => true) value is defulat can't install
```
### dependencies
add thirdparty library dependencies
{0} = {module = {1}, version = {2} components = {3}} == find_package({0} {2} components = {2})  
target_link_libraries("project" PUBLIC {0})  
{1}: necessary
{2}: default => lastest
```
[dependencies]
cpptoml = {module = "cpptoml"} # cmake option is library name in cmake
Boost   = {module = " ${Boost_LIBRARIES}", components="system filesystem"}
fmt     = {module = "fmt::fmt"}
nlpo    = {module = "nlpo::nlpo}"
```

# Cppkg
cmake dependencies auto install package

## Cppkg Structure

## Usage
### Add dependencies
Example: exam
search package
default cppm use package repo => [cppkg](https://github.com/injae/cppkg.git)
```
cppm search
Name           Version             Description                             Use                                                                   
=================================================================================================================================================
exam           lastest             example package                         exam = {module="exam::exam", version="lastest"}
```
add this option in cppm.toml

cppm.toml
```
[dependencies]
exam = {module="exam::exam", version="lastest"}
```
if can't find package
add new package in local repo

```
cppm init -d exam
```
and edit exam.toml file

exam.toml
``` 
[package]
    name     = "exam"
    version  = "lastest" # git repo version is lastest
    description = "cppkg example"
    cmake    = {name = {exam cmake library name}}
    download = {git="{git repo}"} # or url
```
build exam.toml
```
cppm build -D exam
```
Result
```
. exam
+-- lastest/
|   +-- cppkg.toml  # == exam.toml
|   +-- eaxm.cmake.in # cppkg library auto installer
+-- {other version}
```
add other options in cppkg.toml and eaxm.cmake.in

cppkg.toml
```
[package]
    name     = "exam"
    version  = "lastest" # git repo version is lastest
    description = "cppkg example"
    cmake    = {name = {exam cmake library name}, finlib={Findlib.cmake file}}
    download = {git="{git repo}"} # or url
```
cmake.findlib options value auto install your project cmake/Modules path
Find{name}.cmake is none cmake project finder

exam.cmake.in
```
cmake_minimum_required(VERSION 3.10)
project(exam--install NONE)

find_package(exam QUIET)
if(NOT exam_FOUND AND NOT exam_FIND_VERSION_EXACT)
    include(ExternalProject)
    if(NOT WIN32)
 # Linux or OSX Setting
        ExternalProject_Add(
        exam
        GIT_REPOSITORY {git repo}
        SOURCE_DIR repo
        # Defulat cppkg install path if you want to install global path, remove this options
        #if you want local install add CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=$ENV{HOME}/.cppm/local
        CMAKE_ARGS "${CMAKE_ARGS}"
        #CONFIGURE_COMMAND
        #BUILD_COMMAND 
        #INSTALL_COMMAND 
        BUILD_IN_SOURCE true
        )
    else(NOT WIN32)
 # Windows Setting
        ExternalProject_Add(
        exam
        GIT_REPOSITORY {git repo}
        SOURCE_DIR repo
        # Defulat cppkg install path if you want to install global path, remove this options
        #if you want local install add CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=$ENV{HOME}/.cppm/local
        CMAKE_ARGS "${CMAKE_ARGS}"
        #CONFIGURE_COMMAND
        #BUILD_COMMAND 
        #INSTALL_COMMAND 
        BUILD_IN_SOURCE true
        )
    endif(NOT WIN32)
endif()
```





