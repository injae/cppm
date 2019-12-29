#ifndef __CPPM_UTIL_HASH_HPP__
#define __CPPM_UTIL_HASH_HPP__

#include<iostream>
#include<string>

namespace cppm::util
{
    constexpr uint64_t mix(char m, uint64_t s) {
        return ((s<<7) + ~(s>>3)) + ~m;
    }
     
    constexpr uint64_t hash(const char* m) {
        return (*m) ? mix(*m,hash(m+1)) : 0;
    }
    
    inline uint64_t hash(const std::string& str) {
        return hash(str.c_str());
    }
    
    inline constexpr uint64_t operator "" _h(const char* str, std::size_t size) {
        return hash(str);
    }
    
    inline constexpr uint64_t operator "" _h(const char* str) {
        return hash(str);
    }
    
}

#endif

