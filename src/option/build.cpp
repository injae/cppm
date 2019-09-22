#include <iostream>
#include <thread>
#include <memory>
#include <fmt/format.h>

#include "option/build.h"
#include "util/algorithm.hpp"
#include "util/command.h"
#include "util/filesystem.h"
#include "package/package.h"
#include "util/system.hpp"

using namespace fmt::literals;
namespace cppm::option
{
    std::string CommandBuilder::build(Config& config)
    {
        using namespace util;
        auto has_toolchains = [&config]() -> std::string {
           return config.cppm_config.package.toolchains() == "" ?
           "" : " -DCMAKE_TOOLCHAIN_FILE=\"{0}\""_format(config.cppm_config.package.toolchains());
        };

        auto sudo = is_install && !strcmp(compiler::what(), "msvc")? "sudo" : "";
        if(is_install) target = "install";
        auto target_cmd = target != "" ? "--target " + target : "";

        return "  cd {0} "_format(config.path.build)
            +  "&& cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1"
            +  has_toolchains()
            +  " {0} {1}"_format(config.cmake.option, cmake_option) + " .. 2>&1"
            +  "&& {0} cmake --build . {1} -- {2} 2>&1"_format(sudo, target_cmd, build_option);
    }
    
    Build::Build() {
        app_.add_option("Generator")
            .abbr("G")
            .desc("cmake -G option")
            .args("{Generator}")
            .call_back([&](){ cmd.cmake_option += " -G {}"_format(app_.get_arg()); });
        app_.add_option("ninja")
            .abbr("n")
            .desc("ninja use to build this option remove build directory")
            .call_back([&](){
                clean = true;
                cmd.cmake_option += " -G Ninja ";
            });
        app_.add_option("make")
            .abbr("m")
            .desc("Unix make use to build this option remove build directory")
            .call_back([&](){
                clean = true;
                cmd.cmake_option += " -G \"Unix Makefiles\" ";
            });
        app_.add_option("gcc")
            .abbr("g")
            .desc("g++ use to compile ")
            .call_back([&](){ cmd.cmake_option += " -DCMAKE_CXX_COMPILER={0}"_format("g++"); });
        app_.add_option("clang")
            .abbr("c")
            .desc("clang++ use to compile ")
            .call_back([&](){ cmd.cmake_option += " -DCMAKE_CXX_COMPILER={0}"_format("clang++"); });
        app_.add_option("release")
            .abbr("r")
            .desc("compile release mode")
            .call_back([&](){ cmd.cmake_option += " -DCMAKE_BUILD_TYPE={0}"_format("RELEASE"); });
        app_.add_option("debug")
            .abbr("d")
            .desc("compile debug mode")
            .call_back([&](){ cmd.cmake_option += " -DCMAKE_BUILD_TYPE={0}"_format("DEBUG"); });
        app_.add_option("ntc")
            .desc("not change CMakeLists.txt test options")
            .call_back([this]() { none_tc = true; });
        app_.add_option("export")
            .desc("export cppkg")
            .call_back([&]() { config_load(); export_cppkg(); });
        app_.add_command("install")
            .desc("cmake target install ")
            .call_back([this]() { cmd.is_install = true; });
        app_.add_command()
            .desc("Build command")
            .args("{cppm options} {builder options}")
            .call_back([&](){
                config_load();
                fs::create_directories(config_.path.build);
                if(util::compiler::what() != "msvc"_format()) {
                    cmd.build_option += "-j{}"_format(std::thread::hardware_concurrency());
                }
                if(!none_tc) {
                    cmakelist_build();
                    fs::copy_file((std::string(std::getenv("HOME")))+"/.cppm/cmake/cppm_tool.cmake"
                                  ,config_.path.cmake +"/cppm_tool.cmake"
                                  ,fs::copy_option::overwrite_if_exists);
                    fs::copy_file((std::string(std::getenv("HOME")))+"/.cppm/cmake/HunterGate.cmake"
                                  ,config_.path.cmake +"/HunterGate.cmake"
                                  ,fs::copy_option::overwrite_if_exists);
                }
                if(clean) {
                    fs::remove_all(config_.path.build);
                    fs::create_directory(config_.path.build);
                }
                if(!app_.args().empty()) {
                    cmd.build_option += util::accumulate(app_.args(), " ");
                    app_.args().clear();
                }
                //system(cmd.build(config_).c_str());
                //system(cmd.after_option.c_str());
                util::system::exec(cmd.build(config_).c_str(), [](auto& str){ fmt::print(str); });
                util::system::exec(cmd.after_option.c_str(), [](auto& str){ fmt::print(str); });
            });
    }

    void Build::cmakelist_build()
    {
        std::ofstream CmakeLists(config_.path.root + "/CMakeLists.txt"); CmakeLists.is_open();
        CmakeLists << config_.generate();
        CmakeLists.close();
    }

    void Build::export_cppkg() {
        package::Package pkg;
        pkg.name = config_.package.name;
        for(auto& lib : config_.libs.list) {
            if(lib.install){
                pkg.cmake.name += "{0}::{1} "_format(config_.package.name,lib.name);
            }
        }
        pkg.description = config_.package.description;
        pkg.download.git.url = config_.package.git_repo;
        if(pkg.download.git.url == "") { fmt::print(stderr, "need git_repo"); exit(1); }
        pkg.deps = config_.dependencies;
        pkg.global = false;
        package::cppkg::init(pkg);
        package::cppkg::build(pkg.name);
        package::cppkg::regist("{}/{}"_format(pkg.name,pkg.version));
    }
}
