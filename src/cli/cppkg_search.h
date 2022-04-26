#pragma once

#ifndef __CPPM_CLI_CPPKG_SEARCH_H__
#define __CPPM_CLI_CPPKG_SEARCH_H__

#include "cli/util.hpp"

namespace cppm::command {

    class CppkgSearch {
    public:
        DERIVE_SERDE(CppkgSearch,
                     .attrs(desc{"Search for a package"},
                            callback{CppkgSearch::execute},
                            option_group("Search","Search options"))
                     [attrs(option("-r,--repo", "show {repo_name}'s packages"))]_SF_(repo)
                     [attrs(option("target", "search target"))]_SF_(target)
                     [attrs(flag("-a,--all", "show all results"))]_SF_(all)
                     [attrs(flag("-o,--one", "show one result"))]_SF_(one)
                     )
    private:
        static void execute(CppkgSearch& cmd);
    private:
        std::string repo;
        std::string target;
        bool all;
        bool one;
    };
}

#endif
