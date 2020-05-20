#pragma once

#ifndef __CPPM_CORE_DEPENDENCY_HPP__
#define __CPPM_CORE_DEPENDENCY_HPP__

#include <tomlpp/orm.hpp>
#include <fmt/format.h>

namespace cppm::core {
    class Dependency : public toml::orm::table {
    public:
        template<typename Def>
        void parse(Def& defn) {
            name = defn.name();
            defn.if_value(TOML_D(version), "latest")
                .element(TOML_D(type), "lib")
                .element(TOML_D(description), "")
                .element(TOML_D(link), "public")
                .element(TOML_D(repo), "cppkg")
                .element(TOML_D(path))
                .element(TOML_D(url))
                .element(TOML_D(sha256))
                .element(TOML_D(git))
                .element(TOML_D(flags))
                .element(TOML_D(helper))
                .element(TOML_D(branch))
                .element(TOML_D(custom), false)
                .element(no_cmake, "no_module", false)
                .element(TOML_D(components));
            if(not defn.is_value() && *type == "lib") defn.element(TOML_D(module), "");
            //defn.no_remains();
        }

        std::string name;
        opt<std::string> type;
        opt<std::string> version;
        opt<std::string> description;
        opt<std::string> link;
        opt<std::string> repo;
        opt<std::string> components;
        opt<std::string> path;
        opt<std::string> git;
        opt<std::string> url;
        opt<std::string> sha256;
        opt<std::string> helper;
        opt<std::string> branch;
        opt<std::string> flags;
        opt<bool> custom;
        opt<bool> no_cmake;
        opt<std::string> module;
    };
}

#endif
