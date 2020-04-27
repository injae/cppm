#pragma once

#ifndef __CPPM_CORE_WORKSPACE_HPP__
#define __CPPM_CORE_WORKSPACE_HPP__

#include <tomlpp/orm.hpp>

namespace cppm::core {
    class Workspace : public toml::orm::table {
    public:
        template<typename Def>
        void parse(Def& defn) {
            defn.element(TOML_D(member));
        }
        opt<arr<std::string>> member;
    };
}

#endif
