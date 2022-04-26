#pragma once

#ifndef __CPPM_CLI_INIT_H__
#define __CPPM_CLI_INIT_H__

#include "cppm/core/config.hpp"
#include "cli/util.hpp"

#include <serdepp/attribute/cli11.hpp>

namespace cppm::command
{
    using namespace serde::attribute::cli11;
    class Init 
    {
    public:
        DERIVE_SERDE(Init,
                     .attrs(desc{"make c++ project"},
                            option_group("Type", "Project Type",[](auto* g){ g->require_option(1); }),
                            callback(execute))
                     [attrs(group_flag("Type", "-b,--bin", "initialize binary project"))]_SF_(bin)
                     [attrs(group_flag("Type", "-l,--lib", "initialize library project"))]_SF_(lib)
                     [attrs(option("name", "project name", [](auto* opt){ opt->required(); }))]_SF_(name)
                     )
    private:
        static void execute(Init& cmd);
        static std::string make_project(const std::string& name, bool dupl_check=true);
        static void make_gitignore(core::Path& project);

        std::string name;
        bool bin, lib;
        std::map<std::string, std::string> dep;
    };
}
#endif
