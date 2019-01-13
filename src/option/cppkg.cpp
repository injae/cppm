#include "option/cppkg.h"
#include "package/package.h"
#include "option/cppkg_init.h"

#include <fmt/format.h>

using namespace fmt::literals;

namespace cppm::option
{
    Cppkg::Cppkg() {
        app_.add_option("help")
            .abbr("h")
            .desc("show cppm command and options")
            .call_back([&](){ app_.show_help(); });
        app_.add_command("init")
            .desc("init cppkg")
            .args("{option} {name}")
            .call_back([&](){ CppkgInit().app().parse(app_); });
        app_.add_command("build")
            .desc("build cppm package file")
            .args("{cppkg name}")
            .call_back([&](){ _build(); });
        app_.add_command("update")
            .desc("update cppkg repo")
            .call_back([&](){ _update(); });
        app_.add_command("search")
            .desc("search cppkg repo, default is show all")
            .args("{name}")
            .call_back([&](){ _search(); });
        app_.add_command("push")
            .desc("push cppkg in local repo")
            .args("{name}")
            .call_back([&](){ _push(); });
    }

    void Cppkg::_update() {
        using namespace fmt::literals;
        auto cppkg_path = "{0}/.cppm/repo/cppkg"_format(getenv("HOME"));
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

    void Cppkg::_search() {
        using namespace fmt::literals;
        auto list = package::cppkg::list();
        fmt::print("{:<15}{:<20}{:<40}{:<70}\n", "Name", "Version","Description","Use");
        fmt::print("{:=<15}{:=<20}{:=<40}{:=<70}\n", "=", "=","=","=");
        std::string arg;
        if(!app_.args().empty()){ arg = app_.get_arg(); }
        for(auto& [rname, repo] : list.repos) {
            for(auto& [pname, pkg] : repo.pkgs) {
                for(auto& [vname, ver] : pkg.versions) {
                    package::Package package;
                    package.parse(cpptoml::parse_file("{0}/{1}"_format(ver,"cppkg.toml")));
                    if(!arg.empty()) {
                        if(   pname.find(arg) == std::string::npos
                           && package.description.find(arg) == std::string::npos)  {break;}
                    }
                    auto component = package.cmake.components != ""
                                   ? " components=\"{0}\""_format(package.cmake.components) : "";
                    auto use = "{0}={{module=\"{1}\", version=\"{2}\"{3}}}"_format
                                (package.name, package.cmake.name, package.version, component);
                    fmt::print("{:<15}{:<20}{:<40}{:<70}\n", pname, std::string(vname), package.description, use);
                }
            }
        }
    }
}
