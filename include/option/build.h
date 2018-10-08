#ifndef __CPPM_OPTION_BUILD_H__
#define __CPPM_OPTION_BUILD_H__

#include "option/base_option.h"
#include "config/config.h"
#include <string>

namespace cppm::option
{
    class CommandBuilder
    {
    public:
        CommandBuilder(Config& config) : config(config) {}
        std::string build();
        std::string cmake_option = "";
        std::string build_option = "";
        Config& config;
    };
    
    class Build : public base_option
    {
    public:
        Build(Config& config);
    private:
        void cmakelist_build(Config& config);
    };
}

#endif
