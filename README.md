Cppm [![Build Status](https://travis-ci.com/injae/cppm.svg?branch=master)](https://travis-ci.com/injae/cppm)
========
 C++ cmake project helper
-------------------------------------
## Other language README
[Korean](./document/README-ko.md)

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
### cppm options and commands
1. cppm build
compile to use cppm.toml
2. cppm init
make c++ cmake project
3. cppm add toolchain {args}
this command add build option -DCMAKE_TOOLCHAIN_FILE={args}
{args} save ~/.cppm/config.toml file

4. cppm install --> working

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
builder  = "ninja"   # cppm use builder name, default make
compiler = "clang++" # cppm use compiler name, default g++
```

### compiler
compiler option setting
```
[compiler]
clang++ = {option = "-std=c++17" ## compiler options
          ,version = "7.0"}      ## compiler minimum version
```
### builder
builder option setting
```
[builder]
ninja = {option = "-j4"}
```
### bin
make binary 
```
[[bin]]
name   = "cppm" # bin name
source = ["src/.*"] # source files
[[bin]]
name   = "tbin" # bin name
source = ["src/.*"] # source files
```

### lib
make library
```
[[lib]]
name   = "nlpo"   # lib name
type   = "static" # lib type , static or shared or header-only
source = ["src/.*"] # source files 
```
### dependencies
add thirdparty library dependencies
```
[dependencies]
cpptoml = {cmake = "cpptoml"} # cmake option is library name in cmake
Boost   = {cmake = " ${Boost_LIBRARIES}", components="system filesystem"} # components cmake library components
fmt     = {cmake = "fmt::fmt"}
nlpo    = {cmake = "nlpo::nlpo}"
```

