#ifndef __CPPM_UTIL_COMMAND_H__
#define __CPPM_UTIL_COMMAND_H__

#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <reproc++/reproc.hpp>

namespace console {
    using hook = std::function<bool(reproc::stream, const u_int8_t*, size_t)>;
    namespace hooks {
        struct normal {
            bool operator()(reproc::stream stream, const uint8_t *buffer, size_t size) {
                (void) stream;
                std::cout << std::string(reinterpret_cast<const char *>(buffer), size) << std::endl;
                return true;
            }
        };
    }
    class console
    {
    public:
        console(std::string cmd) : cmd_(cmd) {}
        void operator[](std::string args) { cmd_ += (" " + args); }
        std::string get() { return cmd_; }
        void execute(hook&& hook_= hooks::normal());
        reproc::options options;
    private:
        std::string cmd_;
    };

    std::vector<std::string> split(const std::string &txt, char ch);
}

#endif
