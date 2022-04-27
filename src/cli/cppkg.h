#pragma once

#ifndef __CPPM_CLI_CPPKG_H__
#define __CPPM_CLI_CPPKG_H__

#include "cli/util.hpp"
#include "cli/cppkg_search.h"
#include "cli/cppkg_init.h"

namespace cppm::command {

    struct CppkgUpdate : callback_interface {
        static constexpr auto desc ="update cppkg repo";
        static void callback(std::vector<std::string>& args);
    };

    struct CppkgBuild : callback_interface {
        static constexpr auto desc ="build cppkg.toml file";
        static void callback(std::vector<std::string>& args);
    };

    struct CppkgGenerate : callback_interface {
        static constexpr auto desc ="generate {cppkg name}.cmake.in";
        static void callback(std::vector<std::string>& args);
    };

    class Cppkg {
    public:
        DERIVE_SERDE(Cppkg,
                     .attrs(desc{"cppkg option and commands"}, callback{Cppkg::execute})
                     _SF_(update)
                     _SF_(search)
                     _SF_(build)
                     _SF_(init)
                     )
    private:
        static void execute(Cppkg& cmd);
        SingleCommand<CppkgUpdate> update;
        SingleCommand<CppkgBuild>  build;
        CppkgInit                  init;
        CppkgSearch                search;
    };

}


#endif
