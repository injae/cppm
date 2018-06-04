#ifndef __CPPM_H__
#define __CPPM_H__

#include<boost/filesystem.hpp>
#include<fstream>
#include<string>
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
private:
    Cppm();
    void make_project_property();
    fs::path find_config_file();
    
private:
    Project project_;
};


#endif