#ifndef __CPPM_UTIL_CMAKE_H__
#define __CPPM_UTIL_CMAKE_H__

#include <fmt/format.h>

namespace cppm::util::cmake
{
    using namespace fmt::literals;
    inline std::string var(const std::string& str) { return "${{{0}}}"_format(str);}
}

#endif
