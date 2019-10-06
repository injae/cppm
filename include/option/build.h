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
        std::string after_option = "";
        std::string target = "";
        bool is_install = false;
        bool is_install_local = true;
    };
    
    class Build : public base_option
    {
    public:
        Build();
    private:
        void export_cppkg();
        CommandBuilder cmd;
        bool none_tc = false;
        bool only_tc = false;
        bool clean = false;
    };
}

#endif
