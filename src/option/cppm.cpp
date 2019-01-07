#include <string>

#include "option/cppm.h"
#include "util/filesystem.h"
#include "util/algorithm.hpp"
#include "option/build.h"
#include "option/init.h"
#include "option/add.h"
#include "package/package.h"

#include <fmt/format.h>

namespace cppm::option
{
    Cppm::Cppm() {
        app_.add_option("help")
            .abbr("h")
            .desc("show cppm commands and options")
            .call_back([&](){ app_.show_help(); });
        app_.add_command("add")
            .desc("add cppm config")
            .call_back([&](){ Add().app().parse(app_); });
        app_.add_command("init")
            .desc("make c++ project")
            .call_back([&](){ Init().app().parse(app_); });
        app_.add_command("update")
            .desc("update cppkg repos")
            .call_back([&](){ _update(); });
        app_.add_command("search")
            .desc("search cppkg")
            .call_back([&](){ _search(); });
        app_.add_command("build")
            .desc("make CmakeLists.txt and project build")
            .call_back([&](){ Build().app().parse(app_); });
        app_.add_command("run")
            .desc("run binary file(run build/{project_name}) argument is binary argument")
            .call_back([&](){ _run(); });
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
        auto cppkg_path = "{0}/.cppm/repo/cppkg"_format(getenv("HOME"));
        auto command = "cd {0} && git fetch"_format(cppkg_path);
        system(command.c_str());
    }

    void Cppm::_search() {
        using namespace fmt::literals;
        auto list = package::cppkg::list();
        for(auto& [rname, repo] : list.repos) {
            for(auto& [pname, pkg] : repo.pkgs) {
                for(auto& [vname, ver] : pkg.versions) {
                    fmt::print("{:<10}{:^20}\n", pname, std::string(vname));
                }
            }
        }
    }
    void Cppm::run(int argc, char **argv) {
        app_.parse(argc, argv);
    }
}
