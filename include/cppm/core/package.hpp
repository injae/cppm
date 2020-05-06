#pragma once

#ifndef __CPPM_CORE_PACKAGE_HPP__
#define __CPPM_CORE_PACKAGE_HPP__

#include <tomlpp/orm.hpp>

namespace cppm::core {
    class Package : public toml::orm::table {
    public:
        template<typename Def>
        void parse(Def& defn) {
            defn.element(TOML_D(name))
                .element(TOML_D(version), "0.0.1")
                .element(TOML_D(standard), "17")
                .element(TOML_D(description), "")
                .element(with_vcpkg, "vcpkg", false)
                .element(git_repo, "git")
                .no_remains();
        }
        std::string name;
        opt<std::string> version;
        opt<std::string> description;
        opt<std::string> standard;
        opt<std::string> git_repo;
        opt<bool> with_vcpkg;
    };
}

#endif
