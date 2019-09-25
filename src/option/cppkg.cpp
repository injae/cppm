#include "option/cppkg.h"
#include "config/cppm_tool.h"
#include "package/package.h"
#include "option/cppkg_init.h"
#include "util/filesystem.h"
#include "option/cppkg_install.h"

#include <fmt/format.h>

using namespace fmt::literals;

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
            .desc("search cppkg repo, default is show all")
            .call_back([&](){ _search(); });
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

    void Cppkg::_search() {
        using namespace fmt::literals;
        auto list = package::cppkg::list();
        auto str_cut = [](const std::string str, size_t size_) {
            return str.size() > size_ ? str.substr(0,size_-1) + "$" : str;
        };
       
        fmt::print("{:<20}{:<10}{:<13}{:<50}{:<70}\n", "Name", "Version", "Repository","Description","Use");
        fmt::print("{:=<20}{:=<10}{:=<13}{:=<50}{:=<70}\n", "=", "=", "=","=","=");
        std::string arg;
        if(!app_.args().empty()) { arg = app_.get_arg(); }
        for(auto& [rname, repo] : list.repos) {
            for(auto& [pname, pkg] : repo.pkgs) {
                for(auto& [vname, ver] : pkg.versions) {
                    package::Package package;
                    package.parse(cpptoml::parse_file("{0}/{1}"_format(ver,"cppkg.toml")));
                    if(!arg.empty()) {
                        if(   pname.find(arg) == std::string::npos
                           && package.description.find(arg) == std::string::npos) { break; }
                    }
                    auto component = package.cmake.components != ""
                                   ? " components=\"{0}\""_format(package.cmake.components) : "";
                    auto use = "{0}={{module=\"{1}\", version=\"{2}\"{3}}}"_format
                                (package.name, package.cmake.name, package.version, component);
                    fmt::print("{:<20}{:<10}{:<13}{:<50}{:<70}\n"
                               , str_cut(pname, 20), std::string(vname), rname
                               , str_cut(package.description, 50), use);
                }
            }
        }
        if(auto list = util::file_list(Hunter::package_path())) {
            for(auto pkg : *list) {
                auto name = pkg.path().filename().string();
                if(!arg.empty()) { if(name.find(arg) == std::string::npos) { continue; } }

                fmt::print("{:<20}{:<10}{:<13}{:<50}{:<70}\n"
                           , str_cut(name, 20), "latest" , "hunter", "", "https://docs.hunter.sh/en/latest/packages/pkg/{}.html#index-0"_format(name));
            }
        }
    }
}
