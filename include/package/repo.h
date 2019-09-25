#ifndef __CPPM_PACKAGE_REPO_H__
#define __CPPM_PACKAGE_REPO_H__

#include <map>
#include "util/version.h"
#include <string>

namespace cppm::package::cppkg
{
    struct Cppkg {
        struct Repo{
            struct Pkg {
                std::map<Version, std::string> versions;
            };
            std::map<std::string, Pkg> pkgs;
        };
        std::map<std::string, Repo> repos;
    };
}


#endif
