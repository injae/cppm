#include "config/config.h"
#include "util/cmake.h"
#include "config/cppm_tool.h"
#include "package/package.h"
#include "util/filesystem.h"
#include <fmt/format.h>

namespace cppm
{
    Config Config::load(const std::string &path) {
        auto table = cpptoml::parse_file(path+"/cppm.toml");
        Config config;
        config.path = Path::make(path);
        config.parse(table);
        config.dependency_check();
        return config; 
    }

    void Config::dependency_check() {
        using namespace package;
        using namespace fmt::literals;
        std::vector<Dependency> not_installed_dep;
        for(auto [name, dep] : dependencies.list) {
            if(dep.hunter) { continue; }
            if(!fs::exists("{0}/{1}/{2}/{1}.cmake.in"_format(path.thirdparty,name,dep.version))) {
                not_installed_dep.push_back(dep);
            }
        }
        for(auto dep : not_installed_dep){
            auto path = cppkg::search(dep.name, dep.version);
            cppkg::install(*this, path);
        }
        for(auto& [name, dep] : dependencies.list) {
            if(dep.module == "" && !dep.hunter) {
                auto table = cpptoml::parse_file("{0}/{1}/{2}/cppkg.toml"_format(path.thirdparty,name,dep.version));
                package::Package pkg;
                pkg.parse(table);
                dep.module = pkg.cmake.name;
                dep.components = pkg.cmake.components;
            }
        }
    }

    void Config::parse(table_ptr table) {
        package.parse(table);
        cmake.parse(table);
        hunter.parse(table);
        builder.parse(table);
        bins.parse(table);
        libs.parse(table);
        //        test.parse(table);
        compiler.parse(table);
        dependencies.parse(table);
        cppm_config.load();
    }

    void Config::write(table_ptr table) {
        
    }
    
    std::string Config::generate() {
        using namespace fmt::literals;

        return "cmake_minimum_required(VERSION {0})\n"_format(cmake.version)
             + "\n"
             + "include(cmake/cppm_tool.cmake)\n"
             + "cppm_project()\n"
             + dependencies.use_hunter(hunter)
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
