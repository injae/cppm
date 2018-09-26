#ifndef __CPPM_H__
#define __CPPM_H__

#include<boost/filesystem.hpp>
#include<yaml-cpp/yaml.h>
#include<fstream>
#include<string>
#include<memory>

#include"cppm_options.h"
#include"config/project.h"

namespace fs = boost::filesystem;

class Cppm 
{
public:
public:
    static Cppm* instance();
    void run(int argc, char** argv);
    cppm::Project& project() { return project_; }
    void parse_project_config();
    void make_config_file(cppm::Project& project);
private:
    Cppm();
    fs::path find_config_file();
private:
    cppm::Project project_;
    std::unique_ptr<CppmOptions> option_;
};

#endif
