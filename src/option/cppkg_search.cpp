#include "option/cppkg_search.h"
#include "util/string.hpp"
#include "util/filesystem.h"
#include "util/algorithm.hpp"
#include <fmt/format.h>

using namespace fmt::literals;
using namespace cppm::util::str;

namespace cppm::option
{
    //  is_all or repo_name > name
    CppkgSearch::CppkgSearch(){
        app_.add_option("all").abbr("a")
            .desc("show all package")
            .call_back([&](){ is_all = true; });
        app_.add_option("repo").abbr("r").args("{repo name}")
            .desc("show {repo_name}'s packages")
            .call_back([&](){ repo_name = app_.get_arg(); show_one = true; });
        app_.add_command().args("{packag name}")
            .call_back([&](){
                auto name = !app_.args().empty() ? app_.get_arg() : "";
                if(is_all) { repo_name = ""; name = ""; }
                fmt::print("{:<20}{:<10}{:<13}{:<50}{:<70}\n"
                           , "Name", "Version", "Repository","Description","Use");
                fmt::print("{:=<20}{:=<10}{:=<13}{:=<50}{:=<70}\n"
                           , "=", "=", "=","=","=");
                auto list = package::cppkg::list();
                for(auto& [rname, repo] : list.repos) {
                    if(!is_all && show_one && repo_name == rname) continue;
                    else if(!is_all && !show_one && rname == "hunter") continue;

                    for(auto& [pname, pkg] : repo.pkgs) {
                        for(auto& [vname, ver] : util::reverse(pkg.versions)) {
                            package::Package package;
                            package.parse(cpptoml::parse_file("{0}/{1}"_format(ver,"cppkg.toml")));
                            if(name != ""&&!has_str(pname, name)&&!has_str(package.description, name)) break; 
                            fmt::print("{:<20}{:<10}{:<13}{:<50}{:<70}\n"
                                      , str_cut(pname, 20), std::string(vname), rname
                                      , str_cut(package.description, 50), _make_use_column(package,rname));
                        }
                    }
                }
                if(is_all || repo_name == "hunter" || name != "") { _show_hunter_package(name); }
        });
    }

    std::string CppkgSearch::_make_use_column(const package::Package& pkg, const std::string& repo) {
        auto component = pkg.cmake.components != ""
            ? " components={0}"_format(quot(pkg.cmake.components)) : "";
        return repo == "cppkg" && component == ""
               ? "{0}={1}"_format(pkg.name, quot(pkg.version))
               : "{0}={{module={1}, version={2}{3}}}"_format(
                  pkg.name, quot(pkg.cmake.name), quot(pkg.version), component);

    }

    void CppkgSearch::_show_hunter_package(const std::string& target){
        if(auto list = util::file_list(Hunter::package_path())) {
            for(auto pkg : *list) {
                auto name = pkg.path().filename().string();
                if(!has_str(name, target)) { continue; }
                fmt::print("{:<20}{:<10}{:<13}{:<50}{:<70}\n"
                        , str_cut(name, 20), "latest" , "hunter", ""
                        , "https://docs.hunter.sh/en/latest/packages/pkg/{}.html#index-0"_format(name));
            }
        }
    }
}
