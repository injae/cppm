#pragma once

#ifndef __CPPM_CORE_COMPILER_HPP__
#define __CPPM_CORE_COMPILER_HPP__

#include <tomlpp/orm.hpp>

namespace cppm::core {
    class Compiler : public toml::orm::table {
    public:
        template<typename Def>
        void parse(Def& defn) {
            name = defn.name();
            defn.if_value(TOML_D(option))
                .element(TOML_D(version))
                .no_remains();
        }
        std::string name;
        opt<std::string> version;
        opt<std::string> option;
    };
}

#endif
