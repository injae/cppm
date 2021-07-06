#pragma once

#ifndef __CPPM_OPTION_BUILD_H__
#define __CPPM_OPTION_BUILD_H__

#include "option/base_option.h"
#include "cmake/cmake.h"
#include <string>
#include <optional>

namespace cppm::option
{
    class Build : public base_option
    {
    public:
        Build();
    private:
        void export_cppkg();
        void print_config();
        void transcompile(core::Path& path);
    private:
        cmake::Cmake cmake_;
        bool none_tc = false;
        bool only_tc = false;
        bool clean = false;
        std::optional<std::string> print_format = std::nullopt;
    };
}

#endif
