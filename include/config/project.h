#ifndef __CONFIG_PROJECT_H__
#define __CONFIG_PROJECT_H__

#include <string>
#include <vector>
#include "config/package.h"
#include "config/path.h"
#include "config/builder.h"
#include "config/compiler.h"
#include "config/library.h"
#include "config/binary.h"
#include "config/command.h"
#include "config/thirdparty.h"
#include "config/cmake.h"

namespace cppm
{
    class Project
    {
    public:
        static Project parse(YAML::Node& node);
        static Project& parse(YAML::Node& node, Project& project);
    public:
        void parse_user_cmake_script(YAML::Node& node);
        bool has(std::string name);
        std::vector<Thirdparty> has(std::vector<std::string>& names);
        std::vector<std::string> cmake_lib_name(std::vector<std::string> libraries);
        std::string make_cmake_bin_lib_script();
            
    public:
          Package  package 
		; Path     path
        ; Builder  builder
        ; Cmake    cmake
        ; Compiler compiler 
        ; std::vector<Binary>      binaries
        ; std::vector<Library>     libraries
        ; std::vector<Command>     user_commands
		; std::vector<Thirdparty>  thirdparties
        ; std::vector<std::string> user_cmake_scripts
        ; std::vector<std::string> include_script
        ;
    };
}

#endif
