#include "option/cppkg.h"
#include "config/cppm_tool.h"
#include "package/package.h"
#include "option/cppkg_init.h"
#include "util/filesystem.h"
#include "util/string.hpp"
#include "option/cppkg_install.h"
#include "option/cppkg_search.h"

#include <fmt/format.h>

using namespace fmt::literals;
using namespace cppm::util::str;

namespace cppm::option
{
    Cppkg::Cppkg() {
        app_.add_option("help").abbr("h")
            .desc("show cppm command and options")
            .call_back([&](){ app_.show_help(); });
        app_.add_command("init").args("{option} {name}")
            .desc("init cppkg")
            .call_back([&](){ CppkgInit().app().parse(app_); });
        app_.add_command("build").args("{cppkg name}")
            .desc("build cppm package file")
            .call_back([&](){ _build(); });
        app_.add_command("update")
            .desc("update cppkg repo")
            .call_back([&](){ _update(); });
        app_.add_command("search").args("{name}")
            .desc("search cppkg repo, default is only cppkg")
            .call_back([&](){ CppkgSearch().app().parse(app_); });
        app_.add_command("push").args("{name}")
            .desc("push cppkg in local repo")
            .call_back([&](){ _push(); });
        app_.add_command("install").args("{name}")
            .desc("install cppkg package")
            .call_back([&](){ CppkgInstall().app().parse(app_); });
    }


    void Cppkg::_update() {
        using namespace fmt::literals;
        auto cppkg_path = "{0}repo/cppkg"_format(tool::cppm_root());
        auto command = "cd {0} && git pull"_format(cppkg_path);
        system(command.c_str());
    }

    void Cppkg::_build() {
        if(app_.args().empty())    { fmt::print(stderr,"need argument");     exit(1);}
        if(app_.args().size() > 1) { fmt::print(stderr,"too many argument"); exit(1);}
        package::cppkg::build(app_.get_arg());
    }

    void Cppkg::_push() {
        if(app_.args().empty())    { fmt::print(stderr,"need argument");}
        if(app_.args().size() > 1) { fmt::print(stderr,"too many argument");}
        package::cppkg::regist(app_.get_arg());
    }

}
