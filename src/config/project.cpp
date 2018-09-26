#include"config/project.h"
#include<nieel/util/hash.hpp>
#include<nieel/algorithm.hpp>
#include<nieel/filesystem.h>

namespace cppm
{
    
    Project Project::parse(YAML::Node& node) {
        Project project;
        auto root_path = nieel::reverse_find_file(fs::current_path(), "cppm.yaml")->parent_path().string();
        project.path = Path::make(root_path);
        Project::parse(node, project);
        return project;
    }
    
    Project& Project::parse(YAML::Node& node, Project& project) {
        using namespace nieel;
        auto root_path = nieel::reverse_find_file(fs::current_path(), "cppm.yaml")->parent_path().string();
        for(auto it : node) {
            auto property = it.first.as<std::string>().c_str();
            switch(hash(property))
            {
            case "package"_h:
                project.package = Package::parse(node);
                break;
            case "compiler"_h:
                project.compiler = Compiler::parse(node);
                break;
            case "builder"_h:
                project.builder  = Builder::parse(node);
                break;
            case "cmake"_h:
                project.cmake    = Cmake::parse(node);
            case "binary"_h:
                project.binaries = Binary::parse_binaries(node);
                break;
            case "library"_h:
                project.libraries = Library::parse_libraries(node);
                break;
            case "user-cmake-script"_h:
                std::cout << "t" << std::endl;
                for(auto script : node["user-cmake-script"]) {
                    project.user_cmake_scripts.push_back(script.as<std::string>());
                }
                break;
            case "include"_h:
            {
                for(auto include : node["include"]){
                    auto script = nieel::find_files(root_path, std::regex(include.as<std::string>()));
                    nieel::insert(project.include_script, script);
                }
                for(auto include : project.include_script) {
                    auto script = YAML::LoadFile(include.c_str());
                    Project::parse(script, project);
                }
                break;
            }
            case "thirdparty"_h:
                project.thirdparties = Thirdparty::parse_thirdparties(node);
                break;
            default:
                project.user_commands.emplace_back(Command::parse(node, property));
                break;
            }
        }
        return project;
    }
    
    bool Project::has(std::string name) {
        using trans_type = std::vector<std::string>;
        return nieel::has( nieel::transform<trans_type>(thirdparties, [](auto thirdparty){ return thirdparty.name; })
                          , name);
    }
    
    std::vector<Thirdparty> Project::has(std::vector<std::string>& names) {
        if(thirdparties.empty()) return std::vector<Thirdparty>();
        auto lib = thirdparties;
        return nieel::erase_if(lib, [&names](auto& thirdparty){ return !nieel::has(names, thirdparty.name);  });
    }
    
    std::vector<std::string> Project::cmake_lib_name(std::vector<std::string> libraries)  {
        using trans_type = std::vector<std::string>;
        return nieel::transform<trans_type>(has(libraries), [](auto thirdparty){ return thirdparty.cmake_lib_name; });
    }
    
    std::string Project::make_cmake_bin_lib_script() {
        using trans_type = std::vector<std::string>;
        auto lib = nieel::accumulate(nieel::transform<trans_type>(libraries, [](auto& library){ return library.cmake_script(); }), std::string{""});
        auto bin = nieel::accumulate(nieel::transform<trans_type>(binaries , [](auto& binary) { return binary.cmake_script(); }), std::string{""});
        return "\n" + lib + "\n" + bin;
    }
}
