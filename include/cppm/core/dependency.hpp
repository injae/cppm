#pragma once

#ifndef __CPPM_CORE_DEPENDENCY_HPP__

#define __CPPM_CORE_DEPENDENCY_HPP__

#include <serdepp/utility.hpp>
#include <serdepp/attributes.hpp>

#include "cppm/core/feature.hpp"
#include "cppm/core/cppkg.hpp"
#include <algorithm>

namespace cppm::core {

    enum class link_type { PUBLIC, PRIVATE, INTERFACE };
    enum class repo_type { cppkg, hunter, system, vcpkg, workspace };

    struct Dependency {
        DERIVE_SERDE(Dependency,
                     (&Self::version,    "version",  value_or_struct)
                     (&Self::description, "description", make_optional)
                     (&Self::type,       "type",     default_{cppkg_type::lib})
                     (&Self::repo,       "repo",     default_{repo_type::cppkg})
                     (&Self::link,       "link",     default_{link_type::PRIVATE}, to_lower)
                     (&Self::module,     "module")
                     (&Self::features,   "feature-map", make_optional)
                     (&Self::default_feature, "features", make_optional)
                     (&Self::default_features_flag, "default_features", default_{true})
                     (&Self::custom,     "custom")
                     (&Self::no_cmake,   "no_module")
                     (&Self::optional,   "optional", default_{false})
                     (&Self::components, "components")
                     (&Self::path,       "path")
                     (&Self::git,        "git")
                     (&Self::branch,     "branch")
                     (&Self::url,        "url")
                     (&Self::sha256,     "sha256")
                     (&Self::helper,     "helper")
                     (&Self::flags,      "flags")
                     (&Self::dependencies, "dependencies", make_optional)
                     )

        std::string name;
        cppkg_type type;
        std::string version;
        link_type link;
        repo_type repo;
        std::string description;
        std::map<std::string, Feature> features;
        std::vector<Feature> default_feature;
        std::optional<std::string> components;
        std::optional<std::string> path;

        std::optional<std::string> git;
        std::optional<std::string> branch;

        std::optional<std::string> url;
        std::optional<std::string> sha256;

        std::optional<std::string> helper;
        std::optional<std::string> flags;

        std::optional<bool> custom;
        std::optional<bool> no_cmake;
        bool optional;
        bool default_features_flag;
        std::optional<std::string> module;
        std::unordered_map<std::string, Dependency> dependencies;
    };
}


#endif

