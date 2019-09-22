#include "option/cppkg_init.h"
#include "package/package.h"

#include <fmt/format.h>

using namespace fmt::literals;

namespace cppm::option
{
    CppkgInit::CppkgInit() {
        app_.add_option("help").abbr("h")
            .desc("show cppm command and options")
            .call_back([&](){ app_.show_help(); });
        app_.add_option("git").abbr("g").args("{repo}")
            .desc("add git repo")
            .call_back([&](){ pkg.download.git.url = app_.get_arg(); pkg.version = "git";});
        app_.add_option("global")
            .desc("not cppm local install")
            .call_back([&](){ pkg.global = true; });
        app_.add_option("git_tag").abbr("t").args("{tag}")
            .desc("add git tag")
            .call_back([&](){ pkg.download.git.tag = app_.get_arg(); });
        app_.add_option("url").abbr("u").args("{url}")
            .desc("add url repo Require --version")
            .call_back([&](){ pkg.download.url = app_.get_arg(); });
        app_.add_option("module").abbr("m").args("{module}")
            .desc("add module name")
            .call_back([&](){ pkg.cmake.name = app_.get_arg(); });
        app_.add_option("des").abbr("d").args("{description}")
            .desc("add description")
            .call_back([&](){ pkg.description = app_.get_arg(); });
        app_.add_option("version").abbr("v").args("{version}")
            .desc("add library version Require --version")
            .call_back([&](){ pkg.version = app_.get_arg(); });
        app_.add_option("p").abbr("p")
            .desc("cmake base library install to local cppkg repo, Require --url or --git")
            .call_back([&](){ regist = true; });
        app_.add_command().args("{name}")
            .call_back([&](){
                     if(app_.args().size() > 1) { fmt::print(stderr, "too many argument"); exit(1); }
                else if(app_.args().size() < 1) { fmt::print(stderr, "need package name"); exit(1); }
                pkg.name = app_.get_arg();
                package::cppkg::init(pkg);
                package::cppkg::build("{0}"_format(pkg.name));
                if(regist) {
                    if(pkg.download.git.url == ""
                    && pkg.download.url == "") { fmt::print(stderr,"need --url or --git"); exit(1);}
                    if(pkg.version    == "")   { fmt::print(stderr,"need --version"); exit(1);}
                    if(pkg.cmake.name == "")   { fmt::print(stderr,"need --module"); exit(1);}
                    package::cppkg::regist("{0}/{1}"_format(pkg.name,pkg.version));
                }
            });
    }
}
