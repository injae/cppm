#ifndef __CPPM_CONFIG_LIB_H__
#define __CPPM_CONFIG_LIB_H__

#include "config/base_config.h"
#include "config/base_target.h"
#include <vector>

namespace cppm
{
    class Lib : public base_target {};

    class Libs : public base_config
    {
    public:
        void parse(table_ptr table);
        std::string generate(Config& config);
    public:
        std::vector<Lib> list;
    };
}


#endif
