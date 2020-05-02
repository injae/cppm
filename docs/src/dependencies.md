# Dependencies
***[cppkg](https://github.com/injae/cppkg)*** is cppm central package repository  
cppm use it by default to find requested packages  
if you want to use ***[Hunter Package Manager](https://github.com/cpp-pm/hunter)*** manager set repo="hunter"  
cppm auto detect vcpkg   
you can find package with ***cppm cppkg search***  
```console
$ cppm cppkg search -h
Usage:
   cppm cppkg search [--verbose]

Option:
   --all [-a]                      :show all package 
   --help [-h]                     :show cppm commands and options
   --repo [-r] {repo name}         :show {repo_name}'s packages  # cppkg or hunter
```

## Adding a Dependency
add ***[dependencies]*** in your ***cppm.toml***file 
```toml
[dependencies]
fmt = "6.2.0"
```
Re-run ***cppm build***, and Cppm Search yout package thirdparty directory ***thirdparty/fmt/6.2.0/cppkg.toml***  
if can't find it cppm search from ***[cppkg](https://github.com/injae/cppkg)*** repository and copy to ***thirdparty/fmt/6.2.0/cppkg.toml***

You can easily add unregistered packages

If you want to add unregistered package need to make cppkg.toml file
cmake base unregistered package is only need 3 config ***version*** ***git or url*** *** module ***
no cmake base package need to [custom downloader](#custom-downloader)
cppm provides commands to create cppkg.toml
# cppkg file generate command
```console
$ cppm run cppkg init -h                                                                                                             
Usage:
   cppm cppkg init [--verbose]

Option:
   --des [-d] {description}        :add description    
   --flags [-f] {arg}              :add cmake build flags
   --git [-g] {repo}               :add git repo       
   --git_tag [-t] {tag}            :add git tag        
   --help [-h]                     :show cppm commands and options
   --module [-m] {module}          :add module name    
   --type [-t] {arg}               :add type default is lib
   --uri [-U] {arg}                :auto detect uri or git and version
   --url [-u] {url}                :add url repo Require --version
   --version [-v] {version}        :add library version Require --version
```

### Example fmt library
6.2.0 version config  
```toml
# thirdparty/fmt/6.2.0/cppkg.toml
[fmt]
version = "6.2.0"
description = "A modern formatting library"
module = "fmt::fmt"
url="https://github.com/fmtlib/fmt/releases/download/6.2.0/fmt-6.2.0.zip"
flags="-DFMT_DOC=OFF -DFMT_TEST=OFF -DFMT_FUZZ=OFF" # (optional) for cmake build fast flags
```

git repository version  
git version dependency always version check in CMake build step  
```toml
# thirdparty/fmt/git/cppkg.toml
[fmt]
version = "git"
description = "A modern formatting library"
module = "fmt::fmt"
git="https://github.com/fmtlib/fmt.git"
flags="-DFMT_DOC=OFF -DFMT_TEST=OFF -DFMT_FUZZ=OFF" # (optional) for cmake build fast flags
```

### Easy Way to use cppkg init command
```console
# fmt 6.2.0
$ cppm cppkg init -u "https://github.com/fmtlib/fmt/releases/download/6.2.0/fmt-6.2.0.zip" -v "6.2.0" -m "fmt::fmt" fmt
# or (auto detect version)
$ cppm cppkg init -U "https://github.com/fmtlib/fmt/releases/download/6.2.0/fmt-6.2.0.zip" fmt
# fmt git
$ cppm cppkg init -U "https://github.com/fmtlib/fmt.git" fmt
# or (auto detect version)
$ cppm cppkg init -g "https://github.com/fmtlib/fmt.git" -m "fmt::fmt" fmt
# fmt 6.2.0 with cmake flags
$ cppm cppkg init -U "https://github.com/fmtlib/fmt/releases/download/6.2.0/fmt-6.2.0.zip" --flags "-DFMT_DOC=OFF -DFMT_TEST=OFF -DFMT_FUZZ=OFF" fmt
```

Hunter package manager version  
```toml
# cppm.toml
[dependencies]
fmt = {version="latest", module="fmt::fmt", repo="hunter"}
```
### Custom Downloader
```toml
#thirdparty/fmt/6.2.0/cppkg.toml
[fmt]
version = "6.2.0"
description = "A modern formatting library"
module = "fmt::fmt"
url="https://github.com/fmtlib/fmt/releases/download/6.2.0/fmt-6.2.0.zip"
custom=true 
#helper=""(incomplete)
```
[ExternalProject_add](https://cmake.org/cmake/help/latest/module/ExternalProject.html)
```cmake
# Cppkg Base Dependency Downloader
# Almost same cmake ExternalProject_Add
# Other Options:
# - Linux Configures:
#    L_CONFIGURE {...}, L_BUILD {...}, L_INSTALL {...}
# - Windows Configures:
#    W_CONFIGURE {...}, W_BUILD {...}, W_INSTALL {...}
# - Install Path Options:
#    LOCAL(default) GLOBAL 
cmake_minimum_required(VERSION 3.6)
project(fmt-6.2.0-install C CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cppm_tool.cmake)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
download_package(fmt 6.2.0  URL https://github.com/fmtlib/fmt/releases/download/6.2.0/fmt-6.2.0.zip CMAKE_ARGS ${CMAKE_ARGS} -DFMT_DOC=OFF -DFMT_TEST=OFF -DFMT_FUZZ=OFF)
```




# Dependency Format
```toml
[${name}] 
    version = "x.x.x|git|latest(hunter only)"
    type = "lib(default)|bin|cmake(incomplete)"
    git = "..." # if git version 
    url = "..." # if x.x.x version
    module = "..." # (require)
    link = "public(default)|private|interface"
    custom = "false(default)|true" # cppm build time cppkg.toml translate ${name}.cmake.in in 
                                   # if you want to custom installer(${name}.cmake.in) set true
    repo = "cppkg(default)|hunter"
    flags="${cmake build flags}"
    helper="Find...cmake" (optional) cmake/Modules/Find${package_name}.cmake 
    components = "..."
```
