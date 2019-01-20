|Linux|Windows|
|:----|:------|
[![Linux build status][1]][2] | [![Windows build status][3]][4] |
[1]: https://travis-ci.com/injae/cppm.svg?branch=master
[2]: https://travis-ci.com/injae/cppm
[3]: https://ci.appveyor.com/api/projects/status/6ovjp02higajbxhm?svg=true
[4]: https://ci.appveyor.com/project/injae/cppm
[5]:
[6]:

Cppm 
========
> c++ cmake project manger  
> cppm.toml file convert to CMakeLists.txt file   
> c++ dependency auto install, only use cmake files  
[Document](https://injae.github.io/cppm/)

Features
========
- [x] cmake dependencies auto installer (cppkg)
- [x] generate build command (cppm build {options})
- [x] Cppkg package search (cppm search)
- [x] cmake project initialize (cppm init {options} {name})
- [x] regist cppkg package in local repository (cppm add cppkg {name})
- [x] cppkg repository update (cppm update)
- [x] hunter package dependency available 

## Dependencies
1. cmake-3.10.0
2. hunter package [hunter](https://github.com/ruslo/hunter)  
2. c++-17-compiler (g++7 or clang7)
3. boost
4. nlpo
5. cpptoml
6. fmt

## Installation
### Linux
> dependencies is auto install but boost compile is too slow
> if you want to fast install
> use linux package manager
### Ubuntu example
```
git clone https://github.com/INJAE/cppm.git
cd cppm
sudo cmake -H. -Bbuild
cd build
sudo cmake --build . --target install
```

# cppm.toml
## package
```
[package]
 name = "example"        # user package name
 version = "1.0.0"       # user package version
 description = "example" # package description
 c++-version = "17"      # default 17 
 ```

### cmake
> ccache auto use
```
[cmake]
version  = "3.10"    # cmake minimum version
include = ["cmake/cppm_install.cmake","..."] # include cmake files
option   = "..."        # cmake options
```
### hunter
> hunter package can use
```
[hunter]
url = "..." # default url: https://github.com/ruslo/hunter/archive/v0.23.89.tar.gz
sha1 =  "..." # default sha1: a370290a8b32987755a36d64807bf8c7f5b61730
```

### compiler
```
[compiler]
clang++ = {option = "..."} ## compiler options
g++     = {option = "..."} ## compiler options
```
### bin
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
```
[[lib]]
name   = "nlpo"   # lib name
type   = "static" # lib type , static or shared or header-only
source = ["src/.*"] # source files 
install = # true and false (default => true) value is defulat can't install
```
### dependencies
> add thirdparty library dependencies  
> {0} = {module = {1}, version = {2} components = {3}} == find_package({0} {2} components = {2})  
> target_link_libraries("project" PUBLIC {0})  
> {1}: necessary  
> {2}: default => lastest  
> The 'no_module' option is used when target does not have module name 
> and is linked with cmake value.
```
[dependencies]
cpptoml = {module = "cpptoml"} # cmake option is library name in cmake
Boost   = {module = " ${Boost_LIBRARIES}", components="system filesystem", no_module=true} 
fmt     = {module = "fmt::fmt" HUNTER} # search and install to use hunter package manager
nlpo    = {module = "nlpo::nlpo}"
```

# Cppkg
> cmake dependencies auto install package

## Usage
### Add dependencies
> example: exam  
> default cppm use package repo => [cppkg](https://github.com/injae/cppkg.git)  
1. search package  
```
cppm cppkg search exam
Name           Version             Description                             Use                                                                   
=================================================================================================================================================
exam           lastest             example package                         exam = {module="exam::exam", version="lastest"}
```
2. add this option in cppm.toml

cppm.toml
```
[dependencies]
exam = {module="exam::exam", version="lastest"}
```

if you can't find package add new package in local repo

### Generate New Package Auto Installer and Add your local repository
```
cppm cppkg init exam
```
1. edit exam.toml file

exam.toml
``` 
[package]
    name     = "exam"
    version  = "lastest" # git repo version is lastest
    description = "cppkg example"
    cmake    = {name = {exam cmake library name}}
    download = {git="{git repo}"} # or url
```
2. build exam.toml
```
cppm cppkg build exam
```
Result
```
. exam
+-- lastest/
|   +-- cppkg.toml  # == exam.toml
|   +-- eaxm.cmake.in # cppkg library auto installer
+-- {other version}
```
3. add other options in cppkg.toml and eaxm.cmake.in

### cppkg.toml
```
[package]
    name     = "exam"
    version  = "lastest" # git repo version is lastest
    description = "cppkg example"
    global = false
    cmake    = {name = {exam cmake library name}, finlib={Findlib.cmake file}}
    download = {git="{git repo}"} # or url
```
cmake.findlib options value auto install your project cmake/Modules path  
Find{name}.cmake is none cmake project finder  

exam.cmake.in
```
cmake_minimum_required(VERSION 3.10)
project(exam--install NONE)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cppm_tool.cmake)
download_package(exam lastest GIT https://github.com/exam/exam.git   LOCAL)
# Other Options:
# - Linux Configure:
#    L_CONFIGURE {...}, L_BUILD {...}, L_INSTALL {...}
# - Windows Configure: 
#    W_CONFIGURE {...}, W_BUILD {...}, W_INSTALL {...}
# - Install Path Option:
#    LOCAL(default) GLOBAL
#    
```
4. add your local cppkg repository
```
cppm cppkg push exam
```
 if dependency is cmake base project, you can add too simple  
 git repo default version is lastest  
 url is not have default version, need version  
```
cppm cppkg init -g {git repo} -m exam::exam -p exam
#or
cppm cppkg init -u {zip url} -v {version} -m exam::exam -p exam 
```


