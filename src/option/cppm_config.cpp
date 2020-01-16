#include "option/cppm_config.h"
#include "option/add.h"
#include <fmt/format.h>

namespace cppm::option
{
    CppmConfig::CppmConfig() {
        app_.add_command<Add>("add").desc("add cppm config");
    }
}
