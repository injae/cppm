Cppm 
========
 C++ cmake project helper
-------------------------------------
## Goals
like Rust Cargo 

## description
toml file convert CMakelists file

## Dependencies
1. boost
2. nlpo
3. cpptoml
4. fmt

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
 name = "example"     # user package name
 version = "1.0.0"    # user package version
 description = "example" # package description
 ```

### cmake
cmake setting
ccache auto use
```
[cmake]
version = "3.10"  ## cmake minimum version
option = ""       ## cmake options
builder = "ninja" ## cppm use builder name, default make
compiler = "clang++"  ## cppm use compiler name, default g++
```

### compiler
compiler option setting
```
[compiler]
clang++ = {option = "-std=c++17" ## compiler options
          ,version = "7.0"} ## compiler minimum version
```
### 5. builder
builder option setting
```
[builder]
ninja = {option = "-j4"}
```
### 6. bin
```
[[bin]]
name   = "cppm" # bin name
source = ["include/.*" ,"src/.*"] # source files
[[bin]]
name   = "tbin" # bin name
source = ["include/.*" ,"src/.*"] # source files
```

### 7. lib
```
[[lib]]
name   = "nlpo" # lib name
type = "static" # lib type , static or shared or headonly
source = ["include/.*" ,"src/.*"] # source files
```
### 8. dependencies
```
[dependencies]
cpptoml = {cmake = "cpptoml"}
Boost   = {cmake = " ${Boost_LIBRARIES}", components="system filesystem"}
fmt     = {cmake = "fmt::fmt"}
nlpo    = {cmake = "nlpo::nlpo"
```

