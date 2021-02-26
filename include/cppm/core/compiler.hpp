#pragma once

#ifndef __CPPM_CORE_COMPILER_HPP__
#define __CPPM_CORE_COMPILER_HPP__

#include <serdepp/utility.hpp>

namespace cppm::core {
    class Compiler {
    public:
        derive_serde(Compiler, ctx
                     .name(name)
                     .or_value(option,"option", "")
                     .TAG(version);
                     )
        std::string name;
        std::optional<std::string> version;
        std::optional<std::string> option;
    };
}

#endif
