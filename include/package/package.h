#ifndef __CPPM_PACKAGE_PACKAGE_H__
#define __CPPM_PACKAGE_PACKAGE_H__

#include "config/base_config.h"
#include "config/config.h"
#include "config/dependency.h"
#include "package/repo.h"
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
        std::string description;
        std::string version;
        Dependencies deps;
        download_ download;
        cmake_ cmake;
        bool global = false;
    };

namespace cppkg
{
    void init(const std::string& name);
    void init(Package& package);
    Cppkg list();
    void build(const std::string& name);
    void regist(const std::string& name);
    std::string search(const std::string& name, const std::string& version); //return to path
    void install(Config& config, const std::string& path); //return to path
}
}

#endif
