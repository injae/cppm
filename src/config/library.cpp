#include"config/library.h"
#include"cmake/generator.h"
#include<nieel/util/hash.hpp>
#include<nieel/string.hpp>
#include<nieel/filesystem.h>
#include<nieel/algorithm.hpp>
#include<iostream>
#include<sstream>
#include<typeinfo>

namespace cppm
{
    Library Library::parse(YAML::Node& node, std::string name) {
        using namespace nieel;
        auto root_path = nieel::reverse_find_file(fs::current_path(), "cppm.yaml")->parent_path().string();
        Library library;
        library.name = name;
        for(auto it : node["library"][name]) {
            auto property = it.first.as<std::string>().c_str(); 
            switch(hash(property))
            {
            case "type"_h:
                library.type = it.second.as<std::string>();
                break;
            case "version"_h:
                library.version = it.second.as<std::string>();
                break;
            case "include"_h:
                for(auto include : node["library"][name]["include"]){
                    library.include.push_back(include.as<std::string>());
                }
                break;
            case "source"_h:
                for(auto source : node["library"][name]["source"]){
                    auto sources = nieel::find_files(root_path, std::regex(source.as<std::string>()), false);
                    nieel::insert(library.source, sources);
                }
                break;
            case "dependencies"_h:
                for(auto dependency : node["library"][name]["dependencies"]){
                    library.dependencies.emplace_back(cppm::parse_dependency(dependency.as<std::string>()));
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
        library.cmake_script();
    }
    
    std::string Library::cmake_script() {
        using namespace cmake;
        using trans_type = std::vector<std::string>;
        auto project = Cppm::instance()->project();
        if(project.libraries.empty()) return "";
        std::stringstream output("");
        output << set("thirdparty", nieel::str::accumulate(nieel::transform<trans_type>(dependencies, [](auto dep){ return dep.cmake_name; }), std::string{"\n\t"}) + "\n") << endl()
               << set("source"    , nieel::str::accumulate(source, std::string{"\n\t"}) + "\n") << endl()
               << "build_library(" << name << " " << type << " \"" << get("source") << "\" \"" << get("thirdparty") << "\")" << endl()
               ;
        return output.str();
    }
}