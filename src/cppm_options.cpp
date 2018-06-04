#include"cppm_options.h"
#include"cppm.h"
#include<range/v3/core.hpp>
#include<range/v3/numeric/accumulate.hpp>
#include<range/v3/algorithm/for_each.hpp>

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
    auto project = Cppm::instance()->project();
    if(vm_.count("command")) { auto cmd = vm_["command"].as<std::string>();
        auto subargs = ranges::accumulate(get_subarg(), std::string{});
        for(auto& command : subcommand_) {
             if(command.first == cmd) {
                 system(("cd "+ project.path + " && " + command.second + " " + subargs).c_str());
                 continue;
             }
        }
             if(cmd == "build") _build();
        else if(cmd == "run")   _run();
    }
    else if(vm_.count("help")   ) _help(); 
    else if(vm_.count("version")) _version();
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