#pragma once

#ifndef __CPPM_CORE_HUNTER_HPP__
#define __CPPM_CORE_HUNTER_HPP__

#include <serdepp/utility.hpp>


namespace cppm::core {
    struct Hunter {
        DERIVE_SERDE(Hunter,
                     (&Self::url, "url", default_{"https://github.com/cpp-pm/hunter/archive/v0.23.269.tar.gz"})
                     (&Self::sha1, "sha1", default_{"64024b7b95b4c86d50ae05b926814448c93a70a0"})
                     (&Self::use, "use", default_{false})
                     .no_remain()
                     )
        std::string url;
        std::string sha1;
        bool use;
    };
}

#endif

