#pragma once

#ifndef __CPPM_CORE_CMAKE_HPP__
#define __CPPM_CORE_CMAKE_HPP__

#include <tomlpp/orm.hpp>

namespace cppm::core {
    class CMake : public toml::orm::table {
    public:
        template<typename Def>
        void parse(Def& defn) {
            defn.element(TOML_D(version),"3.12")
                .element(TOML_D(option))
                .element(TOML_D(compiler))
                .element(TOML_D(builder))
                .element(TOML_D(toolchain))
                .element(TOML_D(include))
                .no_remains();
        }
        opt<std::string> version;
        opt<std::string> option;
        opt<std::string> compiler;
        opt<std::string> builder;
        opt<std::string> toolchain;
        opt<arr<std::string>> include;
    };
}


#endif

