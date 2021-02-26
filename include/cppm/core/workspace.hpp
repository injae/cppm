#pragma once

#ifndef __CPPM_CORE_WORKSPACE_HPP__
#define __CPPM_CORE_WORKSPACE_HPP__

#include <serdepp/utility.hpp>

namespace cppm::core {
    class Workspace {
    public:
        derive_serde(Workspace, ctx.TAG(member);)
        std::optional<std::vector<std::string>> member;
    };
}

#endif
