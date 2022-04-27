#pragma once

#ifndef __CPPM_CLI_CPPKG_INIT_H__
#define __CPPM_CLI_CPPKG_INIT_H__

#include "cli/util.hpp"

#include <serdepp/attribute/cli11.hpp>

namespace cppm::command {
    using namespace serde::attribute::cli11;

    class CppkgInit
    { 
    public:
        DERIVE_SERDE(CppkgInit,
                     .attrs(desc{"init cppkg"},
                            option_group("URI", "Download URI", [](auto* g){ g->require_option(1); }),
                            callback(execute))
                     [attrs(group_option("URI", "-u,--url", "download url"))]_SF_(uri)
                     [attrs(group_option("URI", "-g,--git", "download git url"))]_SF_(git)

                     [attrs(option("--tag", "git tag", [](auto* o) {
                     }))]_SF_(git_tag)
                     [attrs(option("-d,--des", "package description"))]_SF_(description)
                     [attrs(option("-v,--version", "cppkg version", [](auto* o) {
                         o->required()->default_val("0.0.1");
                     }))]_SF_(version)
                     [attrs(option("-m,--module", "CMake Module Name", [](auto* o) {
                         o->required();
                     }))]_SF_(module)
                     [attrs(option("-t,--type", "package type", [](auto* o) {
                         o->default_val("lib");
                     }))]_SF_(type)
                     [attrs(option("target", "cppkg name", [](auto* o) { o->required(); }))]_SF_(target)
                     )
    private:
        static void execute(CppkgInit& cmd);
    private:
        std::string uri;
        std::string git;
        std::string git_tag;
        std::string module;
        std::string description;
        std::string type;
        std::string version;
        std::string target;
    };
}


#endif
