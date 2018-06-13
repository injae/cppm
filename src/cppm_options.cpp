#include"cppm_options.h"
#include"cppm.h"
#include"cmake_find_package.h"
#include<range/v3/core.hpp>
#include<range/v3/numeric/accumulate.hpp>
#include<range/v3/algorithm/for_each.hpp>
#include"utils.h"
#include<string>
#include<boost/regex.hpp>

#include<iterator>

CppmOptions::CppmOptions(int argc, char* argv[]) : Option("Cppm Options", argc, argv) {
    desc_.add_options()
        ("help"      , "produce a help message"     )
        ("version,v" , "Display the version number" )
        ;
    visible_option_.add(desc_);
    desc_.add(make_command_desc());
}

void CppmOptions::run() {
    if(vm_.count("command")) { auto cmd = vm_["command"].as<std::string>();
        _user_command(cmd.c_str());
             if(cmd == "build")      _build();
        else if(cmd == "run")        _run();
        else if(cmd == "install")    _install();
        else if(cmd == "thirdparty") _show_thirdparties();
        else if(cmd == "hint")       _get_cmake_lib_hint();
    }
    else if(vm_.count("help")   ) _help(); 
    else if(vm_.count("version")) _version();
}

void CppmOptions::_user_command(std::string_view cmd) {
    auto project = Cppm::instance()->project();
    auto subargs = ranges::accumulate(get_subarg(), std::string{});
    for(auto& command : subcommand_) {
         if(command.first == cmd) {
             system(("cd "+ project.path + " && " + command.second + " " + subargs).c_str());
             continue;
         }
    }
}

void CppmOptions::registe_subcommand(std::pair<std::string, std::string> command) {
    subcommand_.insert(command);
}

void CppmOptions::_help() {
   std::cout << "Usage: regex [options]\n"
             << visible_option_
             << std::endl;
}

void CppmOptions::_version() {
   std::cout << "Version: "<< Cppm::instance()->project().version << std::endl;
}

void CppmOptions::_build() {
    auto project = Cppm::instance()->project();
    std::string cmd = "cd " + project.bin + " && cmake .. && make";
    system(cmd.c_str()); 
}

void CppmOptions::_run() {
    auto project = Cppm::instance()->project();
    auto subargs = ranges::accumulate(get_subarg(), std::string{});
    std::string cmd = "cd " + project.bin + " && ./" + project.name +" " + subargs;
    system(cmd.c_str());
}

void CppmOptions::_show_thirdparties() {
    auto thirdparties = Cppm::instance()->thirdparties();
    for(auto thirdparty : thirdparties) {
        std::cout << "[" + thirdparty.name << "]\n"
                  << "-build-type: " + thirdparty.build_type << "\n"
                  << "-version: "+ thirdparty.version << "\n"
                  ;
    }
}

void CppmOptions::_install() {
    auto thirdparties = Cppm::instance()->thirdparties();
    auto subargs = get_subarg();
    std::vector<cppm::Thirdparty> install_list;
    for(auto thirdparty : thirdparties) {
        for(auto subarg : subargs) {
            if(util::has_str(subarg, "all")) {
                install_list = thirdparties;  break;
            }
            else if(util::has_str(subarg, thirdparty.name.c_str())) {
               install_list.push_back(thirdparty);
            }
        }
    }
    
    for(auto& library : install_list) {
        cppm::install_thirdparty(library);
    }
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

void CppmOptions::_config_base_install() {
     
}