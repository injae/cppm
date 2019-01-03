#ifndef __CPPM_PACKAGE_PACKAGE_H__
#define __CPPM_PACKAGE_PACKAGE_H__

#include "config/base_config.h"
#include "config/config.h"
#include <string>

namespace cppm::package
{
    struct cmake_
    {
        std::string name;
        std::string components;
        std::string option;
        std::string find_lib;
    };

    struct git_
    {
        std::string url;
        std::string tag;
    };

    struct download_
    {
        std::string url;
        git_ git;
    };

    class Package : public base_config
    {
    public:
        void parse(table_ptr table);
        std::string generate();
    public:
        std::string name;
        std::string version;
        std::string find_cmake;
        std::string header;
        std::string footer;
        download_ download;
        cmake_ cmake;
    };

namespace cppkg
{
    void init(const std::string& name);
    void build(const std::string& name);
    void regist(const std::string& name);
    std::string search(const std::string& name, const std::string& version); //return to path
    void install(Config& config, const std::string& path); //return to path
}
}

#endif
