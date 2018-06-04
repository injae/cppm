#ifndef __CMAKE_GENERATOR_H__
#define __CMAKE_GENERATOR_H__

#include<string>
#include<string_view>

namespace cmake 
{
   std::string      make_default_project(std::string project_name, std::string version);
   std::string      cmake_version(std::string cmake_version, std::string project_name);
   std::string      project(std::string project_name);
   std::string      get(std::string value_name);
   std::string      set(std::string variable_name, std::string value);
   std::string      include_directory(std::string path);
   std::string      add_subdirectory(std::string path);
   std::string      include(std::string cmake_file);
   std::string      compiler_flag(std::string compiler_flag);
   std::string_view endl();
}

#endif