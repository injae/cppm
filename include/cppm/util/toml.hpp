#pragma once

#ifndef CPPM_UTIL_TOML_HPP
#define CPPM_UTIL_TOML_HPP

#include <toml.hpp>
#include "cppm/util/string.hpp"

namespace cppm::util::toml {
    inline ::toml::value find_recursive(const ::toml::value& value, const std::string& key) {
        auto keys = str::split(key, '.');
        ::toml::value buf = value;
        for(auto key : keys) {
            auto temp = ::toml::find(buf, key);
            buf = temp;
        }
        return buf;
    }
}


#endif
