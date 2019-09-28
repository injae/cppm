#ifndef __CPPM_UTIL_STRING_HPP__
#define __CPPM_UTIL_STRING_HPP__

#include <string>

namespace cppm::util::str {
    inline std::string quot(const std::string& str) { return "\""+str+"\""; }
    inline std::string operator"" _quot(const char* str, std::size_t len) {  return quot({str, len}); }
    inline std::string str_cut(const std::string& str, size_t size) {
            return str.size() > size ? str.substr(0, size-1) + "$" : str;
    }
    inline bool has_str(const std::string& target, const std::string& str) {
        if(target == "")  { return  false; }
        return target.find(str) != std::string::npos;
    }
}


#endif
