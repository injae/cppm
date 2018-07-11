#include"config/project.h"
#include<nieel/util/hash.hpp>
#include<nieel/algorithm.hpp>
#include<nieel/filesystem.h>

namespace cppm
{
    
    Project Project::parse(YAML::Node& node) {
        using namespace nieel;
        Project project;
        auto config_file_path = nieel::reverse_find_file(fs::current_path(), "cppm.yaml")->parent_path().string();
        project.path = Path::make(config_file_path);
        for(auto it : node) {
            auto property = it.first.as<std::string>().c_str();
            switch(hash(property))
            {
            case hash("package"):
                project.package = Package::parse(node);
                break;
            case hash("compiler"):
                project.compiler = Compiler::parse(node);
                break;
            case hash("builder"):
                project.builder  = Builder::parse(node);
                break;
            case hash("binary"):
                project.binaries = Binary::parse_binaries(node);
                break;
            case hash("library"):
                project.libraries = Library::parse_libraries(node);
                break;
            case hash("thirdparty"):
                project.thirdparties = Thirdparty::parse_thirdparties(node);
                break;
            case hash("user-cmake-script"):
                project.parse_user_cmake_script(node);
                break;
            default:
                project.user_commands.emplace_back(Command::parse(node, property));
                break;
            }
        }
        return project;
    }
    
    void Project::parse_user_cmake_script(YAML::Node& node) {
        if(!node["user-cmake-script"]) return; 
        for(auto script : node["user-cmake-script"]) {
            user_cmake_scripts.push_back(script.as<std::string>());
        }
    }
    bool Project::has(std::string name) {
        using trans_type = std::vector<std::string>;
        return nieel::has( nieel::transform<trans_type>(thirdparties, [](auto thirdparty){ return thirdparty.name; })
                          , name);
    }
    
    std::vector<Thirdparty> Project::has(std::vector<std::string>& names) {
        auto lib = thirdparties;
        return nieel::erase_if(lib, [this, &names](auto& thirdparty){ return !nieel::has(names, thirdparty.name);  });
    }
    
    std::vector<std::string> Project::cmake_lib_name(std::vector<std::string> libraries)  {
        using trans_type = std::vector<std::string>;
        return nieel::transform<trans_type>(has(libraries), [](auto thirdparty){ return thirdparty.cmake_lib_name; });
    }
    
    std::string Project::make_cmake_bin_lib_script() {
        using trans_type = std::vector<std::string>;
        auto lib = nieel::accumulate(nieel::transform<trans_type>(libraries, [](auto& library){ return library.cmake_script(); }), std::string{""});
        auto bin = nieel::accumulate(nieel::transform<trans_type>(binaries,[](auto& library){ return library.cmake_script(); }), std::string{""});
        return lib + "\n" + bin;
    }
}