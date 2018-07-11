#include"config/binary.h"
#include"cmake/generator.h"
#include<nieel/string.hpp>
#include<nieel/util/hash.hpp>

namespace cppm
{
    Binary Binary::parse(YAML::Node& node, std::string name) {
        using namespace nieel;
        
        Binary binary;
        binary.name = name;
        for(auto it : node["binary"][binary.name]) {
            auto property = it.first.as<std::string>().c_str(); 
            switch(hash(property))
            {
            case hash("version"):
                binary.version = it.second.as<std::string>();
                break;
            case hash("include"):
                for(auto include : node["binary"][name]["include"]){
                    binary.include.push_back(include.as<std::string>());
                }
                break;
            case hash("source"):
                for(auto source : node["binary"][name]["source"]){
                    binary.source.push_back(source.as<std::string>());
                }
                break;
            case hash("dependencies"):
                for(auto dependency : node["binary"][name]["dependencies"]){
                    binary.dependencies.push_back(dependency.as<std::string>());
                }
                break;
            }
        }
        return binary;
    }
    
    
    std::vector<Binary> Binary::parse_binaries(YAML::Node& node) {
        std::vector<Binary> binaries;
        for(auto it : node["binary"]) {
            binaries.push_back(Binary::parse(node, it.first.as<std::string>()));
        }
        return binaries;
    }
    
    
    std::string Binary::cmake_script() {
        using namespace cmake;
        auto project = Cppm::instance()->project();
        if(project.binaries.empty()) return "";
        std::stringstream output("");
        output << "cpp_file_list(src/)"  << endl()
               << set("thirdparty", nieel::accumulate(project.cmake_lib_name(dependencies), std::string{" "})) << endl()
               << "build_binary(" << name << " \"" << get("source") << "\" \"" << get("thirdparty") << "\")" << endl()
               ;
        return output.str();
    }
}