#include "option/build.h"
#include "util/algorithm.hpp"
#include <fmt/format.h>

namespace cppm::option
{
    std::string CommandBuilder::build()
    {
        using namespace fmt::literals;
        auto builder  = config.builder.list[config.cmake.builder];
        auto compiler = config.compiler.list[config.cmake.compiler];
        
        auto is_ninja = builder.name == "ninja" ? " -G Ninja" : " ";
        auto luncher  = compiler.ccache ? 
                        " -DCMAKE_CXX_COMPILER_LAUNCHER={0}"_format("ccache") : " " ;
        auto comp_opt = compiler.name != "" ?
                        " -DCMAKE_CXX_COMPILER={0}"_format(compiler.name) : " ";

        return "  cd {0} "_format(config.path.build)
            +  "&& cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1"
            +            comp_opt
            +            luncher
            +            is_ninja
            +  " {0} "_format(cmake_option) + " .. "
            +  "&& {0} {1} {2}"_format(builder.name, builder.option, build_option);
    }
    
    Build::Build(Config& config) {
        using namespace fmt::literals;
        app_.add_option("help")
            .abbr("h")
            .desc("show cppm commands and options")
            .call_back([&](){ app_.show_help(); });
        app_.add_option("release")
            .abbr("r")
            .desc("compile release mode")
            .call_back([&](){
                cmakelist_build(config);
                CommandBuilder cmd(config);
                cmd.cmake_option += " -DCMAKE_BUILD_TYPE={0}"_format("RELEASE");
                system(cmd.build().c_str());
            });
        app_.add_command()
            .desc("before make or ninja command")
            .call_back([&](){
                cmakelist_build(config);
                CommandBuilder cmd(config);
                cmd.cmake_option += " -DCMAKE_BUILD_TYPE={0}"_format("DEBUG");
                if(!app_.args().empty()) {
                    cmd.build_option += util::accumulate(app_.args(), " ");
                    app_.args().clear();
                }
                system(cmd.build().c_str());
            });
    }
    
    void Build::cmakelist_build(Config& config)
    {
        std::ofstream CmakeLists(config.path.root + "/CMakeLists.txt"); CmakeLists.is_open();
        CmakeLists << config.generate();
        CmakeLists.close();
    }
}
