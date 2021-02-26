#include <string>
#include <iostream>
#include "option/cppm.h"
#include "option/build.h"
#include "option/init.h"
//#include "option/cppm_config.h"
#include "cppm_version.h"
#include "option/cppkg.h"
#include "cppm/util/algorithm.hpp"
#include "cppm/util/filesystem.h"

#include <fmt/format.h>

namespace cppm::option
{
    Cppm::Cppm() {
        app_.name("cppm");
        app_.add_option("version").abbr("v")
            .desc("show cppm version")
            .call_back([&](){ fmt::print("cppm version {}\n", CPPM_VERSION);});
        //app_.add_command<CppmConfig>("config")
        //    .desc("cppm config setting")
        //    .is_show(false);
        app_.add_command<Init>("init")
            .desc("make c++ project");
        app_.add_command("update")
            .desc("update cppm version")
            .call_back([&](){ _update(); });
        app_.add_command<Build>("build")
            .desc("make CmakeLists.txt and project build");
        app_.add_command("run")
            .desc("run binary file(run build/{project_name}) argument is binary argument")
            .args("{binary options}")
            .is_show(false)
            .call_back([&](){ _run(); });
        app_.add_command<Cppkg>("cppkg").desc("cppkg option and commands");
        app_.add_command("test")
            .desc("run the tests")
            .args("{ctest flags}")
            .call_back([&]() {
              config_load();
              util::working_directory(config_->path.build.string().c_str());
              auto str_args = "ctest " + util::accumulate(app_.args(), " ");
              app_.args().clear();
              system(str_args.c_str());
            });
    }
    void Cppm::_run() {
        using namespace fmt::literals;
        config_load();
        auto cache = cmake::Cache(config_->path.build.string());
        auto binary_path = "{0}/{1}/{2}"_format(config_->path.build, cache["CMAKE_BUILD_TYPE"], config_->package.name);
        auto args = util::accumulate(app_.args(), " "); app_.args().clear();
        system((binary_path + args).c_str());
    }

    void Cppm::_update() {
        using namespace fmt::literals;
        system("cppm cppkg install cppm");
    }

    void Cppm::run(int argc, char **argv) {
        app_.parse(argc, argv);
    }
}
