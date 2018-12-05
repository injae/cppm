#ifndef __CPPM_OPTION_INSTALL_H__
#define __CPPM_OPTION_INSTALL_H__

#include "option/base_option.h"
#include "config/config.h"
#include <cpptoml.h>
#include <memory>

namespace cppm::option
{
    class Install : public base_option
    {
    public:
        Install(Config& config);
    };
}

#endif
