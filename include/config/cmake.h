#ifndef __CPPM_CONFIG_CMAKE_H__
#define __CPPM_CONFIG_CMAKE_H__

#include "config/base_config.h"
#include <vector>

namespace cppm
{

    class Cmake : public base_config
    {
    public:
        void parse(table_ptr table);
        std::string generate();
    public:
        std::string version;
        std::string option;
        std::string compiler;
        std::string builder;
        std::vector<std::string> tool_chain;
        std::vector<std::string> include;
    };
}


#endif
