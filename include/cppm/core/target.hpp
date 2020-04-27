#pragma once

#ifndef __CPPM_CORE_TARGET_HPP__
#define __CPPM_CORE_TARGET_HPP__

#include "cppm/core/dependency.hpp"

namespace cppm::core {
    class Target : public toml::orm::table {
    public:
        template<typename Def>
        void parse(Def& defn) {
           defn.element(TOML_D(dependencies))
               .element(dev_dependencies,"dev-dependencies")
               .no_remains();
        }
        opt<nested<Dependency>> dependencies;
        opt<nested<Dependency>> dev_dependencies;
    };
}

#endif
