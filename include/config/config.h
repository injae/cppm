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

namespace cppm
{
    class Config : public base_config
    {
    public:
        static Config load(const std::string& path);
        void parse(table_ptr table);
        std::string generate();
        static void write(table_ptr table);
    public:
        Path         path;
        Cmake        cmake;
        Bins         bins;
        Libs         libs;
        Package      package;
        Builder      builder;
        Compiler     compiler;
        Dependencies dependencies;
        CppmConfig  cppm_config;
    };
}

#endif
