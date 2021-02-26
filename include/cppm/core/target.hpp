#pragma once

#ifndef __CPPM_CORE_TARGET_HPP__
#define __CPPM_CORE_TARGET_HPP__

#include "cppm/core/dependency.hpp"
#include <serdepp/utility.hpp>

namespace cppm::core {
    class Target{
    public:
        derive_serde(Target, ctx
                     .name(name)
                     .TAG(dependencies)
                     .tag(dev_dependencies, "dev-dependencies")
                     .no_remain();
                     )

        std::string name;
        std::optional<std::map<std::string,Dependency>> dependencies;
        std::optional<std::map<std::string,Dependency>> dev_dependencies;
    };
}

#endif
