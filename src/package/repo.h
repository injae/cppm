#ifndef __CPPM_PACKAGE_REPO_H__
#define __CPPM_PACKAGE_REPO_H__

#include <map>
#include "util/version.h"

namespace cppm::cppkg
{
    struct cppkg {
        struct Repo{
            struct Pkg {
                std::map<Version,std::string> versions;
            };
            std::map<std::string, Pkg> pkgs;
        };
        std::map<std::string,Repo> repos;
    };
}


#endif
