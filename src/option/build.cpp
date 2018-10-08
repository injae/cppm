#include "option/build.h"
#include <fmt/format.h>

namespace cppm::option
{
    std::string CommandBuilder::build()
    {
        using namespace fmt::literals;
        auto builder  = config.builder.list.begin()->second;
        auto compiler = config.compiler.list.begin()->second;
        
        auto is_ninja = builder.name == "ninja" ? " -G Ninja" : " ";
        auto luncher  = compiler.ccache ? 
                        " -DCMAKE_CXX_COMPILER_LAUNCHER={0}"_format("ccache") : " " ;

        return "  cd {0} "_format(config.path.build)
            +  "&& cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1"
            +  "         -DCMAKE_CXX_COMPILER={0}"_format(compiler.name)
            +            luncher
            +            is_ninja
            +  " {0} "_format(cmake_option) + " .. "
            +  "&& {0} {1} ."_format(builder.name, builder.option);

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
                cmd.cmake_option += " -CMAKE_BUILD_TYPE={0}"_format("RELEASE");
                //system(cmd.build().c_str());
            });
        app_.add_option("debug")
            .abbr("d")
            .desc("compile debug mode")
            .call_back([&](){
                cmakelist_build(config);
                CommandBuilder cmd(config);
                cmd.cmake_option += " -CMAKE_BUILD_TYPE={0}"_format("DEBUG");
                //system(cmd.build().c_str());
            });
    }
    
    void Build::cmakelist_build(Config& config)
    {
        std::ofstream CmakeLists(config.path.root + "/CMakeLists_test.txt"); CmakeLists.is_open();
        CmakeLists << config.generate();
        CmakeLists.close();
    }
}
