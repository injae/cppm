#include"cmake/generator.h"
#include<nieel/util/hash.hpp>
#include"boost/filesystem.hpp"
#include<sstream>
#include<string_view>
#include<cctype>

namespace fs = boost::filesystem;

namespace cmake 
{
   std::string make_default_project(cppm::Project& project) {
      std::stringstream output("");
      output << cmake_version("3.6", project.package.name)  << endl()
             << endl()
             
             << cmake_project(project.package.name + " LANGUAGES CXX " + "VERSION " + project.package.version) << endl()
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
             << set("third_party_library", " ") << endl()
             << endl()
             << compiler_flag(project.compiler.option)
             << endl()
             << include(get("CMAKE_MODULE_PATH") + "/cmake_option.cmake") << endl()
             << "option( BUILD_SHARED_LIB \"ON is build shared lib\"" + is_on_shared(project)+ ")"
             << endl()
                
             << include(get("CMAKE_MODULE_PATH") + "/project_maker.cmake")<< endl()
             << make_cppm_lib_bin(project.package.type, get("PROJECT_NAME"))
             << endl()
             << include_user_script(project) 
             << endl()
                
            // << add_subdirectory(get("TEST_DIR"))               << endl()
            // << "enable_testing()"                            << endl()
             << endl()
             ; 
      return output.str();
   }
   
    std::string is_on_shared(cppm::Project& project) {
        using namespace nieel;
        switch(hash(project.package.type.c_str()))
        {
        case hash("static"):
            return " OFF";
        case hash("shared"):
            return " ON";
        default:
            return " ON";
        } 
    }
   
   //std::string option(std::string name, std::string descripte, std::string on_off) {
   //   return "option(" + name + " " + descripte + " " + on_off + ")";
   //}
   
   std::string cmake_version(std::string cmake_version, std::string project_name) {
      return  "cmake_minimum_required(VERSION " + cmake_version + ")";
   }
   
   std::string cmake_project(std::string project_name) {
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
      return set("CMAKE_CXX_FLAGS"       ,"\"" + get("CMAKE_CXX_FLAGS") +" "+compiler_flag+ "\"");
   }

   std::string cppm_find_library(cppm::Thirdparty& thirdparty) {
      std::stringstream output("");
      output << "find_package(" << thirdparty.name <<" REQUIRED)" << endl()
             << endl()
             << include(get("CMAKE_MODULE_PATH") +"/utils.cmake") << endl()
             << make_external_library(thirdparty)
             << "list(APPEND third_party_library " << thirdparty.name << ")" <<endl()
             ;
      return output.str();
   }
   
   std::string include_user_script(cppm::Project& project)  {
      std::stringstream output("");
      for(auto script : project.user_cmake_scripts) {
         output << include(script) << endl();
      }
      return output.str();
   }
   
   std::string make_cppm_lib_bin(std::string type, std::string name) {
      using namespace nieel;
      switch(hash(type.c_str())) 
      {
      case hash("bin"):
         return "make_excutable(" + name + ")";
      case hash("static"):
      case hash("shared"):
         return "make_lib(" + name + ")";
      default:
         std::cerr<< "wrong project type: " << type << std::endl;
         exit(1);
      }
   }
  
   std::string make_external_library(cppm::Thirdparty& thirdparty) {
      using namespace nieel; 
      
      switch(hash(thirdparty.build_type.c_str()))
      {
      case hash("static"):
         return "make_thirdparty_static_lib(" + thirdparty.name  + " " + thirdparty.cmake_var_name + ")";
      case hash("shared"): 
         return "make_thirdparty_shared_lib(" + thirdparty.name  + " " + thirdparty.cmake_var_name + ")";
      case hash("header_only"):
         return "make_thirdparty_header_only(" + thirdparty.name + " " + thirdparty.include_dir    + ")";
      default:
         std::cerr<< "wrong parameter" << std::endl;
         exit(1);
      }
   }
}