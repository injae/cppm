#include"cmake/find_package.h"
#include"cmake/generator.h"
#include"cppm.h"
#include<nieel/filesystem.h>
#include<cctype>
#include<fstream>
#include<regex>
#include<boost/filesystem.hpp>
#include<nieel/string.hpp>
namespace fs = boost::filesystem;

namespace cmake 
{
    std::vector<cppm::Thirdparty> find_package_list() {
        std::vector<cppm::Thirdparty> packages;
        
        std::string cmake_path = "/usr/local/share";
        auto cmake_dir = nieel::find_regex_files(cmake_path, std::regex("cmake.*"));
        auto module_path = cmake_path + "/" + cmake_dir[0] + "/Modules";
        
        for(auto config_file : nieel::find_regex_files(module_path, std::regex("Find.*cmake"))) {
            cppm::Thirdparty package;
            std::regex filter("Find(.*)\\.cmake");
            std::smatch what;
            if(!std::regex_search(config_file, what, filter)) continue;
            
            package.name = what[1];
            nieel::str::to_lower(package.name);
            package.config_file = module_path + "/" + config_file;
            packages.emplace_back(std::move(package));
        }
        
        return packages;
    }
    
    std::vector<cppm::Thirdparty> config_package_list() {
        std::vector<cppm::Thirdparty> packages;
        
        std::string cmake_path = "/usr/local/lib/cmake";
        auto library_dirs = nieel::find_regex_files(cmake_path, std::regex(".*"));
        for(auto dir : library_dirs) {
           cppm::Thirdparty package;
           package.name = dir;
           
           auto package_path = cmake_path + "/" + package.name;
           package.config_file = package_path + "/" + package.name + "-config.cmake";
           
           packages.emplace_back(std::move(package));
        }
        
        return packages; 
    }
    
    std::vector<cppm::Thirdparty> get_find_package_list() {
        std::vector<cppm::Thirdparty> packages;
        auto package1 = find_package_list();
        auto package2 = config_package_list();
        packages.insert(packages.end(), package1.begin(), package1.end());
        packages.insert(packages.end(), package2.begin(), package2.end());
        return packages;
    }
    
    void get_package_config_hint(cppm::Thirdparty& thirdparty) {
        char fdata[512];
        std::ifstream ifs(thirdparty.config_file);
        if(ifs.is_open()) {
            while(!ifs.eof()) {
               memset(fdata, 0, 512);
               ifs.getline(fdata, 512);
               std::smatch what;
               std::string filter_str = "(.*)_LIBRARIES";
               std::regex filter(filter_str);
               std::string data(fdata);
               if(!std::regex_search(data, what, filter)) continue;
               std::cout << what[0] << std::endl;
            }
        }
        
        ifs.close(); 
    }
    
    void make_find_library(cppm::Thirdparty& library) {
        auto project = Cppm::instance()->project();
        auto file_name = project.path.cmake_find_module + "/Find"+library.name+".cmake";
        std::ofstream file(file_name); file.is_open();
        file <<  cppm_find_library(library);
        file.close();
    }
    
}