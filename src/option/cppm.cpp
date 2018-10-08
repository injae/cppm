#include <iostream>
#include <string>

#include "option/cppm.h"
#include "util/filesystem.h"
#include "option/build.h"
#include <fmt/format.h>

namespace cppm::option
{
    Cppm::Cppm() {
        config_load();
        app_.add_option("help")
            .abbr("h")
            .desc("show cppm commands and options")
            .call_back([&](){ app_.show_help(); });
        app_.add_command("build")
            .desc("make Cmakelists.txt and project build")
            .call_back([&](){ Build(config_).app().parse(app_);});
    } 

    void Cppm::config_load() {
        auto path = cppm::util::reverse_find_file(fs::current_path(), "cppm.toml");
        if(!path) std::cout << "can't find cppm.toml" << std::endl;
        config_ = cppm::Config::load(path->parent_path().string());
    }
    
    void Cppm::generate_cmakelists() {
        std::ofstream CmakeLists(config_.path.root + "/CMakeLists_test.txt"); CmakeLists.is_open();
        CmakeLists << config_.generate();
        CmakeLists.close();
    }

    void Cppm::run(int argc, char **argv) {
        app_.parse(argc, argv);
    }
}
