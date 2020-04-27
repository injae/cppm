
[Cppm](https://injae.github.io/cppm/) ***(BETA)*** 
========
|Linux|Windows|MacOS|
|:----|:------|:----|
![Linux](https://github.com/injae/cppm/workflows/Linux/badge.svg) | ![Windows](https://github.com/injae/cppm/workflows/Windows/badge.svg) | ![MacOS](https://github.com/injae/cppm/workflows/MacOS/badge.svg) |

![](cppm_demo.gif)
> My native language is not English. Please let me know if you have any problems with grammar.
> This project was inspired by Rust's Cargo.  
> So the configuration of the configuration file is almost similar to Rust's Cargo.toml.
> Cppm is a cross platform C++ project manager like Rust's Cargo.   
> Cmake is as difficult to use as package manager in other languages.   
> So, like other package managers, to manage the C++ project,   
> we created the cppm.toml file so that we could create the Cmake project.   
> Cppm is made up of C++ and CMake only.   
> So with a Cmake and a C++ Compiler, you can build it anywhere on the platform that Cmake supports.  
> The goal of this project is to make the C++ project as comfortable as Rust's Cargo.  
> Not many libraries are supported by the cppkg yet.   
> So cppm made the package of the [Hunter Package Manager](https://github.com/ruslo/hunter) available.   
> If there is a library you want to add, please issue a pull request to the [Cppkg repository](https://github.com/injae/cppkg)  
> Any cppm.toml documentation that is lacking due to time issues would be appreciated   
> if you refer to Cargo.toml documentation or contact me. Documents will be updated as soon as possible.  

## Installation
### Ubuntu
```
sudo apt-get install build-essential
git clone https://github.com/injae/cppm.git
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
```
# AppleClang not working (can't use filesystem and optional)
brew install llvm cmake
# Adding to llvm clang env
# export CC=#llvm clang path
# export CXX=#llvm clang++ path 
git clone https://github.com/injae/cppm.git
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
```
# need visual studio , git , cmake
# scoop install git cmake
git clone https://github.com/injae/cppm.git
cd cppm
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cd build
cmake --build . --config Release
cd Release
./cppm build install
# Add System Path \.cppm\bin
```

## Features
- [x] cmake dependencies auto installer (cppkg)
- [x] generate build command (cppm build {options})
- [x] Cppkg package search (cppm search)
- [x] cmake project initialize (cppm init {options} {name})
- [x] regist cppkg package in local repository (cppm add cppkg {name})
- [x] cppkg repository update (cppm update)
- [x] hunter package dependency available 
- [x] sub project option
- [x] unit test option

## Document
### [GitBook](https://cppm.gitbook.io/project/)
### Beta: [GitBook](https://injae.github.io/cppm/)



