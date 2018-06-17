#include"cppm.h"
#include<nieel/filesystem.h>
#include<nieel/util/hash.hpp>
#include<fstream>

Cppm* Cppm::instance() {
    static Cppm instance_; 
    return &instance_;  
} 

Cppm::Cppm() : option_() { }

void Cppm::make_project_property() {
    project_.bin               = project_.path + "/bin";
    project_.source            = project_.path + "/source";
    project_.include           = project_.path + "/include";
    project_.thirdparty        = project_.path + "/thirdparty";
    project_.cmake_module      = project_.path + "/cmake";
    project_.cmake_find_module = project_.cmake_module + "/Modules";
}

fs::path Cppm::find_config_file() {
    auto config_file = nieel::find_file(fs::current_path(), "cppm.yaml");
    if(!config_file) {
        std::cerr << "Can't not find cppm.yaml file\n" 
                  << std::endl;
        exit(1);
    }
    
    return *config_file;
}

void Cppm::make_config_file(Project& project) {
    std::ofstream config_file(project.path + "/cppm.yaml"); config_file.is_open();
    
    auto config = YAML::LoadFile(project.path+"/cppm.yaml");
    config["project"]["name"]     = project.name;
    config["project"]["version"]  = project.version;
    config["project"]["type"]     = project.type;
    config["project"]["compiler"] = project.compiler;
    config["project"]["builder"]  = project.builder;
    
    config_file << config; 
    
    config_file.close();
}


void Cppm::parse_thirdparty(YAML::Node& node) {
    using namespace nieel::util;
    
    for(auto name : node["project"]["thirdparty"]) {
        cppm::Thirdparty thirdparty; 
        thirdparty.name = name.first.as<std::string>();
        for(auto property : node["project"]["thirdparty"][thirdparty.name]) {
            auto node = property.first.as<std::string>();
            auto data = property.second.as<std::string>();
			
            switch(hash(node.c_str())) 
            {
            case hash("build-type"):
                thirdparty.build_type = data;
                break;
            case hash("url"):
                thirdparty.repo       = cppm::classificate_repo(data);
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
            default:
                std::cerr << "undefined property" << std::endl;
                exit(1);
            }
        }
        thirdparties_.emplace_back(std::move(thirdparty));
    }
}

void Cppm::parse_project_config() {
    using namespace nieel::util;
    auto configure_file = YAML::LoadFile(find_config_file().c_str());
    project_.path = find_config_file().parent_path().string();
    for(auto it : configure_file["project"]) {
        auto node = it.first.as<std::string>().c_str();
        switch(hash(node))
        {
        case hash("name"):
            project_.name = it.second.as<std::string>(); 
            break;
        case hash("thirdparty"):
            parse_thirdparty(configure_file);
            break;
        case hash("version"):
            project_.version = it.second.as<std::string>();
            break;
        case hash("compiler"):
            project_.compiler = it.second.as<std::string>();
            break;
        case hash("compiler-option"):
            project_.compiler_option = it.second.as<std::string>();
            break;
        case hash("user-cmake-script"):
            project_.user_cmake_script = user_cmake_script_parser(configure_file);
            break;
        case hash("type"):
            project_.type = it.second.as<std::string>();
            break;
        case hash("builder"):
            project_.builder = it.second.as<std::string>();
            break;
        case hash("cpu-core"):
            project_.cpu_core = it.second.as<std::string>();
            break;
        default:
            option_->registe_subcommand(std::make_pair(it.first.as<std::string>(), it.second.as<std::string>())); 
        }
    }
    make_project_property();
}

std::vector<std::string> Cppm::user_cmake_script_parser(YAML::Node& node) {
    std::vector<std::string> cmake_scripts;
    for(auto script : node["project"]["user-cmake-script"]) {
        cmake_scripts.push_back(script.as<std::string>());
    }
    
    return cmake_scripts;
}

void Cppm::run(int argc, char** argv) {
    option_ = std::make_unique<CppmOptions>(argc, argv);
    option_->regist(); 
    option_->run();
}
