#ifndef __CPPM_PACKAGE_CPPM_CONFIG_H__
#define __CPPM_PACKAGE_CPPM_CONFIG_H__

#include "config/base_config.h"
#include "package/cppm_package.h"

namespace cppm
{
    class CppmConfig : public base_config
    {
    public:
        void parse(table_ptr table);
    private:
        CppmPackage package;
    };
}

#endif
