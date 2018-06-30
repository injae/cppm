#include"cppm.h"
#include"cppm_options.h"
#include"cmake/find_package.h"
#include"cmake/cmake_option.h"
#include"cmake/generator.h"
#include"options/init.h"
#include"options/install.h"
#include"url.h"
#include<range/v3/core.hpp>
#include<range/v3/algorithm.hpp>
#include<range/v3/numeric.hpp>
#include<nieel/string.hpp>

#include<string>
#include<iterator>

CppmOptions::CppmOptions(int argc, char* argv[]) : Option("Cppm Options", argc, argv) {
    desc_.add_options()
        ("help,h"      , "produce a help message"     )
        ("version,v" , "Display the version number" )
        ;
    visible_option_.add(desc_);
    desc_.add(nieel::make_command_desc());
}

void CppmOptions::run() {
    using namespace nieel::option; 
    using namespace nieel;
    SubOptions suboptions(vm_);
    suboptions(type::option , "help"      , opbind(_help))
              (type::command, "init"      , opbind(_init))
              (type::command, "run"       , opbind(_run))
              (type::command, "test"      , opbind(_test))
              (type::command, "build"     , opbind(_build))
              (type::command, "hint"      , opbind(_get_cmake_lib_hint))
              (type::option , "version"   , opbind(_version))
              (type::command, "install"   , opbind(_install))
              (type::command, "library"   , opbind(_show_libraries))
              (type::command, "thirdparty", opbind(_show_thirdparties))
              (type::default_command, uopbind(_user_command))
              .run();
}

void CppmOptions::_test() {
    Cppm::instance()->parse_project_config();
    std::cout << cmake::make_default_project(Cppm::instance()->project()) << std::endl;
    
}

void CppmOptions::_user_command(std::string cmd) {
    Cppm::instance()->parse_project_config();
    auto project = Cppm::instance()->project();
    auto subargs = ranges::accumulate(get_subarg(), std::string{});
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
             << std::endl;
}
void CppmOptions::_show_libraries() {
    Cppm::instance()->parse_project_config();
    auto project = Cppm::instance()->project();
        std::for_each(project.libraries.begin(),
                     project.libraries.end(),
                     [](auto library) {
                        cppm::Library::show(library);
                     });
}

void CppmOptions::_version() {
   Cppm::instance()->parse_project_config();
   std::cout << "Version: "<< Cppm::instance()->project().package.version << std::endl;
}

void CppmOptions::_build() {
    using namespace cmake::option;
    Cppm::instance()->parse_project_config();
    auto subargs = get_subarg();
    std::ofstream file (Cppm::instance()->project().path.root + "/CMakeLists.txt"); file.is_open();
    file << cmake::make_default_project(Cppm::instance()->project()); file.close();
    
    auto project = Cppm::instance()->project();
    std::string cmd = "cd " + project.path.bin 
                    + " && cmake" + builder(project) + compiler(project) + " .. "
                    + " && " + build(project);
                    
    for(auto subarg : subargs) { cmd += subarg; }
    
    system(cmd.c_str()); 
}

void CppmOptions::_run() {
    Cppm::instance()->parse_project_config();
    auto project = Cppm::instance()->project();
    auto subargs = ranges::accumulate(get_subarg(), std::string{});
    std::string cmd = "cd " + project.path.bin + " && ./" + project.package.name +" " + subargs;
    system(cmd.c_str());
}

void CppmOptions::_show_thirdparties() {
    using namespace ranges;
    Cppm::instance()->parse_project_config();
    auto thirdparties = Cppm::instance()->project().thirdparties;
     
    for(auto& thirdparty : thirdparties) {
        std::cout << "[" + thirdparty.name << "]\n"
                  << "-build-type: " + thirdparty.build_type << "\n"
                  << "-version: "+ thirdparty.version << "\n"
                  ;
    }
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