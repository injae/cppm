#include "option/add.h"

#include <iostream>
#include <fmt/format.h>

using namespace fmt::literals;

namespace cppm::option
{
    Add::Add() {
        app_.add_command("toolchain")
            .desc("add cmake toolchain #example# cppm add toolchains {toolchain_path}... ")
            .args("{path}")
            .call_back([&](){ _toolchain(); });
        app_.add_command("cppm_path")
            .desc("add cppm path to use cppm update command")
            .args("{path}")
            .call_back([&](){ _cppm_path(); });
    }

    void Add::_toolchain() {
        if(app_.args().empty()) {
            fmt::print(stderr, "need argument");
            exit(1);
        }
        //        CppmPackage::add_toolchain(app_.get_arg());
    }

    void Add::_cppm_path() {
        if(app_.args().empty()) {
            fmt::print(stderr, "need argument");
            exit(1);
        }
        //CppmPackage::add_cppm_path(app_.get_arg());
    }

}
