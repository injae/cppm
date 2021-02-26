#pragma once

#ifndef __CPPM_CORE_PACKAGE_HPP__
#define __CPPM_CORE_PACKAGE_HPP__

#include <serdepp/utility.hpp>
#include <cppm_version.h>

namespace cppm::core {
    class Package {
    public:
        derive_serde(Package, ctx
                     .TAG(name)
                     .TAG_OR(version, "0.0.1")
                     .TAG_OR(standard, "17")
                     .TAG_OR(description, "")
                     .tag(tool_version, "tool-version", CPPM_VERSION)
                     .tag(with_vcpkg, "vcpkg", false)
                     .tag(unity_build, "unity-build", false)
                     .tag(git_repo, "git")
                     .no_remain();)

        std::string name;
        std::optional<std::string> version;
        std::optional<std::string> tool_version;
        std::optional<std::string> description;
        std::optional<std::string> standard;
        std::optional<std::string> git_repo;
        std::optional<bool> with_vcpkg;
        std::optional<bool> unity_build;
    };
}

#endif
