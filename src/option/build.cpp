#include <iostream>
#include <thread>
#include <memory>
#include <fmt/format.h>
#include <string>

#include "option/build.h"
#include "config/cppm_tool.h"
#include "util/algorithm.hpp"
#include "util/command.h"
#include "util/filesystem.h"
#include "package/package.h"
#include "util/system.hpp"

using namespace fmt::literals;
using namespace std::literals;
namespace cppm::option
{
    std::string CommandBuilder::build(Config& config)
    {
        using namespace util;
        auto has_toolchains = [&config](){
           return config.cppm_config.package.toolchains() == "" ?
           ""s : " -DCMAKE_TOOLCHAIN_FILE=\"{0}\""_format(config.cppm_config.package.toolchains());
        };
        build_option += (compiler::what() != "msvc"s) ? "-j{}"_format(std::thread::hardware_concurrency()) : "";
        auto sudo = !is_install_local && is_install && strcmp(compiler::what(), "msvc") != 0 ? "sudo" : "";
        cmake_option += is_install_local ? "-DCMAKE_INSTALL_PREFIX={}local"_format(tool::cppm_root()) : "";
        target = is_install ? "install" : "";
        auto target_cmd = target != "" ? "--target " + target : "";

        return "  cd {0} "_format(config.path.build)
            +  "&& cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1"
            +  has_toolchains()
            +  " {0} {1}"_format(config.cmake.option, cmake_option) + " .. "
            +  "&&  cd {0} "_format(config.path.build)
            +  "&& {0} cmake --build . {1} -- {2} "_format(sudo, target_cmd, build_option);
    }
    
    Build::Build() {
        app_.add_option("Generator").abbr("G").args("{Generator}")
            .desc("cmake -G option")
            .call_back([&](){ cmd.cmake_option += " -G {}"_format(app_.get_arg()); });
        app_.add_option("ninja").abbr("n").desc("ninja use to build this option remove build directory")
            .call_back([&](){ clean = true; cmd.cmake_option += " -G Ninja "; });
        app_.add_option("make").abbr("m")
            .desc("Unix make use to build this option remove build directory")
            .call_back([&](){ clean = true; cmd.cmake_option += " -G \"Unix Makefiles\" "; });
        app_.add_option("gcc").abbr("g")
            .desc("g++ use to compile ")
            .call_back([&](){ cmd.cmake_option += " -DCMAKE_CXX_COMPILER={0}"_format("g++"); });
        app_.add_option("clang").abbr("c")
            .desc("clang++ use to compile ")
            .call_back([&](){ cmd.cmake_option += " -DCMAKE_CXX_COMPILER={0}"_format("clang++"); });
        app_.add_option("release").abbr("r")
            .desc("compile release mode")
            .call_back([&](){ cmd.cmake_option += " -DCMAKE_BUILD_TYPE={0}"_format("RELEASE"); });
        app_.add_option("debug").abbr("d")
            .desc("compile debug mode")
            .call_back([&](){ cmd.cmake_option += " -DCMAKE_BUILD_TYPE={0}"_format("DEBUG"); });
        app_.add_option("ntc")
            .desc("not change CMakeLists.txt test options")
            .call_back([this]() { none_tc = true; });
        app_.add_option("export")
            .desc("export cppkg")
            .call_back([&]() { config_load(); export_cppkg(); });
        app_.add_option("local")
            .desc("install local")
            .call_back([&]() { cmd.is_install_local = true; clean = true; });
        app_.add_option("global")
            .desc("install global")
            .call_back([&]() { cmd.is_install_local = false; clean = true; });
        app_.add_command("install")
            .desc("cmake target install ")
            .call_back([this]() { cmd.is_install = true; });
        app_.add_command().args("{cppm options} {builder options}")
            .desc("Build command")
            .call_back([&](){
                config_load();
                fs::create_directories(config_.path.build);
                if(!none_tc) {
                    util::write_file("{0}/CMakeLists.txt"_format(config_.path.root), config_.generate());
                    auto cppm_root = tool::cppm_root(); 
                    util::over_write_copy_file("{0}cmake/cppm_tool.cmake"_format(cppm_root)
                                              ,"{0}/cppm_tool.cmake"_format(config_.path.cmake));
                    util::over_write_copy_file("{0}cmake/HunterGate.cmake"_format(cppm_root)
                                              ,"{0}/HunterGate.cmake"_format(config_.path.cmake));
                }
                if(clean) {
                    fs::remove_all(config_.path.build);
                    fs::create_directory(config_.path.build);
                }
                if(!app_.args().empty()) {
                    cmd.build_option += util::accumulate(app_.args(), " ");
                    app_.args().clear();
                }
                auto cmd1 = cmd.build(config_);
                auto cmd2 = cmd.after_option;
                //fmt::print(cmd1);
                //fmt::print(cmd2);
                system(cmd1.c_str());
                if(cmd2 != "") system(cmd2.c_str());
                //util::system::exec(cmd.build(config_).c_str(), [](auto& str){ fmt::print(str); });
                //util::system::exec(cmd.after_option.c_str(),   [](auto& str){ fmt::print(str); });
            });
    }

    void Build::export_cppkg() {
        package::Package pkg;
        pkg.name = config_.package.name;
        for(auto& lib : config_.libs.list) {
            pkg.cmake.name += lib.install ? "{0}::{1} "_format(config_.package.name,lib.name) : "";
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
