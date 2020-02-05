#ifndef __CPPM_CONFIG_BASE_TARGET_H__
#define __CPPM_CONFIG_BASE_TARGET_H__

#include <vector>
#include <string>
#include "config/compiler.h"
#include "config/dependency.h"

namespace cppm
{
    class Config;

    class base_target
    {
    public:
        std::string name;
        std::string type;
        bool install;
        std::string source;
        std::vector<std::string> sources;
        std::vector<std::string> deps;
        std::optional<Compiler> compiler = std::nullopt;
        std::optional<Dependencies> dependencies = std::nullopt;
        std::vector<std::string> get_sources(Config& config);
    };
}

#endif
