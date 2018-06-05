#ifndef __CPPM_H__
#define __CPPM_H__

#include<boost/filesystem.hpp>
#include<yaml-cpp/yaml.h>
#include<fstream>
#include<string>

#include"cppm_thirdparty.h"

namespace fs = boost::filesystem;

class Cppm 
{
public:
    struct Project
    {
          std::string name
        ; std::string version
        ; std::string path
        ; std::string bin
        ; std::string source
        ; std::string include
        ; std::string thirdparty
        ; std::string cmake_module
        ; std::string cmake_find_module
        ;
    };
public:
    static Cppm* instance();
    void run(int argc, char** argv);
    const Project& project() { return project_; }
    const std::vector<cppm::Thirdparty>& thirdparties() { return thirdparties_; }
private:
    Cppm();
    void make_project_property();
    fs::path find_config_file();
    void parse_thirdparty(YAML::Node& node);
    
private:
    Project project_;
    std::vector<cppm::Thirdparty> thirdparties_; 
};


#endif