#ifndef __CPPM_PACKAGE_CPPM_PACKAGE_H__
#define __CPPM_PACKAGE_CPPM_PACKAGE_H__

#include "config/base_config.h"
#include <vector>
#include <string>

namespace cppm
{
    class CppmPackage : public base_config
    {
    public:
        void parse(table_ptr table);
        static void add_toolchain(const std::string& toolchain);
        static void add_cppm_path(const std::string& path);
        static std::string config_path();
        std::string toolchains();
    public:
        std::vector<std::string> tool_chains;
        std::string cppm_path;
    };
}

#endif
