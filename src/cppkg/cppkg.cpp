#include <fmt/format.h>
#include <cppm/util/filesystem.h>
#include <cppm/util/optional.hpp>
#include <cppm/cppkg/cppkg.h>
#include <cppm/core/cppm_tool.hpp>
#include <tomlpp/orm.hpp>
#include <range/v3/all.hpp>
#include <map>
#include <list>

namespace cppkg
{
    using namespace fmt::literals;
    cppm::core::Dependency parse(const std::string& name, std::string path) {
        using namespace cppm;
        path = path != "" ? "{}/"_format(path) : "";
        std::optional<std::map<std::string,core::Dependency>> dep;
        auto config = toml::orm::parser(dep,"{}cppkg.toml"_format(path));
        return (*dep)[name];
    }

    std::string translate(cppm::core::Dependency& dep) {
        using namespace cppm;
        std::string cmake;
        auto download = dep.git ? "GIT {} "_format(*dep.git) : "URL {} "_format(*dep.url) ;
        download += dep.branch ? "GIT_TAG {} "_format(*dep.branch) : "";
        cmake += "# Cppkg Base Dependency Downloader\n"
                 "# Other Options:\n"
                 "# - Linux Configures:\n"
                 "#    L_CONFIGURE {...}, L_BUILD {...}, L_INSTALL {...}\n"
                 "# - Windows Configures:\n"
                 "#    W_CONFIGURE {...}, W_BUILD {...}, W_INSTALL {...}\n"
                 "# - Install Path Options:\n"
                 "#    LOCAL(default) GLOBAL \n"
                 "cmake_minimum_required(VERSION 3.6)\n"  
              +  "project({0}-{1}-install C CXX)\n\n"_format(dep.name, *dep.version)
              +  "include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cppm_tool.cmake)\n" 
              +  "download_package({} {} {} CMAKE_ARGS ${{CMAKE_ARGS}} {})\n\n"_format( dep.name
                                                                                      , dep.version
                                                                                      , download
                                                                                      , dep.flags.value_or(""));
        return cmake;
    }

    void init(const std::string& name) {
        cppm::core::Dependency dep;
        dep.name = name;
        dep.type = "lib";
        init(dep);
    }

    void init(cppm::core::Dependency& dep) {
        using namespace cppm;
        auto value = [](const std::string& str){ return "\"{}\""_format(str); };
        //auto convert = [](const bool flag) { return flag ? "true" : "false"; };
        util::panic(!fs::exists("cppkg.toml"), "existed cppkg.toml");
        auto url_type = dep.git ? "git" : "url";
        auto is_branch = dep.branch ? "" : "#";
        util::create("cppkg.toml");
        util::write("cppkg.toml"
                    ,"[{}]\n"_format(dep.name)
                    +"version={} #(require)\n"_format(value(*dep.version))
                    +"type={} #lib(default) | bin | cmake\n"_format(value(*dep.type))
                    +"description={} #(require)\n"_format(value(*dep.description))
                    +"module={} #(require) if none_module=true -> no require\n"_format(value(*dep.module))
                    +"{}={} #(require)\n"_format(url_type,value(*dep.url))
                    +"{}branch={} #(optional & require git)\n"_format(is_branch, value(*dep.branch))
                    +"#link={} #default\n"_format(value("public"))
                    //+"#components={} #(optional)\n"_format(value(*dep.components))
                    //+"#none_module={} #(optional)\n"_format(convert(*dep.none_module))
                    //+"#helper={} #(optional)\n"_format(value(dep.helper))
                    //+"#load_path={} #(optional) (package_root) + /path/to/"_format(value(dep.load_path))
                    );
    }

    void build(const std::string& name) {
        using namespace cppm;
        auto dep = cppkg::parse(name);
        auto dir_name = "{}/{}"_format(dep.name, dep.version);
        util::panic(!fs::exists(dir_name), "existed {}"_format(dir_name));
        fs::create_directories(dir_name);
        fs::copy("cppkg.toml", dir_name+"/cppkg.toml");
        auto file = "{}/{}.cmake.in"_format(dir_name,dep.name);
        util::write(file, core::cppm_download_package(dep));
        fmt::print("[cppkg] Success to make {} package\n",dep.name);
    }
    void build() {
        using namespace cppm;
        std::optional<std::list<core::Dependency>> deps;
        toml::orm::parser(deps,"cppkg.toml");
        if(deps) {
            ranges::for_each(*deps, [](auto it){
                
            });
        }
        //auto file = "{}/{}.cmake.in"_format(dir_name,dep.name);
        //util::write(file, core::cppm_download_package(dep));
        //fmt::print("[cppkg] Success to make {} package\n",dep.name);
    }

    void regist(const std::string& name) {
        using namespace cppm;
        core::Dependency dep;
        cppkg::parse(name);
        auto local_path = "{0}repo/local"_format(core::cppm_root());
        auto pack_path = "{0}/{1}/{2}"_format(local_path, dep.name, *dep.version);
        if(!fs::exists(local_path)) fs::create_directories(pack_path);
        if(fs::exists(pack_path)) fs::remove_all(pack_path);
        util::recursive_copy(name,pack_path);
        fmt::print("update {0}/{1} in local-repo [\"{2}\"]\n", dep.name, *dep.version, pack_path);
    }


    Repos list() {
        using namespace cppm;
        auto root_path = "{0}repo"_format(core::cppm_root());
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
        auto cppkg_path = "{0}repo"_format(core::cppm_root());
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
        util::panic(!find_repos.empty(), "can't find {0}/{1} package"_format(name, version));
        std::string path = "";

        if (find_repos.find("cppkg") != find_repos.end()) path = find_repos["cppkg"];
        else if (find_repos.find("local") != find_repos.end()) path = find_repos["local"];
        else path = find_repos.begin()->second;
        auto dep = cppkg::parse(name,path);
        return {dep, path};
    }

    void install(cppm::core::Config& config, const dep_in_repo& depr) {
        using namespace cppm;
        fs::create_directory(config.path.thirdparty);
        auto [dep, path] = depr;
        if(dep.helper) {
            fmt::print("{0}\n",*dep.helper);
            fmt::print("Install {0} to {1}/Modules/\n",dep.helper, config.path.cmake);
            if(!fs::exists("{0}/Modules/"_format(config.path.cmake))) {
                fs::create_directories("{0}/Modules"_format(config.path.cmake));
            }
            fs::copy("{0}/{1}"_format(path,dep.helper)
                     ,"{0}/Modules/{1}"_format(config.path.cmake,dep.helper));
        }
        util::recursive_copy(path,"{0}/{1}/{2}"_format(
                             config.path.thirdparty, dep.name, dep.version));
        fmt::print("Install Cppkg {0}/{1}\n",dep.name, dep.version);
    }
}
