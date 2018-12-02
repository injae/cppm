#ifndef __CPPM_PACKAGE_CPPM_PACKAGE_H__
#define __CPPM_PACKAGE_CPPM_PACKAGE_H__

#include "config/base_config.h"
#include "cppm_version.h"
#include "util/version.h"
#include <vector>

namespace cppm
{
    class CppmPackage : public base_config
    {
    public:
        void parse(table_ptr table);
    private:
        std::vector<std::string> tool_chains;
    };
}

#endif
