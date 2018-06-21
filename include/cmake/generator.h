#ifndef __CMAKE_GENERATOR_H__
#define __CMAKE_GENERATOR_H__

#include<string>
#include<string_view>
#include"cppm.h"
#include"config/thirdparty.h"

namespace cmake 
{
   //std::string      option(std::string name, std::string descripte, std::string on_off);
   std::string      is_on_shared(cppm::Project& project);
   std::string      make_default_project(cppm::Project& project);
   std::string      cmake_version(std::string cmake_version, std::string project_name);
   std::string      cmake_project(std::string project_name);
   std::string      get(std::string value_name);
   std::string      set(std::string variable_name, std::string value);
   std::string      include_directory(std::string path);
   std::string      add_subdirectory(std::string path);
   std::string      include(std::string cmake_file);
   std::string      compiler_flag(std::string compiler_flag);
   std::string      cppm_find_library(cppm::Thirdparty& thirdparty);
   std::string      make_cppm_lib_bin(std::string type, std::string name);
   std::string      make_external_library(cppm::Thirdparty& thirdparty);
   std::string      include_user_script(cppm::Project& project);
   std::string_view endl();
}

#endif