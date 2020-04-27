#pragma once

#ifndef __CPPM_OPTION_CPPKG_INIT_H__
#define __CPPM_OPTION_CPPKG_INIT_H__

#include "option/base_option.h"
#include "cppm/cppkg/cppkg.h"
#include "cppm/core/dependency.hpp"

namespace cppm::option
{
    class CppkgInit : public base_option
    {
    public:
        CppkgInit();
    private:
        core::Dependency pkg;
        bool regist = false;
    };
}

#endif
