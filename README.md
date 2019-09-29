
Cppm [![gitbook][5]][6]
========
|Linux|Windows|
|:----|:------|
[![Linux build status][1]][2] | [![Windows build status][3]][4] | 

[1]: https://travis-ci.com/injae/cppm.svg?branch=master
[2]: https://travis-ci.com/injae/cppm
[3]: https://ci.appveyor.com/api/projects/status/6ovjp02higajbxhm?svg=true
[4]: https://ci.appveyor.com/project/injae/cppm
[5]: https://aleen42.github.io/badges/src/gitbook_2.svg
[6]: https://cppm.gitbook.io/project/

![](cppm_demo.gif)
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

## Dependencies
1. cmake (minimum 3.6)  
2. [Hunter Package Manager](https://github.com/ruslo/hunter)  
2. c++-17-compiler (g++7 or clang7 or visual studio 2017)
3. boost
4. nlpo
5. cpptoml
6. fmt

## Installation
### Ubuntu
```
sudo apt-get install build-essential
git clone https://github.com/INJAE/cppm.git
cd cppm
cmake -H. -Bbuild
cd build
cmake --build . 
./cppm build install
# Adding to cppm path
export PATH="$HOME/.cppm/local/bin:$PATH"
```
### Windows
```
# need visual studio 
git clone https://github.com/INJAE/cppm.git
cd cppm
cmake -H. -Bbuild
cd build
cmake --build . 
./cppm build install
# Adding to Window Path
setx /M path=%path%;%homepath%\.cppm\local\bin
```
## OSX
AppleClang is not supported.  
AppleClang does not support the C++ standard perfectly.   
Set GCC or Clang as the default compiler to compile on OSX.  
Ex. #include<optional> is not available.   

## Features
- [x] cmake dependencies auto installer (cppkg)
- [x] generate build command (cppm build {options})
- [x] Cppkg package search (cppm search)
- [x] cmake project initialize (cppm init {options} {name})
- [x] regist cppkg package in local repository (cppm add cppkg {name})
- [x] cppkg repository update (cppm update)
- [x] hunter package dependency available 
- [ ] unit test option
- [ ] sub project option

## Document
### [GitBook](https://cppm.gitbook.io/project/)



