#pragma once

#ifndef __CPPM_CORE_DEPENDENCY_HPP__
#define __CPPM_CORE_DEPENDENCY_HPP__

#include <fmt/format.h>
#include <serdepp/utility.hpp>
#include <cppm/core/features.hpp>

namespace cppm::core {
    class Dependency {
    public:
        derive_serde(Dependency, ctx
                     .name(name)
                     .or_value(version, "version", "latest")
                     .TAG_OR(type, "lib")
                     .TAG_OR(description, "")
                     .TAG_OR(link, "public")
                     .TAG_OR(repo, "cppkg")
                     .tag(features, "features", std::map<std::string, Feature>())
                     .TAG(module)
                     .TAG(path)
                     .TAG(url)
                     .TAG(sha256)
                     .TAG(git)
                     .TAG(flags)
                     .TAG(helper)
                     .TAG(branch)
                     .TAG_OR(custom, false)
                     .TAG(optional)
                     .TAG(components)
                     .tag(no_cmake, "no_module", false);
                     )

        std::string name;
        std::optional<std::string> type;
        std::optional<std::string> version;
        std::optional<std::string> description;
        std::optional<std::string> link;
        std::optional<std::string> repo;
        std::optional<std::string> components;
        std::optional<std::string> path;
        std::optional<std::string> git;
        std::optional<std::string> url;
        std::optional<std::string> sha256;
        std::optional<std::string> helper;
        std::optional<std::string> branch;
        std::optional<std::string> flags;
        std::optional<std::map<std::string, Feature>> features;
        std::optional<bool> custom;
        std::optional<bool> no_cmake;
        std::optional<bool> optional;
        std::optional<std::string> module;
    };
}

#endif
