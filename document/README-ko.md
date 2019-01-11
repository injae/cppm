|Linux|Windows|
|:----|:------|
[![Linux build status][1]][2] | [![Windows build status][3]][4] |

[1]: https://travis-ci.com/injae/cppm.svg?branch=master
[2]: https://travis-ci.com/injae/cppm
[3]: https://ci.appveyor.com/api/projects/status/6ovjp02higajbxhm?svg=true
[4]: https://ci.appveyor.com/project/injae/cppm

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
- [ ] 
- [ ] 
- [ ] Cppkg package search caching


Dependencies
============
1. cmake-3.10.0
2. c++-17-compiler (g++7 or clang7)
3. boost
4. nlpo
5. cpptoml
6. fmt

설치
# Linux
일반적으로 설치를 하게 되면 자동으로 부스트 라이브러리를 컴파일 하기 때문에  
설치하는데 시간이 많이 걸립니다. 그래서 빠른설치를 위해 OS 패키지 매니저로  
Boost 라이브러리를 설치한 후 설치를 하는것을 권장합니다.  
## Ubuntu 예제
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
프로젝트 기본설정
```
[package]
 name = "example"        # 패키지의 이름입니다.
 version = "1.0.0"       # 패키지의 버전입니다.
 description = "example" # 패키지의 설명입니다.
 cpp_version = "14" # 사용하는 c++ 버전을 넣습니다. Example: 98 11 14 17  기본값 14
 ```

## cmake
Cmake 관련한 설정  
ccache는 자동으로 감지해서 설치가 되어있으면 알아서 사용하게 되있습니다.
```
[cmake]
version  = "3.10"    # cmake 최소 버전 CPPM은 최소 3.10.0이 필요합니다. 이유는 Vcpkg가 3.10.0 부터 지원하기 때문에 사용의 편리함을 위해 기본을 3.10.0으로 설정했습니다.  
option   = ""        # 빌드할 때 사용할 CMake 옵션입니다. 예시 -DCMAKE_TOOLCHAIN_FILE=xxx 를 넣으면 cmake -DCMAKE_TOOLCHAIN_FILE=xxx 와 같은 결과가 발생됩니다.  
```

## compiler
컴파일러 옵션 설정  
여기서 설정한 옵션은 CMakeLists.txt에 저장이 됩니다.
```
[compiler]
clang++ = {option = ""} ## compiler options
g++ = {option = ""} ## compiler options
```
## bin
Binary 프로젝트 설정  
여러개를 설정가능합니다.
```
[[bin]]
name   = "cppm" # bin name
source = ["src/.*"] # source files
install = # true and false (default => true) 이 설정을 false로 할경우 install할때 설치가 되지않습니다. 빌드 디렉토리에만 설치가 됩니다.
[[bin]]
name   = "tbin" # bin name
install = # true and false (default => true)
source = ["src/.*"] # source files
```

## lib
Library 프로젝트 설정  
여러개를 설정가능합니다.
```
[[lib]]
name   = "nlpo"   # lib name
type   = "static" # lib type , static or shared or header-only
source = ["src/.*"] # source files 
install = # true and false (default => true)
```
## dependencies
프로젝트의 Dependencies 설정  
각각 인자는 아래와 같이 변환됩니다.  
{0} = {module = {1}, version = {2} components = {3}} == find_package({0} {2} components = {2})  
target_link_libraries("project" PUBLIC {0})  
{1}: 필수
{2}: 기본값 lastest
```
[dependencies]
cpptoml = {module = "cpptoml", version="lastest"}
Boost   = {module = "${Boost_LIBRARIES}", components="system filesystem"}
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





