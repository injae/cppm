#ifndef __CPPM_PACKAGE_CPPM_CONFIG_H__
#define __CPPM_PACKAGE_CPPM_CONFIG_H__

#include "config/base_config.h"
#include "config/cppm_package.h"

namespace cppm
{
    class CppmConfig : public base_config
    {
    public:
        CppmConfig load();
        void parse(table_ptr table);
    public:
        CppmPackage package;
        std::string root_path;
    };
}

#endif
