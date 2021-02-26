#pragma once

#ifndef __CPPM_CORE_CMAKE_HPP__
#define __CPPM_CORE_CMAKE_HPP__

#include <serdepp/utility.hpp>

namespace cppm::core {
    class CMake {
    public:
        derive_serde(CMake, ctx
                        .TAG_OR(version, "3.12")
                        .TAG(options)
                        .TAG(compiler)
                        .TAG(builder)
                        .TAG(toolchain)
                        .TAG(include)
                        .no_remain();)

        std::optional<std::string> version;
        std::optional<std::string> options;
        std::optional<std::string> compiler;
        std::optional<std::string> builder;
        std::optional<std::string> toolchain;
        std::optional<std::vector<std::string>> include;
    };
}


#endif

