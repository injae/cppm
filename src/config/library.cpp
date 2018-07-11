#include"config/library.h"
#include"cmake/generator.h"
#include<nieel/util/hash.hpp>
#include<nieel/string.hpp>
#include<iostream>
#include<sstream>

namespace cppm
{
    Library Library::parse(YAML::Node& node, std::string name) {
        using namespace nieel;
        Library library;
        library.name = name;
        for(auto it : node["library"][name]) {
            auto property = it.first.as<std::string>().c_str(); 
            switch(hash(property))
            {
            case hash("type"):
                library.type = it.second.as<std::string>();
                break;
            case hash("version"):
                library.version = it.second.as<std::string>();
                break;
            case hash("include"):
                for(auto include : node["library"][name]["include"]){
                    library.include.push_back(include.as<std::string>());
                }
                break;
            case hash("source"):
                for(auto source : node["library"][name]["source"]){
                    library.source.push_back(source.as<std::string>());
                }
                break;
            case hash("dependencies"):
                for(auto dependency : node["library"][name]["dependencies"]){
                    library.dependencies.push_back(dependency.as<std::string>());
                }
                break;
            }
        }
        return library;
    }
    
    std::vector<Library> Library::parse_libraries(YAML::Node& node) {
        std::vector<Library> libraries;
        for(auto it : node["library"]) {
            libraries.push_back(Library::parse(node, it.first.as<std::string>()));
        }
        return libraries;
    }
    
    void Library::show(Library& library) {
        std::cout << "[" + library.name + "]" << "\n";
        std::cout << "type: " << library.type << "\n";
        std::cout << "include: "<< nieel::accumulate(library.include, std::string("\n  - ")) << "\n";
        std::cout << "source: " << nieel::accumulate(library.source, std::string("\n  - ")) << "\n";
        std::cout << "dependencies: " << nieel::accumulate(library.dependencies, std::string("\n  - ")) << "\n";
        library.cmake_script();
    }
    
    std::string Library::cmake_script() {
        using namespace cmake;
        auto project = Cppm::instance()->project();
        if(project.libraries.empty()) return "";
        std::stringstream output("");
        output << set("thirdparty", nieel::accumulate(project.cmake_lib_name(dependencies), std::string{" "})) << endl()
               << "build_library(" << name << " " << type << " " << nieel::accumulate(source, std::string{" "}) << " " << get("thirdparty") << ")" << endl()
               ;
        return output.str();
    }
}