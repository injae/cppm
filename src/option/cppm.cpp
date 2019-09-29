#include <string>
#include <iostream>
#include "option/cppm.h"
#include "util/algorithm.hpp"
#include "option/build.h"
#include "option/init.h"
#include "option/cppm_config.h"
#include "package/package.h"
#include "config/cppm_package.h"
#include "cppm_version.h"
#include "option/cppkg.h"

#include <fmt/format.h>

namespace cppm::option
{
    Cppm::Cppm() {
        app_.add_option("help")
            .abbr("h")
            .desc("show cppm commands and options")
            .call_back([&](){ app_.show_help(); });
        app_.add_option("version")
            .abbr("v")
            .desc("show cppm version")
            .call_back([&](){ fmt::print("cppm version {}\n", CPPM_VERSION);});
        app_.add_command("config")
            .desc("cppm config setting")
            .call_back([&](){ CppmConfig().app().parse(app_); });
        app_.add_command("init")
            .desc("make c++ project")
            .call_back([&](){ Init().app().parse(app_); });
        app_.add_command("update")
            .desc("update cppm version")
            .call_back([&](){ _update(); });
        app_.add_command("build")
            .desc("make CmakeLists.txt and project build")
            .call_back([&](){ Build().app().parse(app_); });
        app_.add_command("run")
            .desc("run binary file(run build/{project_name}) argument is binary argument")
            .args("{binary options}")
            .is_show(false)
            .call_back([&](){ _run(); });
        app_.add_command("cppkg")
            .desc("cppkg option and commands")
            .call_back([&](){ Cppkg().app().parse(app_); });
    } 

    void Cppm::_run() {
        using namespace fmt::literals;
        config_load();
        auto binary_path = "{0}/{1}"_format(config_.path.build,config_.package.name);
        auto args = util::accumulate(app_.args(), " "); app_.args().clear();
        system((binary_path + args).c_str());
    }

    void Cppm::_update() {
        using namespace fmt::literals;
        CppmPackage config;
        config.parse(cpptoml::parse_file(CppmPackage::config_path()));

        system("cppm cppkg install cppm");
    }

    void Cppm::run(int argc, char **argv) {
        app_.parse(argc, argv);
    }
}
