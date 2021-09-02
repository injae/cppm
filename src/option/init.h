#pragma once

#ifndef __CPPM_OPTION_INIT_H__
#define __CPPM_OPTION_INIT_H__

#include "option/base_option.h"
#include <string>

namespace cppm::option
{
    class Init : public base_option
    {
    public:
        Init();
        static std::string make_project(const std::string& name, bool dupl_check=true);
        static void make_gitignore(core::Path& project);
    private:
        void make_bin(const std::string& name);
        void make_lib(const std::string& name);
        std::string add_deps();
        std::vector<std::string> deps;
        std::string name;
        std::string type="bin"; // bin | lib
    };
}

#endif
