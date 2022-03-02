#pragma once

#ifndef __CPPM_CORE_COMPILER_HPP__
#define __CPPM_CORE_COMPILER_HPP__

#include <serdepp/utility.hpp>


namespace cppm::core {
    struct Compiler {
        DERIVE_SERDE(Compiler,
                     (&Self::option,  "option", value_or_struct)
                     (&Self::version, "version", make_optional)
                     )
        std::string version;
        std::string option;
    };
}

#endif
