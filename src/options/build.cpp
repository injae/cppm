#include "cppm.h"
#include "options/build.h"
#include "cmake/cmake_option.h"
#include "cmake/generator.h"

#include <nieel/system.hpp>
#include <nieel/algorithm.hpp>
#include <nieel/filesystem.h>

namespace option 
{
    Build::Build(int argc, char* argv[]) : Option("Build option", argc, argv) {
        desc_.add_options()
            ("help,h", "produce a help message")
            ("update,u", "update cppm cmake module file")
            ;
        visible_option_.add(desc_);
        desc_.add(nieel::make_command_desc());
    }
      
    void Build::run() {
        using namespace nieel::option; 
        using namespace nieel;
        sub_options_(type::option , "help"   , opbind(_help))
                    (type::option , "update" , opbind(_update))
                    (type::default_option    , opbind(_default))
                    .run();
    }
     
    void Build::_help() {
       std::cout << "Usage: regex [options]\n"
                 << visible_option_
                 << sub_options_.command_description("Build")
                 << std::endl;
    }
    
    void Build::_default() {
        using namespace cmake::option;
        Cppm::instance()->parse_project_config();
        auto subargs = get_args();
        auto project = Cppm::instance()->project();
        
        std::ofstream file (project.path.root + "/CMakeLists.txt"); file.is_open();
        file << cmake::make_default_project(project); file.close();
        
        std::string cmd = "cd " + project.path.bin 
                        + " && cmake" + builder(project) + compiler(project) + luncher(project) + " .. "
                        + " && " + build(project);
        for(auto subarg : subargs) { cmd += subarg; }
        
        system(cmd.c_str()); 
    }
    
    void Build::_update() {
        std::cout << "update cppm cmake modules" << std::endl;
        auto project = Cppm::instance()->project();
        std::string cppm_cmake_path = nieel::os::prefix() + std::string("share/cppm/cmake/");
        nieel::copy(cppm_cmake_path + "cmake_option.cmake" , project.path.cmake_module + "/cmake_option.cmake");
        nieel::copy(cppm_cmake_path + "project_maker.cmake", project.path.cmake_module + "/project_maker.cmake");
        nieel::copy(cppm_cmake_path + "library_maker.cmake", project.path.cmake_module + "/library_maker.cmake");
        _default();
    }
    
}