#pragma once

#ifndef __CPPM_CORE_CPPM_TOOL_HPP__
#define __CPPM_CORE_CPPM_TOOL_HPP__

#include <fmt/format.h>
#include "cppm/core/config.hpp"
#include "cppm/util/system.hpp"

using namespace fmt::literals;

namespace cppm::core {
    inline std::string cppm_root() { return "{}.cppm/"_format(cppm::util::system::home_path()); }
    std::string cppm_translate(Config& config);
    std::string cppm_header(Config& config);
    std::string include_cmake_files(Config& config);
    std::string find_cppkg(Config& config);
    std::string hunter_load(Config& config);
    std::string cppm_target_define(Config& config);
    std::string cppm_target_dependencies(Config& config);
    std::string cppm_target_install(Config& config);
    std::string cppm_compiler_option(Config& config);
    std::string cppm_download_package(Dependency& dep);
    Dependency cppm_auto_generate_cppkg(fs::path& path);
    void install_cppm_download_package(fs::path path, Dependency& dep);
    std::string hunter_root();
}

#endif
