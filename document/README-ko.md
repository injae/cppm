Cppm [![Build Status](https://travis-ci.com/injae/cppm.svg?branch=master)](https://travis-ci.com/injae/cppm)
========
 C++ Cmake Project Helper
-------------------------------------
## 설명
toml file convert CMakelists file

## 목표
러스트 Cargo처럼 동작하는게 목표입니다.

## Dependencies
1. cmake-3.10.0
2. c++-17-compiler
3. boost
4. nlpo
5. cpptoml
6. fmt

## 설치
### Linux
일반적으로 설치를 하게 되면 자동으로 부스트 라이브러리를 컴파일 하기때문에
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
### cppm option과 command들
1. cppm build
cppm.toml을 바탕으로 c++ cmake 프로젝트를 빌드합니다.
2. cppm init
cppm 프로젝트를 생성합니다.
make c++ cmake project
3. cppm add toolchain {args}
이 커맨드는 Cmake 툴체인을 추가할때 사용합니다.
-DCMAKE_TOOLCHAIN_FILE={args}
Cmake 위와같은 CMake 옵션을 빌드할때 추가합니다.
그리고 툴체인은 ~/.cppm/config.toml 파일에 저장됩니다.

4. cppm install --> 작업중

## cppm.toml
### package
프로젝트 기본설정
```
[package]
 name = "example"        # 패키지의 이름입니다.
 version = "1.0.0"       # 패키지의 버전입니다.
 description = "example" # 패키지의 설명입니다.
 ```

### cmake
Cmake 관련한 설정
ccache는 자동으로 감지해서 설치가 되어있으면 알아서 사용하게 되있습니다.
```
[cmake]
version  = "3.10"    # cmake 최소 버전 CPPM은 최소 3.10.0이 필요합니다. 이유는 Vcpkg가 3.10.0 부터 지원하기 때문에 사용의 편리함을 위해 기본을 3.10.0으로 설정했습니다.
option   = ""        # 빌드할 때 사용할 CMake 옵션입니다. 예시 -DCMAKE_TOOLCHAIN_FILE=xxx 를 넣으면 cmake -DCMAKE_TOOLCHAIN_FILE=xxx 와 같은 결과가 발생됩니다.
builder  = "ninja"   # cppm이 빌드할 때 사용하는 Builder를 설정합니다. make 혹은 ninja가 사용가능합니다. 기본은 make입니다.
compiler = "clang++" # cppm이 빌드할 때 사용하는 Compiler를 설정합니다. g++ 혹은 clang++를 사용합니다.  기본은 g++입니다.
```

### compiler
컴파일러 옵션 설정
여기서 설정한 옵션은 CMakeLists.txt에 저장이 됩니다.
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
