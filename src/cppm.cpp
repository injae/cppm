#include"cppm.h"
#include"cppm_options.h"
#include"utils.h"

#include<yaml-cpp/yaml.h>

Cppm* Cppm::instance() {
    static Cppm instance_; 
    return &instance_;  
} 

Cppm::Cppm() { }

void Cppm::make_project_property() {
    project_.bin               = project_.path + "/bin";
    project_.source            = project_.path + "/source";
    project_.include           = project_.path + "/include";
    project_.thirdparty        = project_.path + "/thirdparty";
    project_.cmake_module      = project_.path + "/cmake";
    project_.cmake_find_module = project_.cmake_module + "/Modules";
}

fs::path Cppm::find_config_file() {
    auto config_file = find_file(fs::current_path(), "cppm.yaml");
    if(!config_file) {
        std::cerr << "Can't not find cppm.yaml file\n" 
                  << std::endl;
        exit(1);
    }
    
    return *config_file;
}

void Cppm::run(int argc, char** argv) {
    CppmOptions option(argc, argv);
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
            break;
        case hash("version"):
            project_.version = it.second.as<std::string>();
            break;
        default:
            option.registe_subcommand(std::make_pair(it.first.as<std::string>(), it.second.as<std::string>())); 
        }
    }
    make_project_property();
    
    option.regist(); 
    option.run();
}
