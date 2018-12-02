#include "option/init.h"
#include <fmt/format.h>
#include <iostream>
#include "config/path.h"
#include <cstdlib>

namespace cppm::option
{
    Init::Init() {
        using namespace fmt::literals;
        app_.add_option("help")
            .abbr("h")
            .desc("show cppm command and options")
            .call_back([&](){ app_.show_help(); });
        app_.add_option("bin")
            .abbr("b")
            .desc("initialize new c++ binary project")
            .call_back([&](){ this->make_bin(); });
        app_.add_option("lib")
            .abbr("l")
            .desc("initialize new c++ library project")
            .call_back([&](){ this->make_lib(); });
    }

    void Init::make_bin() {
        auto config = make_project();
        auto bin = cpptoml::make_table();
        auto name = *config->get_table("package")->get_as<std::string>("name");
        bin->insert("name", name);

        auto source = cpptoml::make_array();
        source->push_back("include/.*");
        source->push_back("src/.*");
        bin->insert("source", source);

        auto bin_table = cpptoml::make_table_array();
        bin_table->push_back(bin);
        config->insert("bin", bin_table);

        auto project = Path::make((fs::current_path()/name).string());
        fs::fstream file; 
        file.open(project.root + "/cppm.toml", std::ios::out);
        file << *config;
        file.close();

        file.open(project.source + "/main.cpp", std::ios::out);
        file << "int main(int argc, char* argv[]) {\n\n return 0; \n}";
        file.close();
    } 

    void Init::make_lib() {
        auto config = make_project();
        auto lib = cpptoml::make_table();
        auto name = *config->get_table("package")->get_as<std::string>("name");
        lib->insert("name", name);
        lib->insert("type", "shared");

        auto source = cpptoml::make_array();
        source->push_back("include/.*");
        source->push_back("src/.*");
        lib->insert("source", source);

        auto lib_table = cpptoml::make_table_array();
        lib_table->push_back(lib);
        config->insert("lib", lib_table);

        auto project = Path::make((fs::current_path()/name).string());
        fs::fstream file; 
        file.open(project.root + "/cppm.toml", std::ios::out);
        file << *config;
        file.close();

        fs::create_directory(project.include + "/" + name);
    }

    std::shared_ptr<cpptoml::table> Init::make_project() {
        if(app_.args().size() <= 0)  { std::cerr << "need project name"  << std::endl; exit(1);}
        if(app_.args().size() >  1)  { std::cerr << "too many argument"  << std::endl; exit(1);}
        auto project_name = app_.args().front(); app_.args().pop_front();

        if(fs::exists(project_name)) { std::cerr << "this name is exist" << std::endl; exit(1);}
        fs::create_directory(project_name);
        auto project = Path::make((fs::current_path()/project_name).string());
        fs::create_directory(project.build);
        fs::create_directory(project.cmake);
        fs::create_directory(project.include);
        fs::create_directory(project.source);
        fs::create_directory(project.thirdparty);
        fs::create_directory(project.cmake);
        fs::create_directory(project.cmake + "/Modules");

        auto cppm_path = std::string(std::getenv("HOME")) + "/.cppm/";
        fs::copy(cppm_path + "cmake/project_maker.cmake", project.cmake+"/project_maker.cmake");
        auto config = cpptoml::make_table();
        auto package = cpptoml::make_table();
        package->insert("description", " ");
        package->insert("version"    , "0.0.1");
        package->insert("name"       , project_name);
        config->insert("package", package);

        return config;
    }
}
