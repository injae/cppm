#ifndef __CPPM_OPTION_CPPKG_INIT_H__
#define __CPPM_OPTION_CPPKG_INIT_H__

#include "option/base_option.h"
#include "cppkg/cppkg.h"
#include "config/dependency.h"

namespace cppm::option
{
    class CppkgInit : public base_option
    {
    public:
        CppkgInit();
    private:
        Dependency pkg;
        bool regist = false;
    };
}

#endif
