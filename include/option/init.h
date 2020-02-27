#ifndef __CPPM_OPTION_INIT_H__
#define __CPPM_OPTION_INIT_H__

#include "option/base_option.h"
#include "config/config.h"
#include <string>
#include <memory>

namespace cppm::option
{
    class Init : public base_option
    {
    public:
        Init();
    private:
        void make_bin(const std::string& name);
        void make_lib(const std::string& name);
        std::string make_project(const std::string& name);
        void make_gitignore(Path& project);
    };
}

#endif
