#ifndef __CPPM_PACKAGE_PACKAGE_H__
#define __CPPM_PACKAGE_PACKAGE_H__

#include "config/base_config.h"
#include <string>

namespace cppm::package
{
    struct cmake
    {
        std::string name;
        std::string components;
        std::string option;
    };

    struct git
    {
        std::string url;
        std::string tag;
    };

    struct download
    {
        std::string url;
        git git;
    };

    struct build
    {
        std::string config;
        std::string build;
        std::string install;
    };

    class Package : public base_config
    {
    public:
        void parse(table_ptr table);
        std::string generate();
    public:
        std::string name;
        std::string version;
        std::string target;
        download download;
        cmake cmake;
        build build;
    };
}

#endif
