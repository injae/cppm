#include <thread>
#include <memory>
#include <hash_lib/md5.h>

#include <cstdlib>
#include <range/v3/all.hpp>

#include "cli/build.h"
#include "cppm/util/algorithm.hpp"
#include "cppm/util/filesystem.h"
#include "cppm/cppkg/cppkg.h"
#include "cppm/util/system.hpp"

#include "cppm/util/string.hpp"
#include "cppm/core/cppm_tool.hpp"
#include "cppm_version.h"
#include "cmake/cmake.h"

#include "serdepp/adaptor/nlohmann_json.hpp"
#include "serdepp/adaptor/toml11.hpp"

using namespace fmt::literals;
using namespace std::literals;
using namespace cppm::util::str;
namespace cppm::command
{
    void Build::print_config() {
        switch(format) {
        case PrintFormat::string: return;
        case PrintFormat::json: fmt::print("{}", serde::serialize<nlohmann::json>(*config_).dump(3));; break;
        case PrintFormat::toml: std::cout << serde::serialize<toml::value>(*config_); break;
        default: return;
        }
        exit(0);
    }

    void Build::transcompile(core::Path& path) {
        auto tranc_cmake = cppm_translate(config_.value());
        if(util::file_hash("{0}/CMakeLists.txt"_format(path.root)) != hash::MD5{}(tranc_cmake)) {
            fmt::print("[cppm] Generate CMakeLists.txt\n");
            util::write_file("{0}/CMakeLists.txt"_format(path.root), tranc_cmake);
        }
        fs::create_directories(path.cmake);
        util::over_write_copy_file("{0}cmake/cppm_loader.cmake"_format(core::cppm_root())
                                  ,"{0}/cppm_loader.cmake"_format(path.cmake));
    }

    void Build::export_cppkg() { 
        //core::Dependency pkg;
        //pkg.name = config_->package.name;
        //if(config_->lib) {
        //    auto& lib = config_->lib;
        //    pkg.module =
        //        lib->install
        //            ? "{0}::{1} "_format(config_->package.name, lib->name)

        //    pkg.type = core::cppkg_type::lib;
        //}
        //if (!config_->bins.empty()) pkg.type = core::cppkg_type::bin;
        //pkg.description = confi
        //pkg.git = *config_->package.git_repo;
        //pkg.version = "git";
        //cppkg::init(pkg);
        //cppkg::build(pkg.name);
        //exit(0);
    }

    void Build::execute(Build& build) {
        fmt::print("{}\n",build);
        build.config_ = core::cppm_config_load(false);
        auto cmake_script = util::reverse_find_file(fs::current_path() ,"CMakeLists.txt");
        auto& cmake = build.cmake_;
        auto& config = build.config_;
        bool cppm_project = config.has_value();

        core::Path path = config ? build.config_->path
                                        : core::Path::make(cmake_script->parent_path().string());

        cmake.detail = build.detail;

        if(!build.generator.empty()) { cmake.generator(build.generator); }
        if(!build.compiler.empty()) { cmake.define("CMAKE_CXX_COMPILER", build.compiler); }

        if(!cppm_project) {
            if(!cmake_script) {
                throw CLI::Error("Parse Error", "this package is not cmake project\n");
            }
            fmt::print("this package is not cppm package\n build with cppm toolchain");
        }

        cmake.toolchain = "{}cmake/cppm-tools-{}/toolchain.cmake"_format(core::cppm_root(), CPPM_TOOLS_VERSION);
        if(cppm_project && config->cmake.toolchain) {
            cmake.define("CPPM_EXTERNAL_TOOLCHAIN_FILE", config->cmake.toolchain.value());
        }
        if(!build.toolchain.empty()) {
            cmake.define("CPPM_EXTERNAL_TOOLCHAIN_FILE", build.toolchain);
        }

        if(build.prefix.empty()) cmake.define("USE_CPPM_PATH", "ON");

        auto deps = build.define
            | views::transform([](auto def) { return split(def, '='); })
            | to_vector;
            
        for(auto& dep : deps | views::filter([](auto vec) { return vec.size() == 2; })) {
            cmake.define(dep[0], dep[1]);
        }

        fs::create_directories(path.build);
        if(!build.ntc) {
            build.transcompile(path);
            if(build.tc) { throw CLI::Success{}; }
        }

        if(build.clean) {
            fmt::print("[cppm] Clean {}/CMakeCache.txt\n"_format(path.build));
            fs::remove(config->path.build/"CMakeCache.txt");
        }

        if(build.args.empty()) {
            cmake.generator_options(util::str::quot(util::accumulate(build.args, " ")));
        }

        if(util::compiler::what() != "msvc"s) {
            cmake.generator_options(" -j{} "_format(std::thread::hardware_concurrency()));
        }
        cmake.build(path.root.string(), "build");

        throw CLI::Success{};
    }

    void Build::Install::callback(std::vector<std::string>& args) {
        auto cmake_script = util::reverse_find_file(fs::current_path() ,"CMakeLists.txt");
        core::Path path = core::Path::make(cmake_script->parent_path().string());
        cmake::Cache cache(path.build);
        fs::path p;
        auto command = " cmake --install . && cmake --build . --target cppm_link";
        system(command);

        throw CLI::Success{};
    }

    void Build::Uninstall::callback(std::vector<std::string>& args) {
        auto cmake_script = util::reverse_find_file(fs::current_path() ,"CMakeLists.txt");
        core::Path path = core::Path::make(cmake_script->parent_path().string());
        cmake::Cache cache(path.build);
        auto command = " cmake --build . --target uninstall";
        system(command);
        throw CLI::Success{};
    }
}
