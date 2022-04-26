#pragma once

#ifndef __CPPM_CLI_BUILD_H__
#define __CPPM_CLI_BUILD_H__

#include "cppm/core/config.hpp"
#include "cli/util.hpp"
#include "cmake/cmake.h"

#include <serdepp/attribute/cli11.hpp>

namespace cppm::command
{
    using namespace serde::attribute::cli11;
    enum class PrintFormat { none, string, json, toml };

    class Build 
    {
    public:
        DERIVE_SERDE(Build,
                     .attrs(desc{"make CMakeLists.txt and project build"},
                            option_group("Compiler",  "Compiler Options -DCMAKE_CXX_COMPILER",    [](auto* g){ g->require_option(0,1); }),
                            option_group("Generator", "Generator Options -DCMAKE_GENERATOR",   [](auto* g){ g->require_option(0,1); }),
                            option_group("Build",     "CMake Build Options", [](auto* g){ g->require_option(0,1); }),
                            option_group("CMake",     "CMake Flags"),
                            callback(Build::execute))

                     // Generator
                     [attrs(group_flag("Generator","-G", "cmake -G option"))]_SF_(generator)
                     [attrs(group_flag("Generator","-N{Ninja}", "Use Ninja Generator"))]_SF_(generator)
                     [attrs(group_flag("Generator","-M{Unix Makefiles}", "Use Unix Makefiles Generator"))]_SF_(generator)

                     // Compiler
                     [attrs(group_flag("Compiler", "--gcc{g++}", "Build With Gcc Compiler"))]_SF_(compiler)
                     [attrs(group_flag("Compiler", "--clang{clang++}", "Build With Clang Compiler"))]_SF_(compiler)
                     [attrs(group_flag("Compiler", "-C", "Build With Selected Compiler"))]_SF_(compiler)

                     // Build
                     [attrs(group_flag("Build", "-r,--release", "Compile Release Build"))]_SF_(release)
                     [attrs(group_flag("Build", "-d,--debug","Compile Debug Build"))]_SF_(debug)
                     [attrs(group_flag("Build", "--clean", "clean CMakeCache.txt"))]_SF_(clean)
                     [attrs(group_flag("Build", "--check","Check CMakeLists Changes"))]_SF_(check)

                     // Transcompile
                     [attrs(flag("--ntc",   "Not Change CMakeLists.txt"))]_SF_(ntc)
                     [attrs(flag("--tc", "Only Change CMakeLists.txt"))]_SF_(tc)

                     // Install Options
                     [attrs(flag("--local", "install cppm path"))]_SF_(local)
                     [attrs(flag("--global", "install syatem path /usr/local"))]_SF_(global)
                     [attrs(flag("--prefix", "CMake install prefix"))]_SF_(prefix)

                     // CMake Options
                     [attrs(group_option("CMake","--toolchain", "Set CMake Toolchain file"))]_SF_(toolchain)
                     [attrs(group_option("CMake","-D", "Set CMake Cache, same cmake -D flag"))]_SF_(define)
                     [attrs(group_option("CMake","--target", "cmake target"))]_SF_(target)

                     // Cppm Options
                     [attrs(option("-f,--format", "print cppm config"))]_SF_(format)
                     [attrs(flag("--detail",      "Show Cppm Generate Cmake Commands"))]_SF_(detail)
                     [attrs(flag("--export",      "export cppkg file"))](&Self::export_, "export")

                     // SubCommand
                     (&Self::install_, "install")
                     (&Self::uninstall_, "uninstall")
                     (&Self::args, "args")
                     )

    private:
        static void execute(Build& cmd);
        struct Uninstall : callback_interface {
            static constexpr auto desc ="uninstall package";
            static void callback(std::vector<std::string>& args);
        };

        struct Install : callback_interface {
            static constexpr auto desc ="install package";
            static void callback(std::vector<std::string>& args);
        };

        bool release, debug;
        bool clean, check;
        bool ntc, tc;
        bool local, global;
        bool detail;
        bool export_;
        std::string generator;
        std::string compiler;
        std::string prefix;
        PrintFormat format;
        std::vector<std::string> install_cmd;
        std::string target;
        std::string toolchain;
        std::vector<std::string> define;

        SingleCommand<Install> install_;
        SingleCommand<Uninstall> uninstall_;
        std::vector<std::string> args;
    private:
        void export_cppkg();
        void print_config();
        void transcompile(core::Path& path);

    private:
        cmake::Cmake cmake_;
        std::optional<core::Config> config_;
        
    };
}


#endif
