|Linux|Windows|
|:----|:------|
[![Linux build status][1]][2] | [![Windows build status][3]][4] | 

[1]: https://travis-ci.com/injae/cppm.svg?branch=master
[2]: https://travis-ci.com/injae/cppm
[3]: https://ci.appveyor.com/api/projects/status/6ovjp02higajbxhm?svg=true
[4]: https://ci.appveyor.com/project/injae/cppm
[5]: https://aleen42.github.io/badges/src/gitbook_2.svg
[6]: https://cppm.gitbook.io/project/

Cppm [![gitbook][5]][6]
========
> c++ cmake project manger  
> cppm.toml file convert to CMakeLists.txt file   
> c++ dependency auto install, only use cmake files  

## Features
- [x] cmake dependencies auto installer (cppkg)
- [x] generate build command (cppm build {options})
- [x] Cppkg package search (cppm search)
- [x] cmake project initialize (cppm init {options} {name})
- [x] regist cppkg package in local repository (cppm add cppkg {name})
- [x] cppkg repository update (cppm update)
- [x] hunter package dependency available 

## Dependencies
1. cmake (minimum 3.6)
2. hunter package [hunter](https://github.com/ruslo/hunter)  
2. c++-17-compiler (g++7 or clang7)
3. boost
4. nlpo
5. cpptoml
6. fmt

## Installation
### Ubuntu example
```
sudo apt-get install build-essential
git clone https://github.com/INJAE/cppm.git
cd cppm
sudo cmake -H. -Bbuild
cd build
sudo cmake --build . --target install
```
### Windows example
```
git clone https://github.com/INJAE/cppm.git
cd cppm
sudo cmake -H. -Bbuild
cd build
sudo cmake --build . --target install
```
## Document
### [GitBook](https://cppm.gitbook.io/project/)

