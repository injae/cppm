#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <functional>
#include <string>
#include <fmt/format.h>
#include "cppm/util/system.hpp"
#include <fmt/format.h>

using namespace std::literals;
using namespace fmt::literals;

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

    std::string home_path() {
        return (util::system::what() == "windows"s)
            ? "/" : "{}/"_format(std::getenv("HOME"));
    }
}
