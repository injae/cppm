#ifndef __CPPM_OPTION_BUILD_H__
#define __CPPM_OPTION_BUILD_H__

#include "option/base_option.h"
#include "config/config.h"
#include "cmake/cmake.h"
#include <string>

namespace cppm::option
{
    class Build : public base_option
    {
    public:
        Build();
    private:
        void export_cppkg();
        cmake::Cmake cmake_;
        bool none_tc = false;
        bool only_tc = false;
        bool clean = false;
    };
}

#endif
