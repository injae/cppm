#pragma once

#ifndef __CPPM_OPTION_CPPKG_INSTALL_H__
#define __CPPM_OPTION_CPPKG_INSTALL_H__

#include "option/base_option.h"

namespace cppm::option
{
    class CppkgInstall : public base_option
    {
    public:
        CppkgInstall();
    private:
        std::string _base_cppm_project();
        std::string version = "latest";
    };
}

#endif
