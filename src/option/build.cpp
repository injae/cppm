#include <iostream>
#include <thread>
#include <memory>
#include <fmt/format.h>
#include <hashpp/md5.h>

#include <cstdlib>
#include <string>

#include "option/build.h"
#include "cppm/util/algorithm.hpp"
#include "cppm/util/filesystem.h"
#include "cppm/cppkg/cppkg.h"
#include "cppm/util/system.hpp"
#include "cppm/util/string.hpp"
#include "cppm/core/cppm_tool.hpp"
#include "cppm_version.h"
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
        app_.add_option("toolchains")
            .desc("build with cmake toolchain")
            .call_back([&](){ cmake_.toolchain = app_.get_arg(); });
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
        app_.add_command("uninstall")
            .desc("cmake target uninstall")
            .call_back([&](){ cmake_.set_target("uninstall"); });
        app_.add_option("prefix")
            .desc("cmake install prefix")
            .call_back([&](nlpo::arg::One arg){ cmake_.prefix = arg; clean=true; });
        app_.add_command().args("{cppm options} {builder options}")
            .desc("Build command")
            .call_back([&](){
                auto is_cppm = config_load(false);
                if(!is_cppm) { none_tc = true; }
                auto cmake_script = util::reverse_find_file(fs::current_path() ,"CMakeLists.txt");
                if(!is_cppm && !cmake_script) { fmt::print(stderr,"this package is not cmake project\n"); exit(1); }
                core::Path path = is_cppm ? config_->path : core::Path::make(cmake_script->parent_path().string());
                if(is_cppm) {
                    if(config_->cmake && config_->cmake->toolchain) {
                        cmake_.define("CMAKE_EXTERNAL_TOOLCHAIN_FILE", config_->cmake->toolchain.value());
                    }
                }
                if(cmake_.toolchain) {
                    cmake_.define("CMAKE_EXTERNAL_TOOLCHAIN_FILE", cmake_.toolchain.value());
                }
                cmake_.toolchain = "{}cppkg/cppm-tools-{}/toolchain.cmake"_format(core::cppm_root(), CPPM_VERSION);
                if(cmake_.prefix == "") cmake_.define("USE_CPPM_PATH", "ON");
                                        
                fs::create_directories(path.build);
                if(!none_tc) {
                    auto tranc_cmake = cppm_translate(*config_);
                    if(util::file_hash("{0}/CMakeLists.txt"_format(path.root)) != hashpp::md5(tranc_cmake)) {
                        fmt::print("[cppm] Generate CMakeLists.txt\n");
                        util::write_file("{0}/CMakeLists.txt"_format(path.root), tranc_cmake);
                    }
                    fs::create_directories(path.cmake);
                    util::over_write_copy_file("{0}cmake/cppm_tool.cmake"_format(core::cppm_root())
                                               ,"{0}/cppm_tool.cmake"_format(path.cmake));
                    if(only_tc) { exit(1); }
                }
                if(clean) {
                    fmt::print("[cppm] Clean {}/CMakeCache.txt\n"_format(path.build));
                    fs::remove(config_->path.build/"CMakeCache.txt");
                }
                if(!app_.args().empty()) {
                    cmake_.generator_options(util::str::quot(util::accumulate(app_.args(), " ")));
                    app_.args().clear();
                }
                if(util::compiler::what() != "msvc"s) {
                    cmake_.generator_options(" -j{} "_format(std::thread::hardware_concurrency()));
                }
                cmake_.build(path.root.string(), "build");
            });
    }

    void Build::export_cppkg() { 
        core::Dependency pkg;
        pkg.name = config_->package.name;
        if(config_->lib) {
            auto& lib = config_->lib;
            pkg.module =
                lib->install
                    ? "{0}::{1} "_format(config_->package.name, lib->name)
                    : "";
            pkg.type = "lib";
        }
        if (!config_->bins)
          pkg.type = "bin";
        pkg.description = config_->package.description;
        pkg.git = *config_->package.git_repo;
        pkg.version = "git";
        cppkg::init(pkg);
        cppkg::build(pkg.name);
        exit(0);
    }
}
