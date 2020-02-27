#ifndef CPPM_CONFIG_CPPM_TOOL_H__
#define CPPM_CONFIG_CPPM_TOOL_H__

#include "config/config.h"

namespace cppm::tool
{
    std::string cppm_root();
    std::string target_define(Config::ptr config);
    std::string target_dependencies(Config::ptr config);
    std::string target_install(Config::ptr config);
}

#endif
