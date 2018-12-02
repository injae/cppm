#ifndef __CPPM_OPTION_MAKE_H__
#define __CPPM_OPTION_MAKE_H__

#include "option/base_option.h"

namespace cppm::option
{
    class Make : base_option
    {
    public:
        Make();
        
    private:
        nlpo::App app_;
    };
}




#endif
