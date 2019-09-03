#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <functional>
#include <string>
#include <fmt/format.h>
#include "util/system.hpp"

namespace cppm::util::system
{
#if defined(_WIN32)
    #define _POPEN _popen
    #define _PCLOSE _pclose
#else
    #define _POPEN popen
    #define _PCLOSE pclose
#endif

    std::string exec(const char* cmd, const std::function<void(std::string&)>& func ) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&_PCLOSE)> pipe(_POPEN(cmd, "r"), _PCLOSE);
        if (!pipe) { throw std::runtime_error("popen() failed!"); }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
            func(result);
        }
        return result;
    }
}
