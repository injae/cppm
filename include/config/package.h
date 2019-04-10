#ifndef __CPPM_CONFIG_PACKAGE_H__
#define __CPPM_CONFIG_PACKAGE_H__

#include "config/base_config.h"

namespace cppm
{
    class Package : public base_config
    {
    public:
        void parse(table_ptr table);
    public:
        std::string name;
        std::string version;
        std::string description;
        std::string standard;
        std::string git_repo;
    };
}



#endif
