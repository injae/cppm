#include "option/cppkg_init.h"

using namespace fmt::literals;

namespace cppm::option
{
    CppkgInit::CppkgInit() {
        app_.add_option("git").abbr("g").args("{repo}")
            .desc("add git repo")
            .call_back([&](){ pkg.git = app_.get_arg(); pkg.version = "git";});
        app_.add_option("git_tag").abbr("t").args("{tag}")
            .desc("add git tag")
            .call_back([&](){ pkg.branch = app_.get_arg(); });
        app_.add_option("url").abbr("u").args("{url}")
            .desc("add url repo Require --version")
            .call_back([&](){ pkg.url = app_.get_arg(); });
        app_.add_option("module").abbr("m").args("{module}")
            .desc("add module name")
            .call_back([&](){ pkg.module = app_.get_arg(); });
        app_.add_option("des").abbr("d").args("{description}")
            .desc("add description")
            .call_back([&](){ pkg.description = app_.get_arg(); });
        app_.add_option("type").abbr("t")
            .desc("add type default is lib")
            .call_back([&](nlpo::arg::One arg){ pkg.type = arg; });
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
                if(!pkg.type) pkg.type = "lib";
                cppkg::init(pkg);
                cppkg::build(pkg.name);
                if(regist) {cppkg::regist("{}/{}"_format(pkg.name, pkg.version));}
            });
    }
}
