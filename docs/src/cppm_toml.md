# cppkg.toml
```toml
[package]
    name = "..." # (require)
    version = "..." # (require)
    desciprtion = "..." # (require)
    standard = "{11|14|17(default)|20}"
    git = "..." # this option use to cppm build --export
    vcpkg= "false(default)|true" # cppm auto detect vcpkg option
```
```toml
[hunter] # (optional)  hunter package manager setting option
    use = true|false(default) # if you use hunter package this option is default is true
    url = "..." # custom hunter version url setting option, [ url, sha1 ] is pair
    sha1 = "..." # custom hunter version sha1 setting option, [ url, sha1 ] is pair
```
```toml
[cmake] # (optional) cmake setting option 
    include = ["xxx.cmake", ...] # this option is include cmake file
    version = "..." # minimum cmake version setting option default is 3.12
    option = "..." # cmake build option use to command 'cppm build'
    toolchains = "..." # (incomplete) cmake toolchains option
```
Profiles
Prfiles provide a way alter the compiler settings, 
Profile settings can be overridden for specific packages
```toml
[profile.dev] # this is only debug or test or example mode compiler option
[profile.release] # this is only release mode compiler option
[profile.test] # (incomplete) this is only test mode compiler option
[profile.release.compiler] # this is only release mode compiler option
    clang = "..." # compiler option
    gcc = "..." # compiler option
    msvc = "..." # compiler option
[profile.release.package.{package.name}] # (incomplete) override for specific packages
[profile.release.package.{package_name}.compiler] # (incomplete) this is only release mode compiler option
```
```toml
[[bin]]
    name = "..." # target name (require)
    sources = ["src/ss/.*", src/xx.cpp, ...] # (require), source files, you can use regex
    install = true(default)|false  # disable install setting
```
```toml
[lib]
    name = "..." # target name, export cmake package name 
    type = "static(default)|shared|hearder-only" 
    namespace = "... " (default: [package.name]) # cmake export namespace  
    sources = ["src/xxx.cpp", ...] # type = "header_only" this option not working
    install = true(default)|false  # disable install setting
```
```toml
[[test]]
    name = "..." # target name, export cmake package name 
    type = "binary(default)|static|shared|hearder-only" 
    sources = ["src/xxx.cpp", ...] # use source files, type = "header_only" no nessasery this optio
    install = true|false(default)  # disable install setting
```
```toml
[[example]]
    name = "..." # target name, export cmake package name 
    type = "binary(default)|static|shared|hearder-only" 
    sources = ["src/xxx.cpp", ...] # use source files, type = "header_only" no nessasery this option
    install = true|false(default)  # disable install setting
```
```toml
# sub project setting
[workspace]
    member = ["path/", "path"]
```
```toml
[dev-dependency] # this dependency only work debug mode
[dependencies] 
    # cppkg package add dependency
    # name   version(require)
      ... = "x.x.x|git|latest" # this config find cppkg.toml in ${package root}/thirdparty/${name}/${version}/cppkg.toml
    # inline dependency setting
    # name        (require)                   (optional)           (optional) default is public            (optional) package type  
      ... = {module = "..." , version ="...", components="... ...", link = "public|private|interface", type="lib(default)|bin|cmake(incomplete)", repo="cppkg(default)|hunter" }
    # hunter package add dependency
    # name        (require)                   (optional)           (optional) default is public            (require) load hunter package
      ... = {module = "..." , version ="latest", components="... ...", link = "public|private|interface" ,    repo="hunter" }
    # none cmake package add dependency
    # name        (require)            (optional)           (optional) default is public (require) none cmake package
      ... = {module = "...", components="... ...", lnk_type = "public|private|interface", no_module= true }
    # module is name that cmake find_package 
    # matched cmake scirpt
    # find_package(${name} ${version} COMPONENTS ${components})
    # target_link_libraries(${target} ${lnk_type} ${module})
    #link.public    => this library use header and source , dependency foward 
    #link.private   => this library use source only, dependency not forward
    #link.interface => this library use header only, dependency forward
```
```toml
# (incomplete) platform classification depdencies setting 
[target.windows|macos|linux|unix.dependencies] 

```
```toml
# (incomplete)
[exclude]

```
```toml
# (incomplete)
[ci.github_action]
[ci.appveyer]
[ci.azure]
[ci.travis]
```
