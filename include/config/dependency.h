#ifndef __CPPM_CONFIG_DEPENDENCY_H__
#define __CPPM_CONFIG_DEPENDENCY_H__

#include <map>
#include "config/base_config.h"
#include "config/hunter.h"

namespace cppm
{
    struct Dependency
    {
        using table_ptr = std::shared_ptr<cpptoml::table>;
        std::string name;
        std::string module;
        bool none_module;
        bool hunter;
        std::string link_type;
        std::string version;
        std::string components;
        void insert(table_ptr table);
    };

    class Dependencies : public base_config
    {
    public:
        void parse(table_ptr table);
        std::string gen_find_package();
        std::string gen_find_cppkg();
        std::string use_hunter(Hunter& hunter);
        std::string generate();
    public:
        std::map<std::string, Dependency> list;
    };
}

#endif
