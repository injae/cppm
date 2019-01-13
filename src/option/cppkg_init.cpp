#include "option/cppkg_init.h"
#include "package/package.h"

#include <fmt/format.h>

using namespace fmt::literals;

namespace cppm::option
{
    CppkgInit::CppkgInit() {
        app_.add_option("help")
            .abbr("h")
            .desc("show cppm command and options")
            .call_back([&](){ app_.show_help(); });
        app_.add_option("git")
            .abbr("g")
            .desc("add git repo")
            .args("{repo}")
            .call_back([&](){ pkg.download.git.url = app_.get_arg();
                              pkg.version = "lastest";});
        app_.add_option("url")
            .abbr("u")
            .desc("add url repo Require --version")
            .args("{url}")
            .call_back([&](){ pkg.download.url = app_.get_arg(); });
        app_.add_option("module")
            .abbr("m")
            .desc("add module name")
            .args("{module}")
            .call_back([&](){ pkg.cmake.name = app_.get_arg(); });
        app_.add_option("v")
            .abbr("u")
            .desc("add library version Require --url")
            .args("{version}")
            .call_back([&](){ pkg.version = app_.get_arg(); });
        app_.add_option("regist")
            .desc("cmake base library install to local cppkg repo, Require --url or --git")
            .abbr("r")
            .call_back([&](){ regist = true; });
        app_.add_command()
            .args("{name}")
            .call_back([&](){
                if(app_.args().size() > 1) { fmt::print(stderr, "too many argument"); exit(1); }
                if(!regist) package::cppkg::init(app_.get_arg());
                if(pkg.download.git.url == ""
                   && pkg.download.url == "") {fmt::print(stderr,"need --url or --git"); exit(1);}
                if(pkg.version    == "") {fmt::print(stderr,"need --version"); exit(1);}
                if(pkg.cmake.name == "") {fmt::print(stderr,"need --module"); exit(1);}
                pkg.name = app_.get_arg();
                package::cppkg::init(pkg);
                package::cppkg::build("{0}"_format(pkg.name));
                package::cppkg::regist("{0}/{1}"_format(pkg.name,pkg.version));
            });
    }
}
