#include"package/package.h"
#include"cppm.h"
#include<nieel/util/hash.hpp>
#include<nieel/string.hpp>
#include<fstream>
#include<iostream>

namespace cppm::package
{
    fs::path Package::find_package(std::string name) {
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
        return *file;
    }
    
    void Package::install_dependencies() {
        for(const auto& dependency : dependencies) {
            auto pkg = find_package(dependency);
            Package::parse(pkg).install_pkg();
        }
    }
     
    void Package::install_pkg() {
        install_dependencies();
        install.install();
        build.build();
        if(cmake.name != "") {
            auto module_path = Cppm::instance()->project().path.cmake_find_module;
            auto src = repo + "/cmake/" + cmake.find_library;
            auto dst = module_path + "/" + cmake.find_library;
            fs::copy(src, dst);
        }
    }
    
    Package Package::parse(fs::path& path) {
        using namespace nieel;
        auto node = YAML::LoadFile(path.c_str());
        Package package;
        package.repo = path.parent_path().parent_path().string();
        for(auto it : node["package"]) {
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
            case "dependencies"_h:
                for(auto dependency : node["dependencies"])
                    package.dependencies.push_back(dependency.as<std::string>());
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