#ifndef __CPPM_H__
#define __CPPM_H__

#include<boost/filesystem.hpp>
#include<yaml-cpp/yaml.h>
#include<fstream>
#include<string>
#include<memory>

#include"cppm_options.h"
#include"config/thirdparty.h"

namespace fs = boost::filesystem;


class Cppm 
{
public:
    struct Project
    {
          std::string name
        ; std::string version
		; std::string compiler
		; std::string compiler_option
		; std::string builder
		; std::string cpu_core
		; std::string type
        ; std::string path
        ; std::string bin
        ; std::string source
        ; std::string include
        ; std::string thirdparty
        ; std::string cmake_module
        ; std::string cmake_find_module
        ; std::vector<std::string> user_cmake_script
        ;
    };
public:
    static Cppm* instance();
    void run(int argc, char** argv);
    Project& project() { return project_; }
    const std::vector<cppm::Thirdparty>& thirdparties() { return thirdparties_; }
    std::vector<std::string> user_cmake_script_parser(YAML::Node& node);
    void parse_project_config();
    void make_config_file(Project& project);
private:
    Cppm();
    void make_project_property();
    fs::path find_config_file();
    void parse_thirdparty(YAML::Node& node);
private:
    Project project_;
    std::unique_ptr<CppmOptions> option_;
    std::vector<cppm::Thirdparty> thirdparties_; 
};


#endif