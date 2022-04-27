#include "cli/cppm.h"
#include "cppm_version.h"
#include "cppm/util/algorithm.hpp"
#include "cppm/util/filesystem.h"
#include "cppm/core/config.hpp"
#include "cmake/cmake.h"

namespace cppm::command
{
    void Run::execute(Run& cmd) {
        using namespace fmt::literals;
        if(auto config = cppm::core::cppm_config_load("") ) {
            auto cache = cmake::Cache(config->path.build.string());
            auto binary = cmd.target.empty() ? config->package.name : cmd.target;

            auto binary_path = "{0}/{1}/{2}"_format(config->path.build,
                                                    cache["CMAKE_BUILD_TYPE"],
                                                    binary);
            auto args = util::accumulate(cmd.args, " ");
            system("{} {}"_format(binary_path, args).c_str());
            throw CLI::Success{};
        } else {
            throw CLI::Error("parse error", "can't find cppm.toml");
        }
            
    }

    void Cppm::execute(Cppm& cmd) {
        if (cmd.version) {
            fmt::print("cppm version: {}\n", CPPM_VERSION);
        }
        throw CLI::Success{};
    }

    // void Cppm::_update_(cpcli::Command& cmd) {
    //     using namespace fmt::literals;
    //     fmt::print("update\n");
    //     //system("cppm cppkg install cppm");
    //}

    void CTest::execute(CTest& cmd) {
        using namespace fmt::literals;
        if (auto config = cppm::core::cppm_config_load("")) {
            util::working_directory(config->path.build.string().c_str());
            system("ctest {}"_format(util::accumulate(cmd.args, " ")).c_str());
            throw CLI::Success{};
        } else {
            throw CLI::Error("parse error", "can't find cppm.toml");
        }
    }

}
