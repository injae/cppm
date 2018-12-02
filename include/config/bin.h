#ifndef __CPPM_CONFIG_BIN_H__
#define __CPPM_CONFIG_BIN_H__

#include "config/base_config.h"
#include <vector>

namespace cppm 
{
    class Config;

    struct Bin 
    {
        std::string name;
        std::vector<std::string> sources;
    };

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
