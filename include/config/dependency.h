#ifndef __CPPM_CONFIG_DEPENDENCY_H__
#define __CPPM_CONFIG_DEPENDENCY_H__

#include <map>
#include "config/base_config.h"
 
namespace cppm
{
    class Config;

    namespace detail {
        struct download
        {
            std::string url;
            std::string branch;
            bool is_git=false;
        };
    }

    struct Dependency
    {
        std::string name;
        std::string version;
        std::string desc;
        std::string module;
        std::string components;
        std::string load_path;
        std::string flags;
        bool none_module;
        bool hunter;
        std::string link_type;
        std::string type; // lib | bin | cmake
        detail::download download;
        std::string helper; // Find${LIbrary}.cmake add
        bool custom = false; // Find${LIbrary}.cmake add
        void parse(table_ptr table);
    };

    class Dependencies : public base_config
    {
    public:
        void parse(table_ptr table) override;
        void after_init(std::shared_ptr<Config> config) override;
        std::string use_hunter(std::shared_ptr<Config> config);
        std::string generate();
        ~Dependencies(){}
    public:
        std::map<std::string, Dependency> list;
    };
}

#endif
