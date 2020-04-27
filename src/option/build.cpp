#include <iostream>
#include <thread>
#include <memory>
#include <fmt/format.h>
#include <hashpp/md5.h>
#include <string>

#include "option/build.h"
#include "cppm/util/algorithm.hpp"
#include "cppm/util/filesystem.h"
#include "cppm/cppkg/cppkg.h"
#include "cppm/util/system.hpp"
#include "cppm/util/string.hpp"
#include "cppm/core/cppm_tool.hpp"
#include "cmake/cmake.h"
using namespace fmt::literals;
using namespace std::literals;
using namespace cppm::util::str;
namespace cppm::option
{
    Build::Build()  {
        app_.add_option("Generator").abbr("G").args("{Generator}")
            .desc("cmake -G option")
            .call_back([&](){ cmake_.generator(app_.get_arg());});
        app_.add_option("ninja").abbr("n")
            .desc("ninja use to build this option remove build directory")
            .call_back([&](){ cmake_.generator("Ninja"); clean=true; });
        app_.add_option("make").abbr("m")
            .desc("Unix make use to build this option remove build directory")
            .call_back([&](){ cmake_.generator("Unix Makefiles"_quot); clean = true;  });
        app_.add_option("gcc").abbr("g")
            .desc("g++ use to compile ")
            .call_back([&](){ cmake_.define("CMAKE_CXX_COMPILER", "g++"); });
        app_.add_option("clang").abbr("c")
            .desc("clang++ use to compile ")
            .call_back([&](){ cmake_.define("CMAKE_CXX_COMPILER", "clang++");});
        app_.add_option("release").abbr("r")
            .desc("compile release mode")
            .call_back([&](){ cmake_.build_type="Release"; });
        app_.add_option("debug").abbr("d")
            .desc("compile debug mode")
            .call_back([&](){ cmake_.build_type="Debug"; });
        app_.add_option("clean")
            .desc("clean cmake cache")
            .call_back([&](){ clean = true; });
        app_.add_option("detail")
            .desc("show cppm generate cmake commands")
            .call_back([&](){ cmake_.detail = true; });
        app_.add_option("ntc")
            .desc("not change CMakeLists.txt test options")
            .call_back([&](){ none_tc = true; });
        app_.add_option("tc")
            .desc("only change CMakeLists.txt")
            .call_back([&](){ only_tc = true; });
        app_.add_option("check")
            .desc("dependency check")
            .call_back([&](){ cmake_.no_cache=true; });
        app_.add_option("export")
            .desc("export cppkg")
            .call_back([&](){ config_load(); export_cppkg(); });
        app_.add_option("local")
            .desc("install local")
            .call_back([&](){ cmake_.prefix = ""; clean=true;});
        app_.add_option("global")
            .desc("install global")
            .call_back([&](){ cmake_.sudo=true; cmake_.prefix = "/usr/local"; clean = true; });
        app_.add_command("install")
            .desc("cmake target install ")
            .call_back([&](){ cmake_.install = true; })
            .call_back([&](){ cmake_.build_type="Release"; });
        app_.add_option("prefix")
            .desc("cmake install prefix")
            .call_back([&](nlpo::arg::One arg){ cmake_.prefix = arg; clean=true; });
        app_.add_command().args("{cppm options} {builder options}")
            .desc("Build command")
            .call_back([&](){
                config_load();
                if(cmake_.prefix == "") cmake_.prefix = "{}share/{}-{}"_format(
                                                      core::cppm_root() ,config_->package.name ,*config_->package.version);
                fs::create_directories(config_->path.build);
                if(!none_tc) {
                    auto tranc_cmake = cppm_translate(*config_);
                    if(util::file_hash("{0}/CMakeLists.txt"_format(config_->path.root)) != hashpp::md5(tranc_cmake)) {
                        fmt::print("[cppm] Generate CMakeLists.txt\n");
                        util::write_file("{0}/CMakeLists.txt"_format(config_->path.root), tranc_cmake);
                    }
                    fs::create_directories(config_->path.cmake);
                    util::over_write_copy_file("{0}cmake/cppm_tool.cmake"_format(core::cppm_root())
                                               ,"{0}/cppm_tool.cmake"_format(config_->path.cmake));
                    if(only_tc) { exit(1); }
                }
                if(clean) {
                    fmt::print("[cppm] Clean {}/CMakeCache.txt\n"_format(config_->path.build));
                    fs::remove(config_->path.build/"CMakeCache.txt");
                }
                if(!app_.args().empty()) {
                    cmake_.generator_options(util::str::quot(util::accumulate(app_.args(), " ")));
                    app_.args().clear();
                }
                //if(config_->cppm_config.package.toolchains() != "") {
                //    cmake_.define("CMAKE_TOOLCHAIN_FILE", config_->cppm_config.package.toolchains());
                //}
                if(util::compiler::what() != "msvc"s) {
                    cmake_.generator_options(" -j{} "_format(std::thread::hardware_concurrency()));
                }
                cmake_.build(config_->path.root.string(), "build");
            });
    }

    void Build::export_cppkg() { 
        core::Dependency pkg;
        pkg.name = config_->package.name;
        if(config_->lib) {
            auto& lib = config_->lib;
            pkg.module += lib->install ? "{0}::{1} "_format(config_->package.name, lib->name) : "";
            pkg.type = "lib";
        }
        if(!config_->bins) pkg.type = "bin";
        pkg.description = config_->package.description;
        pkg.git = *config_->package.git_repo;
        pkg.version = "git";
        cppkg::init(pkg);
        cppkg::build(pkg.name);
        exit(0);
    }
}
