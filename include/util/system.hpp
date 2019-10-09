#ifndef __CPPM_UTIL_SYSTEM_HPP__
#define __CPPM_UTIL_SYSTEM_HPP__

#include "util/hash.hpp"
#include <fmt/format.h>
#include <functional>


namespace cppm::util::system
{
    #if   defined(_WIN32)
        #define PLATFORM_NAME "windows" // Windows
    #elif defined(_WIN64)
        #define PLATFORM_NAME "windows" // Windows
    #elif defined(__CYGWIN__) && !defined(_WIN32)
        #define PLATFORM_NAME "windows" // Windows (Cygwin POSIX under Microsoft Window)
    #elif defined(__ANDROID__)
        #define PLATFORM_NAME "android" // Android (implies Linux, so it must come first)
    #elif defined(__linux__)
        #define PLATFORM_NAME "linux" // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
    #elif defined(__unix__) || defined(__APPLE__) && defined(__MACH__)
        #include <sys/param.h>
        #if defined(BSD)
            #define PLATFORM_NAME "bsd" // FreeBSD, NetBSD, OpenBSD, DragonFly BSD
        #endif
    #elif defined(__hpux)
        #define PLATFORM_NAME "hp-ux" // HP-UX
    #elif defined(_AIX)
        #define PLATFORM_NAME "aix" // IBM AIX
    #elif defined(__APPLE__) && defined(__MACH__) // Apple OSX and iOS (Darwin)
        #include <TargetConditionals.h>
        #if TARGET_IPHONE_SIMULATOR == 1
            #define PLATFORM_NAME "ios" // Apple iOS
        #elif TARGET_OS_IPHONE == 1
            #define PLATFORM_NAME "ios" // Apple iOS
        #elif TARGET_OS_MAC == 1
            #define PLATFORM_NAME "osx" // Apple OSX
        #endif
    #elif defined(__sun) && defined(__SVR4)
        #define PLATFORM_NAME "solaris" // Oracle Solaris, Open Indiana
    #else
        #define PLATFORM_NAME NULL
    #endif
        
    constexpr const char* what() { return (PLATFORM_NAME == NULL) ? "" : PLATFORM_NAME; }
    
    constexpr const char* prefix() {
        switch(hash(what()))
        {
        case "linux"_h:
        case "osx"_h:
            return "/usr/local/";
        case "windows"_h:
            return "C:/users/";
        }
    }

    std::string home_path();

    std::string exec(const char* cmd ,const std::function<void(std::string&)>& func = [](auto& str) { fmt::print(str);});
}

#if   defined(_MSC_VER)
        #define COMPILER_NAME "msvc"  // visual studio  
        #define COMPILER_VERSION _MSC_VER
        #define USE_MSVC
#elif defined(__GNUC__)
        #define COMPILER_NAME "gcc"   // gcc
        #define COMPILER_VERSION __GNUC__##__GNUC_MINOR__
        #define USE_GCC
#elif defined(__clang__)
    #define COMPILER_NAME "clang" // clang
    #define COMPILER_VERSION __clang_major__##__clang_minor__##__clang_patchlevel__
        #define USE_CLANG
#elif defined(__EMSCRIPTEN__)
    #define COMPILER_NAME "wsm"   // web assembly
        #define USE_WSM
#elif defined(__MINGW32__) "mingw"  
    #define COMPILER_NAME "mingw" // mingw 32bit mingw-w64 32bit
        #define USE_MINGW32
#elif defined(__MINGW64__) "mingw" 
    #define COMPILER_NAME "mingw" // mingw-w64 64bit
        #define USE_MINGW64
#else
    #define COMPILER_NAME NULL    // can't find 
#endif

namespace cppm::util::compiler
{
    constexpr const char* what() { return (COMPILER_NAME == NULL) ? "" : COMPILER_NAME; }
}

#endif
