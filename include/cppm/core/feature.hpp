#pragma once

#ifndef __CPPM_CORE_FEATURES_HPP__
#define __CPPM_CORE_FEATURES_HPP__

#include <serdepp/utility.hpp>
#include <serdepp/attributes.hpp>

namespace cppm::core {
    struct Feature {
        DERIVE_SERDE(Feature,
                     [attributes(value_or_struct)]_SF_(key)
                     [attributes(default_{"OFF"})]_SF_(value))
        std::variant<std::string, std::vector<Feature>> key;
        std::string value;
    };
}


#endif
