# cppkg.toml

```toml
[package]
    name = "..."
    version = "..."
    desciprtion = "..."
    standard = "{11|14|17(default)|20}" # (optional)
    git = "..." # (optional) this option use to cppm build --export
```
```toml
[hunter] # (optional)  hunter package manager setting option
    use = true|false(default) # if you use hunter package this option is default is true
    url = "..." # (optional) custom hunter version url setting option, [ url, sha1 ] is pair
    sha1 = "..." # (optional) custom hunter version sha1 setting option, [ url, sha1 ] is pair
```
```toml
[cmake] # (optional) cmake setting option 
    include = ["xxx.cmake", ...] # (optional) this option is include cmake file
    version = "..." # (optional) minimum cmake version setting option default is 3.12
    option = "..." # (optional) cmake build option use to command 'cppm build'
```
```toml
# (deprecated)
[builder]
    make = {option ="..."}
    ninja = {option ="..."}
```
```toml
[compiler.debug] # (optional) this is only debug mode compiler option
[compiler.release] # (optional) this is only release mode compiler option
[compiler] # (optional) this is debug and release compiler option target compiler is same
    clang = "..." # compiler option
    gcc = "..." # compiler option
    msvc = "..." # compiler option
```
```toml
[[bin]]
    name = "..." # target name (require)
    sources = ["src/xxx.cpp", ...] # (require)
    install = true(default)|false  # disable install setting
    standard = "{11|14|17(default)|20}"  #(working)

```
```toml
[[lib]]
    name = "..." # target name, export cmake package name 
    type = "static(default)|shared|hearder_only" # (require)
    sources = ["src/xxx.cpp", ...] # use source files, type = "header_only" no nessasery this option
    install = true(default)|false  # disable install setting
    standard = "{11|14|17(default)|20}"  #(working)
```
```toml
# sub project setting (BETA)
[workspace]
    member = ["path/", "path"]
```
```toml
# global dependencies setting option, is same [target.default.platform.default.dependencies] (working)
[dependencies] 
    # cppkg package add dependency
    # name   version(require)
      ... = "x.x.x|git|latest(default)" # latest ordering is x.x.x > git 
    # nomal cmake package add dependency
    # name        (require)                   (optional)           (optional) default is public            (optional) package type  
      ... = {module = "..." , version ="...", components="... ...", link = "public|private|interface", type="lib(default)|bin" }
    # hunter package add dependency
    # name        (require)                   (optional)           (optional) default is public            (require) load hunter package
      ... = {module = "..." , version ="...", components="... ...", link = "public|private|interface" , hunter= true }
    # none cmake package add dependency
    # name        (require)            (optional)           (optional) default is public (require) none cmake package
      ... = {module = "...", components="... ...", lnk_type = "public|private|interface", no_module= true }
    # module is name that cmake find_package 
    # matched cmake scirpt
    # find_package(${name} ${version} COMPONENTS ${components})
    # target_link_libraries(${target} ${lnk_type} ${modle})
    #link.public    => this library use header and source
    #link.private   => this library use source only
    #link.interface => this library use header only 

```
```toml
# (working) platform classification depdencies setting 
[platform.windows|macos|linux|unix.dependencies] 

```
```toml
# (working)
[exclude]

```
```toml
# (working)
[ci.github_action]
[ci.appveyer]
[ci.azure]
[ci.travis]
```

```toml
# (working)
[target.{name}.compiler]
[target.{name}.compiler.debug]


[target.{name}.compiler.release]
```
```toml
# (working) target classification depedencies add
[target.{name}.dependencies]
# (working)
[target.{name}.platform.{type}.dependencies]
# (working)
[target.{name}.platform.{type}.sources]
```
```toml
# (working)
[[test]]
```
```toml
# (working)
[[example]]

```
```toml
# (working)
[dev-dependencies]

```
```toml
# (working)    
dependencies = {
    ${name} = ${dependency options},
    ${name} = ${dependency options},
}
```
