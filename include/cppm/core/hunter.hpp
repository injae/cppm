#pragma once

#ifndef __CPPM_CORE_HUNTER_HPP__
#define __CPPM_CORE_HUNTER_HPP__

#include <tomlpp/orm.hpp>

namespace cppm::core {
    class Hunter : public toml::orm::table {
    public:
        template<typename Def>
        void parse(Def& defn) {
            defn.element(TOML_D(url),  "https://github.com/cpp-pm/hunter/archive/v0.23.253.tar.gz")
                .element(TOML_D(sha1), "88ea6d37c897a81a080eb9ae0f69d7807bbb3c73")
                .element(TOML_D(use),  false);
        }
        opt<std::string> url;
        opt<std::string> sha1;
        opt<bool> use;
    };

}

#endif
