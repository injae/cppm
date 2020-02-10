#include "config/config.h"
#include "cmake/cmake.h"
#include "config/cppm_tool.h"
#include "package/package.h"
#include "util/version.h"
#include "util/filesystem.h"
#include <iostream>

namespace cppm
{
    Config Config::load(const std::string &path) {
        auto table = cpptoml::parse_file(path+"/cppm.toml");
        auto lock = cpptoml::make_table();
        Config config;
        config.path = Path::make(path);
        config.parse(table);
        //config.build_lock(table, lock);
        config.dependency_check();
        return config; 
    }

    void Config::dependency_check() {
        using namespace package;
        std::vector<Dependency> not_installed_dep;
        fs::create_directories(path.thirdparty);
        for(auto [name, dep] : dependencies.list) {
            if(dep.hunter) { continue; }
            std::string tpath = "";
            if(dep.load_path.empty()) {
                if(!fs::exists("{}/{}"_format(path.root, dep.load_path))) {
                    fmt::print(stderr, "can't find load-path library, {}", dep.load_path);
                    exit(1);
                }
                continue;
            }
            if(!fs::exists("{0}/{1}"_format(path.thirdparty,name))){
                not_installed_dep.push_back(dep);
                continue;
            }
            if(dep.version == "latest") {
                auto vpath = Version::get_latest_version_folder("{0}/{1}"_format(path.thirdparty,name));
                if(!vpath) { fmt::print(stderr, "can't find {}/{}",name, dep.version); exit(1); }
                tpath = *vpath;
            }
            else { tpath = "{0}/{1}/{2}"_format(path.thirdparty,name,dep.version); }
            if(!fs::exists("{0}/{1}.cmake.in"_format(tpath,name))) {
                not_installed_dep.push_back(dep);
            }
        }
        for(auto dep : not_installed_dep){
            auto path = cppkg::search(dep.name, dep.version);
            cppkg::install(*this, path);
        }
        for(auto& [name, dep] : dependencies.list) {
            if(dep.module == "" && !dep.hunter) {
                std::string tpath = "";
                if(dep.version == "latest") {
                    auto vpath = Version::get_latest_version_folder("{0}/{1}"_format(path.thirdparty,name));
                    if(!vpath) { fmt::print(stderr, "can't find {}/{}",name, dep.version); exit(1); }
                    tpath = *vpath;
                }
                else {
                    tpath = "{0}/{1}/{2}"_format(path.thirdparty,name,dep.version);
                    if(!fs::exists(tpath)) { fmt::print(stderr, "can't find {}/{}",name, dep.version); exit(1); }
                }
                auto table = cpptoml::parse_file("{}/cppkg.toml"_format(tpath));
                package::Package pkg;
                pkg.parse(table);
                dep.module     = pkg.cmake.name;
                dep.type       = pkg.type;
                dep.version    = pkg.version;
                dep.components = pkg.cmake.components;
            }
        }
    }

    void Config::parse(table_ptr table) {
        package.parse(table);
        cmake.parse(table);
        hunter.parse(table);
        builder.parse(table);
        workspace.parse(table);
        bins.parse(table);
        libs.parse(table);
        //test.parse(table);
        compiler.parse(table);
        dependencies.parse(table);
        //std::cout << (*table) << std::endl;
        cppm_config.load();
    }

    void Config::build_lock(table_ptr table, table_ptr lock) {
        lock->insert("target", cpptoml::make_inner_table("default.platform.default"s));

        package.build_lock(table, lock);
        cmake.build_lock(table, lock);
        hunter.build_lock(table, lock);
        bins.build_lock(table, lock);
        bins.build_lock(table, lock);
        

        std::fstream file("{}/cppm.lock"_format(path.root), std::ios::out);
        file << (*lock);
        file.close();
    }

    void Config::write(table_ptr table) {
        
    }
    
    std::string Config::generate() {
        using namespace fmt::literals;

        return "cmake_minimum_required(VERSION {0})\n"_format(cmake.version)
             + "\n"
             + "include(cmake/cppm_tool.cmake)\n"
             + "cppm_project()\n"
             + dependencies.use_hunter(*this)
             + "project({0} VERSION {1} LANGUAGES C CXX)\n"_format(package.name, package.version)
             + "cppm_setting()\n"
             + "\n"
             + "cppm_cxx_standard({})\n"_format(package.standard)
             + compiler.generate()
             + "\n"
             + cmake.generate()
             + dependencies.generate()
             + tool::target_define(*this)
             + tool::target_dependencies(*this)
             + tool::target_install(*this)
             + "\n"
             ;
    }
}
