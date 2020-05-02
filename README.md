
[Cppm](https://injae.github.io/cppm/) ***(BETA)*** [Documents](https://injae.github.io/cppm/)
========
|Linux|Windows|MacOS|
|:----|:------|:----|
![Linux](https://github.com/injae/cppm/workflows/Linux/badge.svg) | ![Windows](https://github.com/injae/cppm/workflows/Windows/badge.svg) | ![MacOS](https://github.com/injae/cppm/workflows/MacOS/badge.svg) |

![](cppm_demo.gif)
> Cppm is a cross platform C++ project manager like Rust's Cargo.   
> This project was inspired by Rust's Cargo.  
> So the configuration of the configuration file is almost similar to Rust's Cargo.toml.
> CMake is as difficult to use as package manager in other languages.   
> So, like other package managers, to manage the C++ project,   
> we created the cppm.toml file so that we could create the CMake project.   
> Cppm is made up of C++ and CMake only.   
> So with a Cmake and a C++ Compiler, you can build it anywhere on the platform that Cmake supports.  
> The goal of this project is to make the C++ project as comfortable as Rust's Cargo.  
> Not many libraries are supported by the cppkg yet. But cppkg package make easy  
> So cppm made the package of the [Hunter Package Manager](https://github.com/ruslo/hunter) available.   
> If there is a library you want to add, please issue a pull request to the [Cppkg repository](https://github.com/injae/cppkg)  
> Cppm auto detect vcpkg toolchain
> My native language is not English. Please let me know if you have any problems with grammar.
> Any cppm.toml documentation that is lacking due to time issues would be appreciated   
> if you refer to Cargo.toml documentation or contact me. Documents will be updated as soon as possible.  

## Easy Config file
```toml
# cppm project config file
[package]
name    = "cppm"
version = "0.0.10"
description = "c++ cmake maker use to toml like Cargo"
git = "https://github.com/injae/cppm.git"

[cmake]
include  = ["cmake/cppm_install.cmake"]

[lib]
name = "cppm-core"
source = ["src/core/.*", "src/util/.*", "src/cppkg/.*"]

[[bin]]
name   = "cppm"
source = ["src/cmake/.*","src/option/.*", "src/util/.*", "src/main.cpp"]

[[test]]
name   = "cppm-test-check"
source = ["tests/test.cpp"]

[[example]]
name   = "cppm-core-load"
source = ["examples/example.cpp"]

[workspace]
    member = ["libs/nlpo", "libs/tomlpp"]

[dependencies]
fmt      = "6.2.0"
hashpp   = "git"
range-v3 = "git"

[dev-dependencies]
    Catch2 = "2.9.1"
```

## Easy to make Unregistered Package Add
```console
$ cppm cppkg init -U "https://github.com/fmtlib/fmt/releases/download/6.2.0/fmt-6.2.0.zip" fmt
```
```toml
# fmt/6.2.0/cppkg.toml
[fmt]
version = "6.2.0"
description = "A modern formatting library"
module = "fmt::fmt"
url="https://github.com/fmtlib/fmt/releases/download/6.2.0/fmt-6.2.0.zip"
```

```console
$ cppm cppkg init -U "https://github.com/fmtlib/fmt.git" fmt
```
```toml
# fmt/git/cppkg.toml
[fmt]
version = "git"
description = "A modern formatting library"
module = "fmt::fmt"
git="https://github.com/fmtlib/fmt.git"
flags="-DFMT_DOC=OFF -DFMT_TEST=OFF -DFMT_FUZZ=OFF" # (optional) for cmake build fast flags
```

## Installation
### Ubuntu
```console
sudo apt-get install build-essential
git clone --recurse-submodules https://github.com/injae/cppm.git
cd cppm
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cd build
cmake --build . --config Release
cd Release
./cppm build install
# Adding to cppm path
export PATH="$HOME/.cppm/bin:$PATH"
```


### Macos
```console
# AppleClang not working (can't use filesystem and optional)
brew install llvm cmake
# Adding to llvm clang env
# export CC=#llvm clang path
# export CXX=#llvm clang++ path 
git clone --recurse-submodules https://github.com/injae/cppm.git
cd cppm
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cd build
cmake --build . --config Release
cd Release
./cppm build install
# Adding to cppm path
export PATH="$HOME/.cppm/bin:$PATH"
```
### Windows
```console
# need visual studio , git , cmake
# scoop install git cmake
git clone --recurse-submodules https://github.com/injae/cppm.git
cd cppm
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cd build
cmake --build . --config Release
cd Release && ./cppm build
cd ../ && cmake --install .
# Add System Path %USERPROFILE%\.cppm\bin
```

## Features
- [x] easy configure file (cppm.toml)
- [x] generate build command (cppm build {options})
- [x] cmake dependencies auto installer (cppkg)
- [x] easy cppkg file generator (cppm cppkg init)
- [x] no sudo, package local install path (~/.cppm/)
- [x] Cppkg package search (cppm search)
- [x] cmake project initialize (cppm init {options} {name})
- [x] cppkg repository update (cppm update)
- [x] hunter package dependency available 
- [x] auto detect vcpkg 
- [x] sub project option (cppm.toml [workspace])
- [x] unit test option (cppm test)

## Document
### [GitBook](https://injae.github.io/cppm/)
