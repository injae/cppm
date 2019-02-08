#ifndef __CPPM_CONFIG_HUNTER_H__
#define __CPPM_CONFIG_HUNTER_H__

#include "config/base_config.h"
#include <vector>

namespace cppm
{

    class Hunter : public base_config
    {
    public:
        void parse(table_ptr table);
        std::string generate();
        static std::string package_path();
    public:
        std::string url;
        std::string sha1;
    };
}


#endif
