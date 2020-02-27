#ifndef __CPPM_CONFIG_BASE_CONFIG_H__
#define __CPPM_CONFIG_BASE_CONFIG_H__

#include <cpptoml.h>
#include <memory.h>
#include <string>
#include <fmt/format.h>

#include "util/optional.hpp"

namespace cppm
{
    using namespace std::literals;
    using namespace fmt::literals;
    using table_ptr = std::shared_ptr<cpptoml::table>;
    class Config;
    class base_config
    {
    protected:
    public:
        virtual void parse(table_ptr table) = 0;
        virtual void build_lock(table_ptr table, table_ptr lock) {};
        virtual void after_init(std::shared_ptr<Config> config) {};
        virtual ~base_config() {};
    };
}


#endif
