#pragma once

#ifndef __CPPM_CORE_PROFILE_HPP__
#define __CPPM_CORE_PROFILE_HPP__

#include "cppm/core/compiler.hpp"

namespace cppm::core {
    class Profile : public toml::orm::table {
    public:
        template<typename Def>
        void parse(Def& defn) {
            name = defn.name();
            defn.element(TOML_D(package))
                .element(TOML_D(compiler))
                ;
        }
        std::string name;
        opt<nested<Profile>> package;
        opt<nested<Compiler>> compiler;
    };
}

#endif
