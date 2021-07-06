#pragma once

#ifndef __CPPM_CORE_PACKAGE_HPP__
#define __CPPM_CORE_PACKAGE_HPP__

#include <serdepp/utility.hpp>
#include <serdepp/attributes.hpp>

namespace cppm::core {
    enum class cxx_standard {
        cpp_98 = 98,
        cpp_11 = 11,
        cpp_14 = 14,
        cpp_17 = 17,
        cpp_20 = 20,
        cpp_23 = 23
    };

    struct Package {
        DERIVE_SERDE(Package,
                (&Self::name,         "name")
                (&Self::version,      "version",      default_se{"0.0.1"})
                (&Self::tool_version, "tool-version", default_se{"0.0.13"})
                (&Self::description,  "description",  default_se{""})
                (&Self::standard,     "standard",     default_se{"17"})
                (&Self::with_vcpkg,   "vcpkg",        default_se{false})
                (&Self::unity_build,  "unity",        default_se{false})
                (&Self::git_repo,     "git")
                .no_remain())
        std::string name;
        std::string version;
        std::string tool_version;
        std::string description;
        std::string standard;
        std::optional<std::string> git_repo;
        bool with_vcpkg;
        bool unity_build;
    };
}


#endif
