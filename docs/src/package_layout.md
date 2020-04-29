## Cppm Base Project Layout
~~~console
.
|-- cppm.toml # cppm config file
|-- CMakeLists.txt # <-- generated from cppm.toml 
|-- build  # Build directory
|   |-- Debug   # Debug mode target directory 
|   `-- Release # Release mode target directory
|-- cmake    # cmake module path
|   |-- Modules  # cmake Find*.cmake path
|   `-- cppm_tool.cmake # cppm_tool load script (require)
|-- include   # public header (export header)
|-- src       # private header and sources
|   `-- main.cpp
|-- test      # test src directory 
|   `-- test01.cpp 
|-- examples  # example src directory
|   `-- example01.cpp 
`-- thirdparty # cppkg config and file path
    `-- ${cppkg_name}/${cppkg_version}/cppkg.toml  # cppkg config file
~~~
