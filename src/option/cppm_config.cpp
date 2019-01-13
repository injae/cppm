#include "option/cppm_config.h"
#include "option/add.h"
#include <fmt/format.h>

namespace cppm::option
{
    CppmConfig::CppmConfig() {
        app_.add_option("help")
            .abbr("h")
            .desc("show cppm command and options")
            .call_back([&](){ app_.show_help(); });
        app_.add_command("add")
            .desc("add cppm config")
            .call_back([&](){ Add().app().parse(app_); });
    }
}
