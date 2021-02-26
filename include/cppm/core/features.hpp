#pragma once

#ifndef __CPPM_CORE_FEATURES_HPP__
#define __CPPM_CORE_FEATURES_HPP__


#include <fmt/format.h>
#include <serdepp/utility.hpp>

namespace cppm::core {
    class Feature {
    public:
        derive_serde(Feature, ctx
                     .name(key)
                     .or_value(member, "member")
                     .TAG(type)
                     .TAG(value);
                     )
        std::string key;
        std::optional<std::string> value;
        std::optional<std::string> type;
        std::vector<std::string> member;
    };

    
}


#endif
