## cppkg.toml
this setting is same [dependency.${dep name}]
```toml
[${name}] 
    version = "x.x.x|git|latest(hunter only)"
    type = "lib(default)|bin|cmake(incomplete)"
    git = "..." # if git version 
    url = "..." # if x.x.x version
    module = "..." # (require)
    link = "public(default)|private|interface"
    custom = "false(default)|true" # this config table export ${name}.cmake.in 
                                   # if you wan't custom ${name}.cmake.in this option set true
    repo = "cppkg(default)|hunter"
    components = "..."
```
