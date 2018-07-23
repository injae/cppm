#include"cppm.h"
#include"cppm_options.h"
#include"cmake/find_package.h"
#include"cmake/cmake_option.h"
#include"cmake/generator.h"
#include"options/init.h"
#include"options/install.h"
#include"package/package.h"
#include"url.h"
#include<nieel/string.hpp>
#include<nieel/algorithm.hpp>

#include<string>
#include<iterator>

CppmOptions::CppmOptions(int argc, char* argv[]) : Option("Cppm Options", argc, argv) {
    desc_.add_options()
        ("help"      , "produce a help message"     )
        ("version,v" , "Display the version number" )
        ;
    visible_option_.add(desc_);
    desc_.add(nieel::make_command_desc());
}

void CppmOptions::run() {
    using namespace nieel::option; 
    using namespace nieel;
    sub_options_(type::option , "help"      , opbind(_help))
                (type::option , "version"   , opbind(_version))
                (type::command, "init"      , opbind(_init), "initialize new cppm project")
                (type::command, "run"       , opbind(_run),  "running binary")
                (type::command, "test"      , opbind(_test))
                (type::command, "build"     , opbind(_build), "project build")
                (type::command, "hint"      , opbind(_get_cmake_lib_hint), "get cmake-lib-name option")
                (type::command, "install"   , opbind(_install), "install library")
                (type::default_command      , uopbind(_user_command))
                .run();
}

void CppmOptions::_test() {
    using namespace cppm;
    Cppm::instance()->parse_project_config();
    for(auto arg : get_subarg()) {
    }
}

void CppmOptions::_user_command(std::string cmd) {
    Cppm::instance()->parse_project_config();
    auto project = Cppm::instance()->project();
    auto subargs = nieel::str::accumulate(get_subarg(), std::string{});
    for(auto& command : project.user_commands) {
         if(command.name == cmd) {
             system(("cd "+ project.path.root + " && " + command.script + " " + subargs).c_str());
             return ; 
         }
    }
    std::cerr << "unknown command" << std::endl; exit(1);
}

void CppmOptions::registe_subcommand(std::pair<std::string, std::string> command) {
    subcommand_.insert(command);
}

void CppmOptions::_help() {
   std::cout << "Usage: regex [options]\n"
             << visible_option_
             << sub_options_.command_description("Command")
             << std::endl;
}

void CppmOptions::_version() {
   Cppm::instance()->parse_project_config();
   std::cout << "Version: "<< Cppm::instance()->project().package.version << std::endl;
}

void CppmOptions::_build() {
    using namespace cmake::option;
    Cppm::instance()->parse_project_config();
    auto subargs = get_subarg();
    auto project = Cppm::instance()->project();
    std::ofstream file (project.path.root + "/CMakeLists.txt"); file.is_open();
    file << cmake::make_default_project(project); file.close();
    
    std::string cmd = "cd " + project.path.bin 
                    + " && cmake" + builder(project) + compiler(project) + luncher(project) + " .. "
                    + " && " + build(project);
    for(auto subarg : subargs) { cmd += subarg; }
    
    system(cmd.c_str()); 
}

void CppmOptions::_run() {
    Cppm::instance()->parse_project_config();
    auto project = Cppm::instance()->project();
    auto subargs = nieel::accumulate(get_subarg(), std::string{});
    std::string cmd = "cd " + project.path.bin + " && ./" + project.package.name +" " + subargs;
    system(cmd.c_str());
}

void CppmOptions::_install() {
    Cppm::instance()->parse_project_config();
    make_sub_command<option::Install>().run();
}

void CppmOptions::_get_cmake_lib_hint() {
    cppm::Thirdparty thirdparty;
    thirdparty.name = get_subarg()[0];
   	 
    for(auto lib : cmake::get_find_package_list()) {
        if(!(lib.name == thirdparty.name)) continue;
        cmake::get_package_config_hint(lib);
    }
}

void CppmOptions::_make_cmake_find_lib_file() {
     
}

void CppmOptions::_init() {
     make_sub_command<option::Init>().run();
}