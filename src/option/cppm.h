#pragma once

#ifndef __CPPM_OPTION_CPPM_H__
#define __CPPM_OPTION_CPPM_H__

#include "option/base_option.h"

namespace cppm::option
{

    class Cppm : public base_option
    {
    public:
        Cppm();
        void run(int argc, char* argv[]);
        void _run();
        void _update();
    };
}


#endif 
