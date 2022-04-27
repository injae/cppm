
#include "cli/cppkg_search.h"
#include "cppm/core/cppm_tool.hpp"
#include "cppm/util/string.hpp"
#include "cppm/util/filesystem.h"
#include "cppm/util/algorithm.hpp"
#include "cppm/cppkg/cppkg.h"

#include <serdepp/adaptor/fmt.hpp>

namespace cppm::command {
    using namespace fmt::literals;
    using namespace cppm::util::str;

    std::string make_use_column(const core::Dependency& dep, const std::string& repo) {
        auto component = dep.components
            ? " components={0}"_format(quot(*dep.components)) : "";
        return repo == "cppkg" && component == ""
               ? "{0}={1}"_format(dep.name, quot(dep.version))
               : "{0}={{module={1}, version={2}{3}}}"_format(
                  dep.name, quot(*dep.module), quot(dep.version), component);
    }

    void show_hunter_package(const std::string& target){
        if(auto list = util::file_list(core::hunter_root())) {
            for(auto pkg : *list) {
                auto name = pkg.path().filename().string();
                if(!has_str(name, target)) { continue; }
                fmt::print("{:<20}{:<10}{:<13}{:<50}{:<70}\n"
                        , str_cut(name, 20), "latest" , "hunter", ""
                        , "https://hunter.readthedocs.io/en/latest/packages/pkg/{}.html"_format(name));
                           
            }
        }
    }
    
    void CppkgSearch::execute(CppkgSearch& cmd) {
        fmt::print("update cppkg repo version\n");
        auto cppkg_path = "{0}repo/cppkg"_format(core::cppm_root());
        if(!fs::exists(cppkg_path)) {
            fmt::print("clone cppkg path:{}\n",cppkg_path);
            system("git clone https://github.com/injae/cppkg.git {}"_format(cppkg_path).c_str());
        }
        auto command = "cd {0} && git pull origin master"_format(cppkg_path);
        system(command.c_str());

        auto name = cmd.target;
        if(cmd.all) { cmd.repo = ""; name = ""; }
        fmt::print("{:<20}{:<10}{:<13}{:<50}{:<70}\n"
                    , "Name", "Version", "Repository","Description","Use");
        fmt::print("{:=<20}{:=<10}{:=<13}{:=<50}{:=<70}\n"
                    , "=", "=", "=","=","=");
        auto list = cppkg::list();
        for(auto& [rname, repo] : list) {
            if(!cmd.all && cmd.one && cmd.repo == rname) continue;
            else if(!cmd.all && !cmd.one && rname == "hunter") continue;

            for(auto& [pname, pkg] : repo.pkgs) {
                for(auto& [vname, ver] : util::reverse(pkg.versions)) {
                    auto dep = cppkg::parse(pname, ver);
                    if(name != ""&&!has_str(pname, name)&&!has_str(dep.description, name)) break; 
                    fmt::print("{:<20}{:<10}{:<13}{:<50}{:<70}\n"
                                , str_cut(pname, 20), std::string(vname), rname
                                , str_cut(dep.description, 50), make_use_column(dep,rname));
                }
            }
        }
        if(cmd.all || cmd.repo == "hunter" || name != "") { show_hunter_package(name); }

        throw CLI::Success();
    }
}