#include"config/cppm-package/package.h"
#include"cppm.h"
#include<nieel/util/hash.hpp>
#include<nieel/string.hpp>
#include<nieel/filesystem.h>
#include<fstream>
#include<iostream>

namespace cppm::package
{
    Package Package::find_package(std::string name) {
        auto repos = Cppm::instance()->project().package.thirdparty_repo;
        std::string system_repo_path = "/usr/local/share/cppm/repos";
        for(auto repo : repos) {
            std::string cmd = "   cd "+ system_repo_path;
                            + "&& git clone " + repo;
        }
        auto file_name = name + ".yaml";
        auto file = nieel::find_file(system_repo_path, file_name);
        if(!file) {
            std::cout << "can't find " + name + " package" <<std::endl;
            exit(1);
        }
        std::cout << "found cppm package " << name << std::endl;
        auto node = YAML::LoadFile(file->c_str());
        return parse(node);
    }
    
    void Package::install_package(Package&& package) {
        auto module_path = Cppm::instance()->project().path.cmake_find_module;
        std::ofstream file(module_path + "/Find" + nieel::upper_first_letter(package.name) + ".cmake");
        if(!file.is_open()) { std::cerr << "can not open file" << std::endl; return; }
        for(auto& script : package.cmake.find_library) {
            file << script << "\n";
        }
        file.close();
    }
    
    Package Package::parse(YAML::Node& node) {
        using namespace nieel;
        Package package;
        for(auto it : node) {
            auto property = it.first.as<std::string>();
            switch(hash(property))
            {
            case "name"_h:
                package.name = it.second.as<std::string>();
                break;
            case "version"_h:
                package.version = it.second.as<std::string>();
                break;
            case "lisence"_h:
                package.lisence = it.second.as<std::string>();
                break;
            case "authors"_h:
                package.authors = it.second.as<std::string>();
                break;
            case "description"_h:
                package.description = it.second.as<std::string>();
                break;
            case "build"_h:
                package.build = Build::parse(node);
                break;
            case "cmake"_h:
                package.cmake = Cmake::parse(node);
                break;
            case "install-script"_h:
                package.install = Install::parse(node);
                break;
            default:
                std::cerr << "wrong option: " << property << std::endl;
                exit(1);
            }
        }
        return package;
    }
}