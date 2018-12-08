#include "option/add.h"
#include "config/cppm_package.h"

namespace cppm::option
{
    Add::Add(Config& config) {
        app_.add_option("help")
            .abbr("h")
            .desc("show cppm command and options")
            .call_back([&](){ app_.show_help(); });
        app_.add_command("toolchain")
            .desc("add cmake toolchain #example# cppm add toolchains {toolchain_path}... ")
            .call_back([&](){ this->toolchain(); });
        app_.add_command("dep")
            .desc("add cmake toolchain #example# cppm add toolchains {toolchain_path}... ")
            .call_back([&](){ this->dependencies(config); });
    }

    void Add::toolchain() {
        if(!app_.args().empty()) {
            CppmPackage::add_toolchain(app_.args().front());
            app_.args().pop_front();
        }
    }

    void Add::dependencies(Config config) {
        if(!app_.args().empty()) {
            app_.args().front();
            app_.args().pop_front();
        }
    }
}
