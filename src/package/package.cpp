#include "package/package.h"
#include "config/dependency.h"
#include "util/filesystem.h"
#include "util/algorithm.hpp"
#include <fmt/format.h>
#include <map>
#include <iostream>

namespace cppm::package
{
    void Package::parse(table_ptr table) {
        if(auto package = table->get_table("package")) {
            name    = package->get_as<std::string>("name").value_or("");
            version = package->get_as<std::string>("version").value_or("");
            description = package->get_as<std::string>("description").value_or("");
            global  = package->get_as<bool>("global").value_or(false);

            auto cmake_      = package->get_table("cmake");
            cmake.name       = cmake_->get_as<std::string>("name").value_or("");
            cmake.option     = cmake_->get_as<std::string>("option").value_or("");
            cmake.components = cmake_->get_as<std::string>("components").value_or("");
            cmake.find_lib   = cmake_->get_as<std::string>("findlib").value_or("");

            auto download_   = package->get_table("download");
            download.url     = download_->get_as<std::string>("url").value_or("");
            download.git.url = download_->get_as<std::string>("git").value_or("");
            download.git.tag = download_->get_as<std::string>("git_tag").value_or("");
            deps.parse(table);
        }
    }

    std::string Package::generate() {
        using namespace fmt::literals;
        std::string git_url;
        std::string git_tag;
        std::string url;
        std::string desc; 
        std::string install = global ? "GLOBAL" : "LOCAL";
        if(download.git.url != "") git_url = "GIT {0}"_format(download.git.url);
        if(download.git.tag != "") git_tag = "GIT_TAG {0}"_format(download.git.tag);
        if(download.url     != "") url = "URL {0}"_format(download.url);
        
        desc += "# Other Options:\n";
        desc += "# - Linux Configures:\n";
        desc += "#    L_CONFIGURE {...}, L_BUILD {...}, L_INSTALL {...}\n";
        desc += "# - Windows Configures:\n";
        desc += "#    W_CONFIGURE {...}, W_BUILD {...}, W_INSTALL {...}\n";
        desc += "# - Install Path Options:\n";
        desc += "#    LOCAL(default) GLOBAL \n";

        return "cmake_minimum_required(VERSION 3.6)\n"
             + "project({0}-{1}-install NONE)\n\n"_format(name, version)
             + "include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cppm_tool.cmake)\n"
             + deps.gen_find_package()
             + "download_package({0} {1} {2} {3} {4} {5})\n\n"_format(
                                 name, version, git_url, git_tag, url, install)
             + desc;
    }
namespace cppkg {
    void init(const std::string& name) {
        using namespace fmt::literals;
        Package package;
        package.name = name;
        package.version = "lastest";
        init(package);
    }

    void init(Package& package) {
        using namespace fmt::literals;
        auto value = [](const std::string& str) { return "\""+ str +"\""; };
        auto is_default = [](std::string str) { return str != "" ? "" : "#"; };
        if(fs::exists(package.name + ".toml")) {
            fmt::print(stderr, "existed {0}", package.name);
            exit(1);
        }
        util::create("{0}.toml"_format(package.name));
        util::write( "{0}.toml"_format(package.name)
                   , "[package]\n" + "name = {0}\n"_format(value(package.name))
                   + "# if you use git repo, package version is lastest\n"
                   + "version = {0}\n"_format(value(package.version))
                   + "description = {0}\n"_format(value(package.description))
                   + "global={0}\n"_format(package.global ? "true" : "false")
                   + "# library name in cmake, cppm.toml use this value \n"
                   + "# [dependencies]\n"
                   + "#${library_name} = {module =${value}}\n"
                   + "cmake = {{name = {0}, findlib={1}}}\n"_format(value(package.cmake.name)
                                                                   ,value(package.cmake.find_lib))
                   + "# write download value git or url\n"
                   + "{0}download = {{git={1}}}\n"_format(is_default(package.download.git.url)
                                                         ,value(package.download.git.url))
                   + "{0}download = {{url={1}}}\n"_format(is_default(package.download.url)
                                                         ,value(package.download.url))
                   + package.deps.generate()
                   ); 
    }

    void build(const std::string& name) {
        using namespace fmt::literals;
        auto table = cpptoml::parse_file(name + ".toml");
        package::Package package;
        package.parse(table);
        auto dir_name = "{0}/{1}"_format(package.name,package.version);
        if(fs::exists(dir_name)) { 
            fmt::print(stderr,"existed {0}/{1}",package.name, package.version); 
            exit(1);
        }
        fs::create_directories(dir_name);
        fs::copy(package.name + ".toml", "{0}/{1}"_format(dir_name,"cppkg.toml"));
        fs::remove(package.name + ".toml");
        auto file = "{0}/{1}.cmake.in"_format(dir_name,package.name);
        util::write(file, package.generate());
        auto dep_file = "{0}/dep.cmake"_format(dir_name);
        util::create(dep_file);
        util::write(dep_file,package.deps.gen_find_package());
    }

    void regist(const std::string& name) {
        using namespace fmt::literals;
        Package package;
        package.parse(cpptoml::parse_file("{0}/cppkg.toml"_format(name)));
        auto local_path = "{0}/.cppm/repo/local"_format(getenv("HOME"));
        auto pack_path = "{0}/{1}/{2}"_format(local_path,package.name,package.version);
        if(!fs::exists(local_path)) fs::create_directories(pack_path);
        if(fs::exists(pack_path)) fs::remove_all(pack_path);
        util::recursive_copy(name,pack_path);
        fmt::print("update {0}/{1} in local-repo [\"{2}\"]\n", package.name, package.version, pack_path);
    }

    Cppkg list() {
        using namespace fmt::literals;
        auto root_path = "{0}/.cppm/repo"_format(getenv("HOME"));
        Cppkg cppkg;
        auto repos = *util::file_list(root_path);
        for(auto repo : repos) {
            auto rname = repo.path().filename().string();
            auto pkgs = *util::file_list("{0}/{1}"_format(root_path,rname));
            for(auto& pkg : pkgs) {
                auto pname = pkg.path().filename().string();
                if(pname == ".git" || pname == "README.md" || pname == "_cppm_test") continue;
                auto versions = *util::file_list("{0}/{1}/{2}"_format(root_path,rname,pname));
                for(auto& ver : versions) {
                    auto vname = ver.path().filename().string();
                    cppkg.repos[rname].pkgs[pname].versions[vname] = ver.path().string();
                }
            }
        }
        return cppkg;
    }

    std::string search(const std::string& name, const std::string& version) {
        using namespace fmt::literals;
        auto cppkg_path = "{0}/.cppm/repo"_format(getenv("HOME"));
        auto repos = util::file_list(cppkg_path);
        if(!repos) { fmt::print(stderr, "can't find cppkg repos"); exit(1);}

        std::map<std::string,std::string> find_repos;
        for(auto& repo : *repos){
            auto repo_name = repo.path().filename().string();
            auto target = "{0}/{1}/{2}/{3}"_format(cppkg_path,repo_name,name,version);
            if(fs::exists(target)) {
                find_repos[repo_name] = target;
            }
        }
        if(find_repos.empty()) {
            fmt::print(stderr, "can't find {0}/{1} package",name,version); 
            exit(1);
        }
        if(find_repos.find("local") != find_repos.end()) return find_repos["local"];
        if(find_repos.find("cppkg") != find_repos.end()) return find_repos["cppkg"];
        return find_repos.begin()->second;
    }

    void install(Config& config, const std::string& path) {
        using namespace fmt::literals;
        //auto value = [](auto str){ return "\"{0}\""_format(str);};
        Package package;
        package.parse(cpptoml::parse_file("{0}{1}/{2}"_format(path,package.version,"cppkg.toml")));
        fmt::print("{0}\n",package.cmake.find_lib);
        if(package.cmake.find_lib != "") {
            fmt::print("Install {0} to {1}/Modules/\n",package.cmake.find_lib,config.path.cmake);
            if(!fs::exists("{0}/Modules/"_format(config.path.cmake))) {
                fs::create_directories("{0}/Modules"_format(config.path.cmake));
            }
            fs::copy("{0}/{1}"_format(path,package.cmake.find_lib)
                    ,"{0}/Modules/{1}"_format(config.path.cmake,package.cmake.find_lib));
        }
        //fs::copy("{0}/{1}.cmake.in"_format(path,package.name)
        //        ,"{0}/{1}.cmake.in"_format(config.path.thirdparty,package.name));
        util::recursive_copy(path,"{0}/{1}/{2}"_format(
                             config.path.thirdparty,package.name,package.version));
        fmt::print("Install Cppkg {0}/{1}\n",package.name,package.version);
        //fmt::print("add this in cppm.toml\n");
        //fmt::print("[dependencies]\n");
        //fmt::print("{0}={{module={1}, version={2}, components={3}}}\n"
        //                                       ,package.name
        //                                       ,value(package.cmake.name)
        //                                       ,value(package.version)
        //                                       ,value(package.cmake.components));
    }
}
}
