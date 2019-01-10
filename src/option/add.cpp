#include "option/add.h"
#include "config/cppm_package.h"
#include "package/package.h"
#include "util/filesystem.h"

#include <iostream>
#include <fmt/format.h>

namespace cppm::option
{
    Add::Add() {
        app_.add_option("help")
            .abbr("h")
            .desc("show cppm command and options")
            .call_back([&](){ app_.show_help(); });
        app_.add_command("toolchain")
            .desc("add cmake toolchain #example# cppm add toolchains {toolchain_path}... ")
            .args("{path}")
            .call_back([&](){ this->toolchain(); });
        app_.add_command("cppkg")
            .desc("add new cppkg")
            .args("{name}")
            .call_back([&](){ this->dependencies(); });
    }

    void Add::toolchain() {
        if(!app_.args().empty()) {
            CppmPackage::add_toolchain(app_.args().front());
            app_.args().pop_front();
        }
    }

    void Add::dependencies() {
        if(app_.args().empty())    { fmt::print(stderr,"need argument");}
        if(app_.args().size() > 1) { fmt::print(stderr,"too many argument");}
        package::cppkg::regist(app_.get_arg());
    }

}
