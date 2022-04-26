#include "cli/init.h"
#include "cppm/util/filesystem.h"
#include "cppm/core/config.hpp"
#include "cppm/core/cppm_tool.hpp"
#include "cppm/util/string.hpp"
#include <cstdlib>
#include <serdepp/adaptor/toml11.hpp>

using namespace fmt::literals;
using namespace cppm::util::str;

namespace cppm::command
{
    std::string Init::make_project(const std::string& name, bool dupl_check) {
        if(dupl_check && fs::exists(name)) { std::cerr << "this name is exist" << std::endl; exit(1);}
        fs::create_directory(name);
        auto project = core::Path::make((fs::current_path()/name).string());
        fs::create_directory(project.build);
        fs::create_directory(project.cmake);
        fs::create_directory(project.include);
        fs::create_directory(project.source);
        fs::create_directory(project.thirdparty);
        fs::create_directory(project.cmake);
        fs::create_directory(project.cmake/"Modules");
        make_gitignore(project);

        auto cppm_path = core::cppm_root();
        util::over_write_copy_file("{0}cmake/cppm_loader.cmake"_format(core::cppm_root())
                                    ,"{0}/cppm_loader.cmake"_format(project.cmake));
        return "[package]\n"
             + "   name = {}\n"_format(quot(name))
             + "   version = {}\n"_format("0.0.1"_quot)
             + "   description = {}\n"_format(""_quot)
             + "\n"
             ;
    }

    void Init::make_gitignore(core::Path& project) {
        std::fstream file; 
        file.open(project.root/".gitignore", std::ios::out);
        file << "build/\n.ccls-cache\ncompile_commands.json\n"_format();
        file.close();
    }

    void Init::execute(Init& init) {
        auto gen = make_project(init.name);

        if(init.bin) {
            gen += "[[bin]]\n"
                +  "   name = {}\n"_format(quot(init.name))
                +  "   source = [{}]\n"_format("src/.*"_quot);
        }
        if(init.lib) {
            gen += "[lib]\n"
                +  "   name = {}\n"_format(quot(init.name))
                +  "   type = {}\n"_format("shared"_quot)
                +  "   source = [{}]\n"_format("src/.*"_quot);
        }
        //auto toml = serde::serialize<toml::value>(init.dep, "dependencies");
        //std::stringstream ss; ss << toml;
        //gen += ss.str();
        
        auto project = core::Path::make((fs::current_path()/init.name).string());
        std::fstream file; 
        file.open(project.root/"cppm.toml", std::ios::out); file << gen; file.close();

        if(init.bin) {
            file.open(project.source/"main.cpp", std::ios::out);
            file << "#include <iostream>\nint main(int argc, char* argv[]) {\n"
                    "std::cout<<\"hello world\"<<std::endl;\n    return 0; \n}";
            file.close();
        }
        else {
            fs::create_directory(project.include/init.name);
        }
    }
}
