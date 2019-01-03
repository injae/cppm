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
            header  = package->get_as<std::string>("header").value_or("");
            footer  = package->get_as<std::string>("footer").value_or("");

            auto cmake_      = package->get_table("cmake");
            cmake.name       = cmake_->get_as<std::string>("name").value_or("");
            cmake.option     = cmake_->get_as<std::string>("option").value_or("");
            cmake.components = cmake_->get_as<std::string>("components").value_or("");
            cmake.find_lib   = cmake_->get_as<std::string>("findlib").value_or("");

            auto download_   = package->get_table("download");
            download.url     = download_->get_as<std::string>("url").value_or("");
            download.git.url = download_->get_as<std::string>("git").value_or("");
            download.git.tag = download_->get_as<std::string>("git_tag").value_or("");
        }
    }

    std::string Package::generate() {
        using namespace fmt::literals;
        auto is_default = [&](std::string str) { return str != "" ? "" : "#"; };
        auto version_ = version == "lastest" ? "" : version;
        return "cmake_minimum_required(VERSION 3.10)\n"
             + "project({0}-{1}-install NONE)\n\n"_format(name, version_)
             + "#==========================================\n"
             + "#Header\n"
             + "#==========================================\n"
             + "find_package({0} {1} QUIET)\n"_format(name, version_)
             + "if(NOT {0}_FOUND AND NOT {0}_FIND_VERSION_EXACT)\n"_format(name)
             + "    include(ExternalProject)\n"
             + "    if(NOT WIN32)\n"
             + " # Linux or OSX Setting\n"
             + "        ExternalProject_Add(\n"
             + "        {0}\n"_format(name)
             + "        {0}URL {1}\n"_format(is_default(download.url), download.url)
             + "        {0}GIT_REPOSITORY {1}\n"_format(is_default(download.git.url),download.git.url)
             + "        {0}GIT_TAG {1}\n"_format(is_default(download.git.tag),download.git.tag)
             + "        SOURCE_DIR ${{CMAKE_BINARY_DIR}}/repo/{0}\n"_format(name)
           //+ "        BINARY_DIR ${{CMAKE_BINARY_DIR}}/repo/{0}/build\n"_format(name)
             + "        CMAKE_ARGS \"{0} {1}\"\n"_format("-DCMAKE_INSTALL_PREFIX=$ENV{HOME}/.cppm/local", cmake.option)
             + "        #CONFIGURE_COMMAND \n"
             + "        #BUILD_COMMAND \n"
             + "        #INSTALL_COMMAND \n"
             + "        BUILD_IN_SOURCE true\n"
             + "        )\n"
             + "    else(NOT WIN32)\n"
             + " # Windows Setting\n"
             + "        ExternalProject_Add(\n"
             + "        {0}\n"_format(name)
             + "        {0}URL {1}\n"_format(is_default(download.url), download.url)
             + "        {0}GIT_REPOSITORY {1}\n"_format(is_default(download.git.url),download.git.url)
             + "        {0}GIT_TAG {1}\n"_format(is_default(download.git.tag),download.git.tag)
             + "        SOURCE_DIR ${{CMAKE_BINARY_DIR}}/repo/{0}\n"_format(name)
           //+ "        BINARY_DIR ${{CMAKE_BINARY_DIR}}/repo/{0}/build\n"_format(name)
             + "        CMAKE_ARGS \"{0} {1}\"\n"_format("-DCMAKE_INSTALL_PREFIX=$ENV{HOME}/.cppm/local", cmake.option)
             + "        #CONFIGURE_COMMAND \n"
             + "        #BUILD_COMMAND \n"
             + "        #INSTALL_COMMAND \n"
             + "        BUILD_IN_SOURCE true\n"
             + "        )\n"
             + "    endif(NOT WIN32)\n"
             + "endif()\n"
             + "#==========================================\n"
             + "#Footer\n"
             + "#==========================================\n\n"
             ;
    }
namespace cppkg {
    void init(const std::string& name) {
        using namespace fmt::literals;
        package::Package package;
        package.name = name;
        package.version = "lastest";
        auto value = [](const std::string& str) { return "\""+ str +"\""; };
        if(fs::exists(package.name + ".toml")) {
            fmt::print(stderr, "existed {0}", package.name);
            exit(1);
        }
        util::create("{0}.toml"_format(package.name));
        util::write( "{0}.toml"_format(package.name)
                   , "[package]\n"
                   + "name = {0}\n"_format(value(package.name))
                   + "# if you use git repo, package version is lastest\n"
                   + "version = {0}\n"_format(value(package.version))
                   + "# library name in cmake, cppm.toml use this value \n"
                   + "# [dependencies]\n"
                   + "# ${library_name} = {module =${value}}\n"
                   + "cmake = {{name = {0}}}\n"_format(value(package.cmake.name))
                   + "# write download value git or url\n"
                   + "#download(git={0})\n"_format(value(""))
                   + "#download(url={0})\n"_format(value(""))
                   + "find_cmake = {0}\n"_format(value(package.find_cmake))
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
        auto file = "{0}/{1}.cmake.in"_format(dir_name,package.name);
        util::write(file, package.generate());
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
        if(package.find_cmake != "") {
            fs::copy("{0}{1}"_format(path,package.find_cmake)
                    ,"{0}/Modules/{1}"_format(config.path.cmake,package.find_cmake));
        }
        fs::copy("{0}/{1}.cmake.in"_format(path,package.name)
                ,"{0}/{1}.cmake.in"_format(config.path.thirdparty,package.name));
        fmt::print("Add Dependency {0}/{1}\n",package.name,package.version);
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
