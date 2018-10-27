#ifndef __CPPM_OPTION_INIT_H__
#define __CPPM_OPTION_INIT_H__

#include "option/base_option.h"
#include "config/config.h"
#include <string>

namespace cppm::option
{
    class Init : public base_option
    {
    public:
        Init(Config& config);
    private:
        void make_bin(Config& config);
        void make_lib(Config& config);
    };
}

#endif
