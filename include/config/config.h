#ifndef __CPPM_CONFIG_CONFIG_H__
#define __CPPM_CONFIG_CONFIG_H__

#include <vector>
#include "config/base_config.h"
#include "config/package.h"
#include "config/bin.h"
#include "config/dependency.h"

namespace cppm
{
    class Config : public base_config
    {
    public:
        static Config load(const std::string& path);
        void parse(table_ptr table);
        std::string generate();
    public:
        Package package;
        Bins bins;
        Dependencies dependencies;
    };
}

#endif
