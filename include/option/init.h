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
        void make_bin();
        void make_lib();
        std::string make_project();
    };
}

#endif
