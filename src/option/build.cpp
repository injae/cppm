#include "option/build.h"
#include "util/algorithm.hpp"
#include <memory>
#include <fmt/format.h>
#include "util/command.h"
#include "util/filesystem.h"

namespace cppm::option
{
    std::string CommandBuilder::build(Config& config)
    {
        using namespace fmt::literals;
        auto builder  = config.builder.list[config.cmake.builder];
        auto compiler = config.compiler.list[config.cmake.compiler];
        
        auto is_ninja = builder.name == "ninja" ? " -G Ninja" : " ";
        auto comp_opt = compiler.name != "none" ?
                        " -DCMAKE_CXX_COMPILER={0}"_format(compiler.name) : " ";

        return "  cd {0} "_format(config.path.build)
            +  "&& cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
            +            comp_opt
            +            is_ninja
            +  " -DCMAKE_TOOLCHAIN_FILE=\"{0}\""_format(config.cppm_config.package.toolchains())
            +  " {0} {1}"_format(config.cmake.option, cmake_option) + " .. "
            +  "&& sudo {0} {1} {2}"_format(config.cmake.builder, builder.option, build_option);
    }
    
    Build::Build(Config& config) {
        using namespace fmt::literals;
        auto cmd_ = std::make_shared<CommandBuilder>();
        app_.add_option("help")
            .abbr("h")
            .desc("show cppm commands and options")
            .call_back([&](){ app_.show_help(); });
        app_.add_option("release")
            .abbr("r")
            .desc("compile release mode")
            .call_back([&, cmd = cmd_](){
                cmd->cmake_option += " -DCMAKE_BUILD_TYPE={0}"_format("RELEASE");
                app_.call_default();
            });
        app_.add_option("debug")
            .abbr("d")
            .desc("compile debug mode")
            .call_back([&, cmd = cmd_](){
                cmd->cmake_option += " -DCMAKE_BUILD_TYPE={0}"_format("DEBUG");
                app_.call_default();
            });
        app_.add_command()
            .desc("before make or ninja command")
            .call_back([&, cmd = cmd_](){
                cmakelist_build(config);
                fs::copy_file((std::string(std::getenv("HOME")))+"/.cppm/cmake/project_maker.cmake"
                                     ,config.path.cmake +"/project_maker.cmake"
                                     ,fs::copy_option::overwrite_if_exists);
                if(!app_.args().empty()) {
                    cmd->build_option += util::accumulate(app_.args(), " ");
                    app_.args().clear();
                }
                //fmt::print(cmd->build(config).c_str());
                system(cmd->build(config).c_str());
            });
    }

    void Build::cmakelist_build(Config& config)
    {
        std::ofstream CmakeLists(config.path.root + "/CMakeLists.txt"); CmakeLists.is_open();
        CmakeLists << config.generate();
        CmakeLists.close();
    }
}
