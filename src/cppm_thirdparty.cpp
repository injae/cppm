#include"cppm_thirdparty.h"
#include"utils.h"
#include"cmake_generator.h"
#include"cppm.h"
#include<boost/filesystem.hpp>
#include<fstream>
#include"url.h"
#include"boost/tokenizer.hpp"
#include"boost/algorithm/string/replace.hpp"
#include<boost/regex.hpp>
#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;

namespace cppm 
{
    Repository classificate_repo(std::string url) {
        Url parsed = parse_url(url);
        Repository repo; 
        repo.url = url;
        if(   has_str(parsed.protocol, "http") 
           || has_str(parsed.protocol, "https")) 
        {
            if(has_str(parsed.path, "git")) {
                repo.type = "git";
            }
            else if(has_str(parsed.path,"svn")) {
                repo.type = "svn";
            }
            else if(   has_str(parsed.path, "tar")
                    || has_str(parsed.path, "zip")
                    || has_str(parsed.path, "gz"))
            {
                repo.type = "link";
            }
            else {
                repo.type = "unknown";
            }
        }
        else {
            if(   has_str(parsed.path, "tar")
               || has_str(parsed.path, "zip")
               || has_str(parsed.path, "gz"))
            {
                repo.type = "file";
            }
            else {
                repo.type = "unknown";
            }
        }
        
        return repo;
    }
     
     
    bool has_find_package(Thirdparty thirdparty) {
        for(auto& package : get_cmake_find_package_list()) {
            if(package.name == thirdparty.name) return true;
        }
        return false;
    }
    
    std::vector<Thirdparty> cmake_find_package_list() {
        std::vector<Thirdparty> packages;
        
        std::string cmake_path = "/usr/local/share";
        auto cmake_dir = find_regex_files(cmake_path, boost::regex("cmake.*"));
        auto module_path = cmake_path + "/" + cmake_dir[0] + "/Modules";
        
        for(auto config_file : find_regex_files(module_path, boost::regex("Find.*cmake"))) {
            Thirdparty package;
            boost::regex filter("Find(.*)\\.cmake");
            boost::smatch what;
            if(!boost::regex_search(config_file, what, filter)) continue;
            
            package.name = what[1];
            boost::to_lower(package.name);
            package.config_file = module_path + "/" + config_file;
            packages.emplace_back(std::move(package));
        }
        
        return packages;
    }
    
    std::vector<Thirdparty> cmake_config_package_list() {
        std::vector<Thirdparty> packages;
        
        std::string cmake_path = "/usr/local/lib/cmake";
        auto library_dirs = find_regex_files(cmake_path, boost::regex(".*"));
        for(auto dir : library_dirs) {
           Thirdparty package;
           package.name = dir;
           
           auto package_path = cmake_path + "/" + package.name;
           package.config_file = package_path + "/" + package.name + "-config.cmake";
           
           packages.emplace_back(std::move(package));
        }
        
        return packages; 
    }
    
    std::vector<Thirdparty> get_cmake_find_package_list() {
        std::vector<Thirdparty> packages;
        auto package1 = cmake_find_package_list();
        auto package2 = cmake_config_package_list();
        packages.insert(packages.end(), package1.begin(), package1.end());
        packages.insert(packages.end(), package2.begin(), package2.end());
        return packages;
    }
    
    void get_package_config_hint(Thirdparty& thirdparty) {
        char fdata[512];
        std::ifstream ifs(thirdparty.config_file);
        if(ifs.is_open()) {
            while(!ifs.eof()) {
               memset(fdata, 0, 512);
               ifs.getline(fdata, 512);
               boost::smatch what;
               std::string filter_str = "(.*)_LIBRARIES";
               boost::regex filter(filter_str);
               if(!boost::regex_search(std::string(fdata), what, filter)) continue;
               std::cout << what[0] << std::endl;
            }
        }
        
        ifs.close(); 
    }
    
    void make_cmake_find_library(Thirdparty& library) {
        auto project = Cppm::instance()->project();
        auto file_name = project.cmake_find_module + "/Find"+library.name+".cmake";
        std::ofstream file(file_name); file.is_open();
        file <<  cmake::make_find_library(library.name, library.cmake_var_name, library.build_type);
        file.close();
    }
    
    void install_thirdparty(Thirdparty& library) {
        switch(hash(library.repo.type.c_str()))
        {
        case hash("git"): 
            git(library.repo.url);
            break;
        case hash("file"):
            tar(library.repo.url);
            break;
        case hash("link"):
            tar(wget(library.repo.url));
            break;
        }
        
    }
    
    void git(std::string& url) {
        auto project = Cppm::instance()->project();
        auto command = "    cd " + project.thirdparty 
                     + " && git clone " + url;
        system(command.c_str());
    }
    
    void svn(std::string& url) {
    }
    
    std::string wget(std::string& url) {
        return ("");
    } 
    
    void tar(std::string file) {
        
    }
    
}