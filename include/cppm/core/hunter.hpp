#pragma once

#ifndef __CPPM_CORE_HUNTER_HPP__
#define __CPPM_CORE_HUNTER_HPP__

#include <serdepp/utility.hpp>

namespace cppm::core {
    class Hunter {
    public:
        derive_serde(Hunter, ctx
                     .TAG_OR(url, "https://github.com/cpp-pm/hunter/archive/v0.23.269.tar.gz")
                     .TAG_OR(sha1, "64024b7b95b4c86d50ae05b926814448c93a70a0")
                     .TAG_OR(use, false)
                     .no_remain();
                     )
        std::optional<std::string> url;
        std::optional<std::string> sha1;
        std::optional<bool> use;
    };

}

#endif
