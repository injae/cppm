#ifndef __CPPM_OPTION_CPPKG_H__
#define __CPPM_OPTION_CPPKG_H__

#include "option/base_option.h"
#include "package/package.h"

namespace cppm::option
{
    class Cppkg : public base_option
    {
    public:
        Cppkg();
    private:
        package::Package pkg;
        bool regist = false;
    };
}

#endif
