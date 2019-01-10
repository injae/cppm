Cppm 
========
|Linux|Windows|
|:----|:------|
[![Linux build status][1]][2] | [![Windows build status][3]][4] |

[1]: https://travis-ci.com/injae/cppm.svg?branch=master
[2]: https://travis-ci.com/injae/cppm
[3]: https://ci.appveyor.com/api/projects/status/6ovjp02higajbxhm?svg=true
[4]: https://ci.appveyor.com/project/injae/cppm

-------------------------------------
 C++ cmake project helper
-------------------------------------
[한국어](./document/README-ko.md)

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
sudo apt-get install liboost-all-dev
```

```
git clone https://github.com/INJAE/cppm.git
cd cppm
sudo cmake -H. -Bbuild
sudo cmake --build . --target install
```

## Document

## cppm.toml
### package
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
clang++ = {option = "-std=c++17" ## compiler options
          ,version = "7.0"}      ## compiler minimum version
```
### bin
make binary 
```
[[bin]]
name   = "cppm" # bin name
source = ["src/.*"] # source files
install = # true and false (default => true)
[[bin]]
name   = "tbin" # bin name
source = ["src/.*"] # source files
install = # true and false (default => true)
```

### lib
make library
```
[[lib]]
name   = "nlpo"   # lib name
type   = "static" # lib type , static or shared or header-only
install = # true and false (default => true)
source = ["src/.*"] # source files 
```
### dependencies
add thirdparty library dependencies
```
[dependencies]
cpptoml = {module = "cpptoml"} # cmake option is library name in cmake
Boost   = {module = " ${Boost_LIBRARIES}", components="system filesystem"} # components cmake library components
fmt     = {module = "fmt::fmt"}
nlpo    = {module = "nlpo::nlpo}"
```

