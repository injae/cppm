#ifndef __CPPM_CONFIG_DEPENDENCY_H__
#define __CPPM_CONFIG_DEPENDENCY_H__

#include <map>
#include "config/base_config.h"
 
namespace cppm
{
    class Config;
    struct Dependency
    {
        std::string name;
        std::string module;
        bool none_module;
        bool hunter;
        std::string link_type;
        std::string type; // lib | bin | cmake
        std::string version;
        std::string components;
        std::string load_path;
        void insert(table_ptr table);
    };

    class Dependencies : public base_config
    {
    public:
        void parse(table_ptr table);
        std::string gen_find_package();
        std::string gen_find_cppkg();
        std::string use_hunter(Config& config);
        std::string generate();
        ~Dependencies(){}
    public:
        std::map<std::string, Dependency> list;
    };
}

#endif
