#ifndef __CPPM_UTIL_COMMAND_H__
#define __CPPM_UTIL_COMMAND_H__

#include <fmt/format.h>
#include <string>

namespace cppm::util
{
    using namespace fmt::literals;
    class Command
    {
    public:
        Command(std::string cmd) : cmd_(cmd) {}
        void operator[](std::string args) { cmd_ += (" " + args); }
        std::string get() { return cmd_; }
    private:
        std::string cmd_;
    };
}

#endif
