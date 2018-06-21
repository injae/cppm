#include"options/init.h"
#include"cppm.h"
#include"cmake/generator.h"
#include"boost/filesystem.hpp"
#include<fstream>
#include<nieel/filesystem.h>
#include"stdlib.h"

namespace fs = boost::filesystem;

namespace option 
{
    Init::Init(int argc, char* argv[]) : Option("Init option", argc, argv) {
        desc_.add_options()
        ("help,h", "produce a help message")
        ;
        visible_option_.add(desc_);
        desc_.add(nieel::make_command_desc());
    }
      
    void Init::run() {
        if(vm_.count("help")) _help();
        else if(vm_.count("command")) { auto cmd = vm_["command"].as<std::string>(); 
                 if(cmd == "binary")      _bin();
            else if(cmd == "static")      _lib("static");
            else if(cmd == "shared")      _lib("shared");
            else if(cmd == "header-only") _lib("header_only");
        }
    }
     
    void Init::_help() {
        
    }
     
    void Init::_bin() {
        cppm::Project project;
        project.package.name = get_subarg()[0];
        project.path.root = (fs::current_path() / project.package.name).string();
        project.package.type = "bin";
        project.package.version = "0.0.1";
        project.builder.type = "make";
        cmake::make_default_project(project);
        nieel::recursive_copy("/usr/local/share/cppm/default_project", project.path.root);
        std::ofstream file (project.package.name + "/CMakeLists.txt"); file.is_open();
        file << cmake::make_default_project(project); file.close();
        Cppm::instance()->make_config_file(project);
        
        std::cout << "initialized c++ binary project" << "\n";
    }
     
    void Init::_lib(std::string type) {
        cppm::Project project;
        project.package.name = get_subarg()[0];
        project.path.root = (fs::current_path() / project.package.name).string();
        project.package.type = type; 
        project.package.version = "0.0.1";
        project.builder.type = "make";
        cmake::make_default_project(project);
        nieel::recursive_copy("/usr/local/share/cppm/default_project", project.path.root);
        std::ofstream file (project.package.name + "/CMakeLists.txt"); file.is_open();
        file << cmake::make_default_project(project); file.close();
        Cppm::instance()->make_config_file(project);
        
        std::cout << "initialized " + type + " c++ library project" << "\n";
    }
}