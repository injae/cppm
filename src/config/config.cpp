#include "config/config.h"
#include "cmake/cmake.h"
#include "config/cppm_tool.h"
#include "util/filesystem.h"
#include "util/optional.hpp"
#include <iostream>

namespace cppm
{
    Config::ptr Config::load(const std::string &path) {
        auto table = cpptoml::parse_file(path+"/cppm.toml");
        auto config = std::shared_ptr<Config>(new Config);
        config->lock = cpptoml::make_table();
        config->path = Path::make(path);
        config->parse(table);
        //config.build_lock(table, lock);
        config->after_init(config);
        return config; 
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


    void Config::after_init(Config::ptr config) {
        config->dependencies.after_init(config);
        
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
             + dependencies.use_hunter(shared_from_this())
             + "project({0} VERSION {1} LANGUAGES C CXX)\n"_format(package.name, package.version)
             + "cppm_setting()\n"
             + "\n"
             + "cppm_cxx_standard({})\n"_format(package.standard)
             + compiler.generate()
             + "\n"
             + cmake.generate()
             + dependencies.generate()
             + tool::target_define(shared_from_this())
             + tool::target_dependencies(shared_from_this())
             + tool::target_install(shared_from_this())
             + "\n"
             ;
    }
}
