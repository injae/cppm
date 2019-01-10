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
 C++ Cmake Project Helper
-------------------------------------
## 설명
cppm.toml 파일을 CMAKE 프로젝트로 변환시켜줍니다.

## 목표
간단한 조작으로 멀티플렛폼에서 C++의 사용을  
Rust의 Cargo 처럼 편하게 사용하는것이 목표입니다.  

## Dependencies
1. cmake-3.10.0
2. c++-17-compiler (g++7 or clang7)
3. boost
4. nlpo
5. cpptoml
6. fmt

## 설치
### Linux
일반적으로 설치를 하게 되면 자동으로 부스트 라이브러리를 컴파일 하기 때문에  
설치하는데 시간이 많이 걸립니다. 그래서 빠른설치를 위해 OS 패키지 매니저로  
Boost 라이브러리를 설치한 후 설치를 하는것을 권장합니다.  
### Ubuntu 예제
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
프로젝트 기본설정
```
[package]
 name = "example"        # 패키지의 이름입니다.
 version = "1.0.0"       # 패키지의 버전입니다.
 description = "example" # 패키지의 설명입니다.
 cpp_version = "14" # 사용하는 c++ 버전을 넣습니다. Example: 98 11 14 17  기본값 14
 ```

### cmake
Cmake 관련한 설정  
ccache는 자동으로 감지해서 설치가 되어있으면 알아서 사용하게 되있습니다.
```
[cmake]
version  = "3.10"    # cmake 최소 버전 CPPM은 최소 3.10.0이 필요합니다. 이유는 Vcpkg가 3.10.0 부터 지원하기 때문에 사용의 편리함을 위해 기본을 3.10.0으로 설정했습니다.  
option   = ""        # 빌드할 때 사용할 CMake 옵션입니다. 예시 -DCMAKE_TOOLCHAIN_FILE=xxx 를 넣으면 cmake -DCMAKE_TOOLCHAIN_FILE=xxx 와 같은 결과가 발생됩니다.  
```

### compiler
컴파일러 옵션 설정  
여기서 설정한 옵션은 CMakeLists.txt에 저장이 됩니다.
```
[compiler]
clang++ = {option = "-std=c++17" ## compiler options
          ,version = "7.0"}      ## compiler minimum version
```
### bin
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

### lib
Library 프로젝트 설정  
여러개를 설정가능합니다.
```
[[lib]]
name   = "nlpo"   # lib name
type   = "static" # lib type , static or shared or header-only
install = # true and false (default => true)
source = ["src/.*"] # source files 
```
### dependencies
프로젝트의 Dependencies 설정  
각각 인자는 아래와 같이 변환됩니다.  
{0} = {module = {1}, version = {2} components = {3}} == find_package({0} {2} components = {2})  
target_link_libraries("project" PUBLIC {0})  
이렇게 변환 됩니다.  
module 인자는 필수인자입니다.  
만약 module인자에 들어갈 값을 못찾으면  
/usr/local/lib/cmake/{0}/ or ~/.cppm/local/cmake/{0} 에서 라이브러리 이름을 찾아   
{0}config.cmake나 {0}target.cmaked에서 찾으시면 됩니다.  
```
[dependencies]
cpptoml = {module = "cpptoml"}
Boost   = {module = " ${Boost_LIBRARIES}", components="system filesystem"}
fmt     = {module = "fmt::fmt"}
nlpo    = {module = "nlpo::nlpo}"
```
