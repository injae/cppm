#include"cppkg/cppkg.h"
#include<cpptoml.h>
#include<fmt/format.h>
#include<util/filesystem.h>
#include<util/optional.hpp>
#include "config/cppm_tool.h"

namespace cppkg
{
    using namespace fmt::literals;
    cppm::Dependency parse(const std::string& name, std::string path) {
        using namespace cppm;
        path = path != "" ? "{}/"_format(path) : "";
        auto config = cpptoml::parse_file("{}cppkg.toml"_format(path));
        cppm::Dependency dep;
        dep.name = name;
        auto ptable = util::panic(config->get_table(name), "need [{}] setting\n");
        dep.parse(ptable);
        
        return dep;
    }

    std::string translate(cppm::Dependency& dep) {
        using namespace cppm;
        util::panic(dep.module == "" && dep.type == "lib","require module={cmake module name}\n");
        util::panic(dep.desc   == "","require description={packag description}\n");
        util::panic(dep.download.url == "","require url={download url or git}\n");
        std::string cmake;
        auto download = dep.download.is_git ? "GIT {} "_format(dep.download.url) : "URL {} "_format(dep.download.url) ;
        download += dep.download.branch != "" ? "GIT_TAG {} "_format(dep.download.branch) : "";
        cmake += "# Cppkg Base Dependency Downloader\n";
        cmake += "# Other Options:\n";
        cmake += "# - Linux Configures:\n";
        cmake += "#    L_CONFIGURE {...}, L_BUILD {...}, L_INSTALL {...}\n";
        cmake += "# - Windows Configures:\n";
        cmake += "#    W_CONFIGURE {...}, W_BUILD {...}, W_INSTALL {...}\n";
        cmake += "# - Install Path Options:\n";
        cmake += "#    LOCAL(default) GLOBAL \n";
        cmake += "cmake_minimum_required(VERSION 3.6)\n"  
              +  "project({0}-{1}-install C CXX)\n\n"_format(dep.name, dep.version)
              +  "include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cppm_tool.cmake)\n" 
              +  "download_package({} {} {} CMAKE_ARGS ${{CMAKE_ARGS}} {})\n\n"_format( dep.name
                                                                                            , dep.version
                                                                                            , download
                                                                                            , dep.flags);
        return cmake;
    }

    void init(const std::string& name) {
        cppm::Dependency dep;
        dep.name = name;
        init(dep);
    }

    void init(cppm::Dependency& dep) {
        using namespace cppm;
        auto value = [](const std::string& str){ return "\"{}\""_format(str); };
        auto convert = [](const bool flag) { return flag ? "true" : "false"; };
        util::panic(fs::exists("cppkg.toml"), "existed cppkg.toml");
        auto url_type = dep.download.is_git ? "git" : "url";
        auto is_branch = dep.download.branch != "" ? "" : "#";
        util::create("cppkg.toml");
        util::write("cppkg.toml"
                    ,"[{}]\n"_format(dep.name)
                    +"version={} #(require)\n"_format(value(dep.version))
                    +"type={} #lib(default) | bin | cmake\n"_format(value(dep.type))
                    +"description={} #(require)\n"_format(value(dep.desc))
                    +"module={} #(require) if none_module=true -> no require\n"_format(value(dep.module))
                    +"{}={} #(require)\n"_format(url_type,value(dep.download.url))
                    +"{}branch={} #(optional & require git)\n"_format(is_branch, value(dep.download.branch))
                    +"#link_type={} #default\n"_format(value("public"))
                    +"#components={} #(optional)\n"_format(value(dep.components))
                    +"#hunter={} #(optional)\n"_format(convert(dep.hunter))
                    +"#none_module={} #(optional)\n"_format(convert(dep.none_module))
                    +"#helper={} #(optional)\n"_format(value(dep.helper))
                    //+"#load_path={} #(optional) (package_root) + /path/to/"_format(value(dep.load_path))
                    );
    }

    void build(const std::string& name) {
        using namespace cppm;
        auto dep = cppkg::parse(name);
        auto dir_name = "{}/{}"_format(dep.name, dep.version);
        util::panic(fs::exists(dir_name), "existed {}"_format(dir_name));
        fs::create_directories(dir_name);
        auto file = "{}/{}.cmake.in"_format(dir_name,dep.name);
        util::write(file, translate(dep));
        fmt::print("[cppkg] Success to make {} package\n",dep.name);
    }

    void regist(const std::string& name) {
        cppm::Dependency dep;
        cppkg::parse(name);
        auto local_path = "{0}repo/local"_format(cppm::tool::cppm_root());
        auto pack_path = "{0}/{1}/{2}"_format(local_path, dep.name, dep.version);
        if(!fs::exists(local_path)) fs::create_directories(pack_path);
        if(fs::exists(pack_path)) fs::remove_all(pack_path);
        cppm::util::recursive_copy(name,pack_path);
        fmt::print("update {0}/{1} in local-repo [\"{2}\"]\n", dep.name, dep.version, pack_path);
    }


    Repos list() {
        using namespace cppm;
        auto root_path = "{0}repo"_format(tool::cppm_root());
        Repos cppkg_repos;
        if(auto repos = util::file_list(root_path)) {
            for(auto repo : *repos) {
                auto rname = repo.path().filename().generic_string();
                if(auto pkgs = util::file_list("{0}/{1}"_format(root_path,rname))) {
                    for(auto& pkg : *pkgs) {
                        auto pname = pkg.path().filename().generic_string();
                        if(pname == ".git" || pname == "README.md" || pname == "_cppm_test") continue;
                        if(auto versions = util::file_list("{0}/{1}/{2}"_format(root_path,rname,pname))) {
                            for(auto& ver : *versions) {
                                auto vname = ver.path().filename().generic_string();
                                cppkg_repos[rname].pkgs[pname].versions[vname] = "{0}/{1}/{2}/{3}"_format(root_path, rname, pname, vname);
                            }
                        }
                    }
                }
            }
        }
        return cppkg_repos;
    }

    dep_in_repo search(const std::string& name, const std::string& version) {
        using namespace cppm;
        auto cppkg_path = "{0}repo"_format(tool::cppm_root());
        auto repos = util::panic(util::file_list(cppkg_path), "can't find cppkg repos");

        std::map<std::string,std::string> find_repos;
        for(auto& repo : repos) {
            auto repo_name = repo.path().filename().string();
            if(version == "latest") {
                auto target = "{0}/{1}/{2}"_format(cppkg_path,repo_name,name);
                if(!fs::exists(target)) { continue; }
                if(auto latest = Version::get_latest_version_folder(target)) {
                    find_repos[repo_name] = *latest;
                }
            }
            else {
                auto target = "{0}/{1}/{2}/{3}"_format(cppkg_path, repo_name, name, version);
                if(fs::exists(target)) { find_repos[repo_name] = target; }
            }
        }
        util::panic(find_repos.empty(), "can't find {0}/{1} package"_format(name, version));
        std::string path = "";
        if (find_repos.find("cppkg") != find_repos.end()) path = find_repos["cppkg"];
        else if (find_repos.find("local") != find_repos.end()) path = find_repos["local"];
        else path = find_repos.begin()->second;
        auto dep = cppkg::parse(name,path);
        return {dep, path};
    }

    void install(cppm::Config::ptr config, const dep_in_repo& depr) {
        using namespace cppm;
        auto [dep, path] = depr;
        if(dep.helper != "") {
            fmt::print("{0}\n",dep.helper);
            fmt::print("Install {0} to {1}/Modules/\n",dep.helper, config->path.cmake);
            if(!fs::exists("{0}/Modules/"_format(config->path.cmake))) {
                fs::create_directories("{0}/Modules"_format(config->path.cmake));
            }
            fs::copy("{0}/{1}"_format(path,dep.helper)
                     ,"{0}/Modules/{1}"_format(config->path.cmake,dep.helper));
        }
        util::recursive_copy(path,"{0}/{1}/{2}"_format(
                             config->path.thirdparty, dep.name, dep.version));
        fmt::print("Install Cppkg {0}/{1}\n",dep.name, dep.version);
    }
}
