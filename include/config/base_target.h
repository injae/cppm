#ifndef __CPPM_CONFIG_BASE_TARGET_H__
#define __CPPM_CONFIG_BASE_TARGET_H__

#include <vector>
#include <string>

namespace cppm
{
    class Config;

    class base_target
    {
    public:
        std::string name;
        bool install;
        std::vector<std::string> sources;
        std::vector<std::string> deps;
        std::vector<std::string> get_sources(Config& config);
    };
}

#endif
