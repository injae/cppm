#ifndef __CPPM_CONFIG_CONFIG_H__
#define __CPPM_CONFIG_CONFIG_H__

#include <vector>
#include "config/base_config.h"
#include "config/path.h"
#include "config/package.h"
#include "config/builder.h"
#include "config/bin.h"
#include "config/lib.h"
#include "config/cmake.h"
#include "config/compiler.h"
#include "config/dependency.h"
#include "config/cppm_config.h"
#include "config/hunter.h"
#include "config/workspace.h"
//#include "config/test.h"

namespace cppm
{
    class Config : public base_config , public std::enable_shared_from_this<Config>
    {
    public:
        using ptr = std::shared_ptr<Config>;
        static Config::ptr load(const std::string& path);
        void parse(table_ptr table) override;
        void build_lock(table_ptr table, table_ptr lock) override;
        void after_init(Config::ptr config) override;
        std::string generate();
        static void write(table_ptr table);
    public:
        Path         path;
        Cmake        cmake;
        Bins         bins;
        Libs         libs;
        //Test         test;
        WorkSpace    workspace;
        Hunter       hunter;
        Package      package;
        Builder      builder;
        Compiler     compiler;
        Dependencies dependencies;
        CppmConfig   cppm_config;
        table_ptr lock;
    };
}

#endif
