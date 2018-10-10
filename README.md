Cppm 
========
 C++ cmake project helper only linux
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
2. cppm init --> preparing
3. cppm install --> preparing



## cppm.toml
### package  
<pre><code>[package]
 name = "example"     # user package name
 version = "1.0.0"    # user package version
 description = "example" # package description </code></pre>`

### cmake
<pre><code>[cmake]
version = "3.10"  ## cmake minimum version
option = ""       ## cmake options
builder = "ninja" ## cppm use builder name
compiler = "clang++"  ## cppm use compiler name
</code></pre>

### compiler 
<pre><code>[compiler]
clang++ = {option = "-std=c++17" ## compiler options
          ,version = "7.0" ## compiler minimum version
          ,ccache = true  ## ccache use
</code></pre>
### 5. builder
### 6. bin
### 7. lib
### 8. dependencies

