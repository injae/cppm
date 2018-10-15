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
        ("lib,l", po::value<std::string>(), "initialize c++ library project")
        ("bin,b", po::value<std::string>(), "initialize c++ binary project")
        ;
        visible_option_.add(desc_);
        desc_.add(nieel::make_command_desc());
    }
      
    void Init::run() {
        using namespace nieel::option; 
        using namespace nieel;

        sub_options_(type::option, "help"   , opbind(_help))
                    (type::option, "bin"    , opbind(_bin))
                    (type::option, "lib"    , opbind(_lib))
                    (type::default_command, [](){std::cerr << "wrong argument" << std::endl;})
                    (type::default_option , [](){std::cerr << "wrong argument" << std::endl;})
                    .run();
    }
     
    void Init::_help() {
       std::cout << "Usage: regex [options]\n"
                 << visible_option_
                 << sub_options_.command_description("Init")
                 << std::endl;
    }
     
    void Init::_bin() {
        cppm::Project project;
        project.package.name = vm_["bin"].as<std::string>();
        project.path.root = (fs::current_path() / project.package.name).string();
        project.package.type = "binary";
        project.package.version = "0.0.1";
        project.compiler.type = "g++";
        project.builder.type = "make";
        nieel::recursive_copy("/usr/local/share/cppm/default_project", project.path.root);
        Cppm::instance()->make_config_file(project);
        
        std::cout << "initialized c++ binary project" << "\n";
    }
     
    void Init::_lib() {
        cppm::Project project;
        project.package.name = vm_["lib"].as<std::string>();
        project.path.root = (fs::current_path() / project.package.name).string();
        project.package.type = "library";
        project.package.version = "0.0.1";
        project.compiler.type = "g++";
        project.builder.type = "make";
        cmake::make_default_project(project);
        nieel::recursive_copy("/usr/local/share/cppm/default_project", project.path.root);
        Cppm::instance()->make_config_file(project);
        
        std::cout << "initialized c++ library project" << "\n";
    }
}
