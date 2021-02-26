#pragma once

#ifndef __CPPM_CORE_PROFILE_HPP__
#define __CPPM_CORE_PROFILE_HPP__

#include "cppm/core/compiler.hpp"
#include <serdepp/utility.hpp>

namespace cppm::core {
    class Profile {
    public:
        derive_serde(Profile, ctx
                     .name(name)
                     .TAG(package)
                     .TAG(compiler);
                     )

        std::string name;
        std::optional<std::map<std::string,Profile>> package;
        std::optional<std::map<std::string,Compiler>> compiler;
    };
}

#endif
