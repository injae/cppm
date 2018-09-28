#ifndef __CPPM_CONFIG_DEPENDENCY_H__
#define __CPPM_CONFIG_DEPENDENCY_H__

#include <vector>
#include "config/base_config.h"

namespace cppm
{
    struct Dependency
    {
        std::string name;
        std::string cmake_name;
        std::string version;
    };

    class Dependencies : public base_config
    {
    public:
        void parse(table_ptr table);
        std::string generate();
    public:
        std::vector<Dependency> list;
    };
}

#endif
