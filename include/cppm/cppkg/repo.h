#pragma once

#ifndef __CPPKG_REPO_H__
#define __CPPKG_REPO_H__

#include <map>
#include "cppm/util/version.h"
#include <string>

namespace cppkg {
    struct Pkg
    {
    public:
        std::map<cppm::Version, std::string> versions;
    };
    struct Repo
    {
    public:
        std::map<std::string, Pkg> pkgs;
    };
    using Repos = std::map<std::string, Repo>;
}

#endif
