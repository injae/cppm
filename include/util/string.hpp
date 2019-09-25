#ifndef __CPPM_UTIL_STRING_HPP__
#define __CPPM_UTIL_STRING_HPP__

#include <string>

namespace cppm::util::str {
    inline std::string quot(const std::string& str) { return "\""+str+"\""; }
    inline std::string operator"" _quot(const char* str, std::size_t len) {  return quot({str, len}); }
}


#endif
