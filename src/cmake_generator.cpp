#include"cmake_generator.h"
#include"boost/filesystem.hpp"
#include<sstream>
#include<string_view>

namespace fs = boost::filesystem;

namespace cmake 
{
   std::string make_default_project(std::string project_name, std::string version) {
      std::stringstream output("");
      output << cmake_version(version,project_name)  << endl()
             << set("PROJECT_NAME", project_name)    << endl()
             << endl()
             
             << project(get("PROJECT_NAME"))         << endl()
             << endl()
             
             << set("PROJECT_ROOT_DIR"      , get("CMAKE_CURRENT_SOURCE_DIR")        ) << endl()
             << set("SOURCE_DIR"            , get("PROJECT_ROOT_DIR") + "/src"       ) << endl() 
             << set("INCLUDE_DIR"           , get("PROJECT_ROOT_DIR") + "/include"   ) << endl()
             << set("THIRD_PARTY_DIR"       , get("PROJECT_ROOT_DIR") + "/thirdparty") << endl()
             << set("TEST_DIR"              , get("PROJECT_ROOT_DIR") + "/test"      ) << endl()
             << set("CMAKE_BINARY_DIR"      , get("PROJECT_ROOT_DIR") + "/bin"       ) << endl()
             << set("EXECUTABLE_OUTPUT_PATH", get("CMAKE_BINARY_DIR")                ) << endl()
             << set("CMAKE_MODULE_PATH"     , get("PROJECT_ROOT_DIR") + "/cmake "
                                            + get("CMAKE_MODULE_PATH")               ) << endl()
             << set("MODULE_PATH"           , get("CMAKE_MODULE_PATH")+ "/Modules"   ) << endl()
             << set("BUILD_DIR"             , get("PROJECT_NAME")     + "/build")      << endl()
             << endl()
             
             << set("CMAKE_CXX_FLAGS"       , get("CMAKE_CXX_FLAGS") + " -std=c++17 -g -Wall -lstdc++fs") << endl()
             << endl()
             
             << endl()
             
             << "enable_testing"                            << endl()
             << endl()
             ; 
      return output.str();
   }
   
   
   std::string cmake_version(std::string cmake_version, std::string project_name) {
      return  "cmake_minimum_required(VERSION " + cmake_version + " REQUIRED)";
   }
   
   std::string project(std::string project_name) {
      return "project("+project_name+")";
   }
   
   std::string get(std::string value_name) {
      return "${" + value_name + "}";      
   }
   
   std::string set(std::string variable_name, std::string value) {
      return "set(" + variable_name + " " + value + ")"; 
   }
   
   inline std::string_view endl() { return "\n"; }
   
   std::string include_directory(std::string path) {
      return "include_directory(" + path + ")";
   }
   
   std::string add_subdirectory(std::string path) {
      return "add_subdirectory("+ path + ")";
   }
   
   std::string add_excuatable(std::string name, std::string source) {
      return "add_excutable(" + name + " " + source + ")";
   }
   
   std::string include(std::string cmake_file) {
      return "include(" + cmake_file + ")";
   }
   
   std::string compiler_flag(std::string compiler_flag) {
      return set("CMAKE_CXX_FLAGS"       , get("CMAKE_CXX_FLAGS") + compiler_flag);
   }
   
}