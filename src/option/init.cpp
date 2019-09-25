#include "option/init.h"
#include "util/filesystem.h"
#include "option/cppkg.h"
#include "config/path.h"
#include "config/cppm_tool.h"
#include "util/string.hpp"
#include <iostream>
#include <cstdlib>
#include <fmt/format.h>

using namespace fmt::literals;
using namespace cppm::util::str;

namespace cppm::option
{
    Init::Init() {
        app_.add_option("help")
            .abbr("h")
            .desc("show cppm command and options")
            .call_back([&](){ app_.show_help(); });
        app_.add_option("bin")
            .abbr("b")
            .desc("initialize new c++ binary project")
            .args("{name}")
            .call_back([&](){ this->make_bin(); });
        app_.add_option("lib")
            .abbr("l")
            .desc("initialize new c++ library project")
            .args("{name}")
            .call_back([&](){ this->make_lib(); });
    }

    void Init::make_bin() {
        auto name = app_.args().front();
        auto gen = make_project();

        gen += "[[bin]]\n"
            +  "   name = {}\n"_format(quot(name))
            +  "   source = [{}]\n"_format("src/.*"_quot)
            ;

        auto project = Path::make((fs::current_path()/name).string());
        std::fstream file; 
        file.open(project.root + "/cppm.toml", std::ios::out);
        file << gen;
        file.close();

        file.open(project.source + "/main.cpp", std::ios::out);
        file << "\nint main(int argc, char* argv[]) {\n\n     return 0; \n}";
        file.close();
        app_.args().pop_front();
    } 

    void Init::make_lib() {
        auto name = app_.args().front();
        auto gen = make_project();

        gen += "[[lib]]\n"
            +  "   name = {}\n"_format(quot(name))
            +  "   type = {}\n"_format("shared"_quot)
            +  "   source = [{}]\n"_format("src/.*"_quot)
            ;

        auto project = Path::make((fs::current_path()/name).string());
        std::fstream file; 
        file.open(project.root + "/cppm.toml", std::ios::out);
        file << gen;
        file.close();

        fs::create_directory(project.include + "/" + name);
        app_.args().pop_front();
    }

    std::string Init::make_project() {
        if(app_.args().size() <= 0)  { std::cerr << "need project name"  << std::endl; exit(1);}
        if(app_.args().size() >  1)  { std::cerr << "too many argument"  << std::endl; exit(1);}
        auto project_name = app_.args().front();

        if(fs::exists(project_name)) { std::cerr << "this name is exist" << std::endl; exit(1);}
        fs::create_directory(project_name);
        auto project = Path::make((fs::current_path()/project_name).string());
        fs::create_directory(project.build);
        fs::create_directory(project.cmake);
        fs::create_directory(project.include);
        fs::create_directory(project.source);
        fs::create_directory(project.thirdparty);
        fs::create_directory(project.cmake);
        fs::create_directory(project.cmake + "/Modules");

        auto cppm_path = tool::cppm_root();
        fs::copy(cppm_path + "cmake/cppm_tool.cmake", project.cmake+"/cppm_tool.cmake");

        return "[package]\n"
             + "   name = {}\n"_format(quot(project_name))
             + "   version = {}\n"_format("0.0.1"_quot)
             + "   description = {}\n"_format(""_quot)
             + "\n"
             ;
    }
}
