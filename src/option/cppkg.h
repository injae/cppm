#pragma once

#ifndef __CPPM_OPTION_CPPKG_H__
#define __CPPM_OPTION_CPPKG_H__

#include "option/base_option.h"

namespace cppm::option
{
    class Cppkg : public base_option
    {
    public:
        Cppkg();
    private:
        void _update();
        void _build();
        void _push();
    };
}

#endif
