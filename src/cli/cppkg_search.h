#pragma once

#ifndef __CPPM_CLI_CPPKG_SEARCH_H__
#define __CPPM_CLI_CPPKG_SEARCH_H__

#include "cli/util.hpp"

namespace cppm::command {

    class CppkgSearch {
    public:
        DERIVE_SERDE(CppkgSearch,
                     .attrs(desc{"Search for a package"}, callback{CppkgSearch::execute})
                     [attrs(option("-r,--repo", "repository to search"))]
                     _SF_(repo)
                     [attrs(option("targets", "search targets"))]
                     _SF_(targets)
                     [attrs(option("-a,--all", "show all results"))]
                     _SF_(all)
                     )
    private:
        static void execute(CppkgSearch& cmd);
    private:
        std::string repo;
        std::vector<std::string> targets;
        bool all;
    };
}

#endif
