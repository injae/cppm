#include<nieel/util/hash.hpp>
#include<nieel/string.hpp>
#include"config/thirdparty.h"
#include"cmake/generator.h"
#include"cmake/find_package.h"
#include"cppm.h"
#include"url.h"


namespace cppm 
{
    Thirdparty Thirdparty::parse(const YAML::Node& node, std::string name) {
        using namespace nieel;
        Thirdparty thirdparty; 
        thirdparty.name = name;
        for(auto property : node["thirdparty"][thirdparty.name]) {
            auto type = property.first.as<std::string>();
            auto data = property.second.as<std::string>();
			
            switch(hash(type.c_str())) 
            {
            case hash("build-type"):
                thirdparty.build_type = data;
                break;
            case hash("url"):
                thirdparty.repo       = Repository::classificate(data);
                break;
            case hash("version"):
                thirdparty.version    = data;
                break;
            case hash("desciption"):
                thirdparty.desciption = data;
                break;
            case hash("license"):
                thirdparty.license    = data;
                break;
            case hash("install"):
                thirdparty.install_type = data;
                break;
            case hash("find-cmake-value"):
                thirdparty.cmake_var_name = data;
                break;
            case hash("cmake-lib-name"):
                thirdparty.cmake_lib_name = data;
                break;
            default:
                std::cerr << "undefined property" << std::endl;
                exit(1);
            }
        }
        return thirdparty;
    }
    
    std::vector<Thirdparty> Thirdparty::parse_thirdparties(const YAML::Node& node) {
        std::vector<Thirdparty> thirdparties;
        for(auto name : node["thirdparty"]) {
            auto data = name.first.as<std::string>();
            thirdparties.push_back(Thirdparty::parse(node, data));
        }
        return thirdparties;
    }
        
    bool has_find_package(Thirdparty thirdparty) {
        for(auto& package : cmake::get_find_package_list()) {
            if(package.name == thirdparty.name) return true;
        }
        return false;
    }
    
    void make_thirdparty_config(Thirdparty& library) {
        auto project = Cppm::instance()->project();
        std::ofstream config_file(project.path.root + "/cppm.yaml"); config_file.is_open();
        
        auto config = YAML::LoadFile(project.path.root+"/cppm.yaml");
        config["thirdparty"][library.name]["build-type"]       = library.build_type;
        config["thirdparty"][library.name]["install"]          = library.install_type;
        config["thirdparty"][library.name]["version"]          = library.version;
        config["thirdparty"][library.name]["url"]              = library.repo.url;
        config["thirdparty"][library.name]["find-cmake-value"] = library.cmake_var_name;
        config["thirdparty"][library.name]["cmake-lib-name"]   = library.cmake_lib_name;
        
        config_file << config; 
        
        config_file.close();
    }
    
    void install_thirdparty(Thirdparty& library) {
        using namespace nieel;
        switch(hash(library.repo.type.c_str()))
        {
        case hash("git"): 
            git(library.repo.url);
            break;
        case hash("file"):
            tar(library.repo.url);
            break;
        case hash("link"):
            tar(wget(library.repo.url));
            break;
        }
        
    }
    
}