#ifndef __CPPM_CONFIG_BIN_H__
#define __CPPM_CONFIG_BIN_H__

#include "config/base_target.h"
#include "config/base_config.h"
#include "config/dependency.h"
#include <vector>

namespace cppm 
{
    class Config;

    class Bin : public base_target {};

    class Bins : public base_config
    {
    public:
        void parse(table_ptr table);
        std::string generate(Config& config);
    public:
        std::vector<Bin> list;
    };
}


#endif
