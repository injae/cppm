#ifndef __CPPM_OPTION_CPPKG_INIT_H__
#define __CPPM_OPTION_CPPKG_INIT_H__

#include "option/base_option.h"
#include "package/package.h"

namespace cppm::option
{
    class CppkgInit : public base_option
    {
    public:
        CppkgInit();
    private:
        package::Package pkg;
        bool regist = false;
    };
}

#endif
