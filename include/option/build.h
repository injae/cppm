#ifndef __CPPM_OPTION_BUILD_H__
#define __CPPM_OPTION_BUILD_H__

#include "option/base_option.h"
#include "config/config.h"
#include <string>

namespace cppm::option
{
    struct CommandBuilder
    {
        std::string build(Config& config);
        std::string cmake_option = "";
        std::string build_option = "";
    };
    
    class Build : public base_option
    {
    public:
        Build();
    private:
        void cmakelist_build();
        void dependency_check();
        void export_cppkg();
        CommandBuilder cmd;
        bool none_tc = false;
        bool clean = false;
    };
}

#endif
