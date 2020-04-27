#pragma once

#ifndef __CPPM_CORE_HUNTER_HPP__
#define __CPPM_CORE_HUNTER_HPP__

#include <tomlpp/orm.hpp>

namespace cppm::core {
    class Hunter : public toml::orm::table {
    public:
        template<typename Def>
        void parse(Def& defn) {
            defn.element(TOML_D(url),  "https://github.com/ruslo/hunter/archive/v0.23")
                .element(TOML_D(sha1), "e14bc153a7f16d6a5eeec845fb0283c8fad8c358")
                .element(TOML_D(use),  false);
        }
        opt<std::string> url;
        opt<std::string> sha1;
        opt<bool> use;
    };

}

#endif
