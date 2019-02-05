#ifndef CPPM_CONFIG_CPPM_TOOL_H__
#define CPPM_CONFIG_CPPM_TOOL_H__

#include "config/config.h"

namespace cppm::tool
{
    std::string target_define(Config& config);
    std::string target_dependencies(Config& config);
    std::string target_install(Config& config);
}

#endif
