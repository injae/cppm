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
    
     
    std::vector<std::string> cmake_find_package_list() {
        std::string cmake_path = "/usr/local/share/";
        auto cmake_dir = find_regex_files(cmake_path, boost::regex("cmake.*"));
        auto module_path = cmake_path + "/" + cmake_dir[0] + "/Modules/";
        auto find_package_files = find_regex_files(module_path, boost::regex("Find.*cmake"));
        return find_package_files;
    }
    
    bool has_find_package(Thirdparty thirdparty) {
        for(auto& package : cmake_find_package_list()) {
            boost::regex filter("Find(?i)(" + thirdparty.name + ")\\.cmake");
            boost::smatch what;
            if(!boost::regex_match(package, what, filter)) continue;
            return true;
        }
        return false;
    }
    
    std::vector<Thirdparty> cmake_package_config_list() {
        std::vector<Thirdparty> packages;
        
        std::string cmake_path = "/usr/local/lib/cmake";
        auto library_dirs = find_regex_files(cmake_path, boost::regex(".*"));
        for(auto dir : library_dirs) {
           Thirdparty package;
           package.name = dir;
           
           auto package_path = cmake_path + "/" + package.name;
           std::string config_file = package_path + package.name + "-config.cmake";
           parse_package_config_file(package, config_file);
           std::cout << package.cmake_var_name << "\n";
           
           packages.emplace_back(std::move(package));
        }
        
        return packages; 
    }
    
    Thirdparty parse_package_config_file(Thirdparty& thirdparty, std::string& file) {
        char fdata[512];
        std::ifstream ifs(file);
        if(ifs.is_open()) {
            while(!ifs.eof()) {
               memset(fdata, 0, 512);
               ifs.getline(fdata, 512);
               boost::smatch what;
               boost::regex filter( "(\\w)_INCLUDE_DIR");
               
               if(!boost::regex_match(std::string(fdata), what, filter)) continue;
               std::cout << fdata << " " << what[0] << std::endl;
            }
        }
        
        ifs.close(); 
        return thirdparty;
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
        
        auto project = Cppm::instance()->project();
        auto file_name = project.cmake_find_module + "/Find"+library.name+".cmake";
        std::ofstream file(file_name); file.is_open();
        file <<  cmake::make_find_library(library.name, library.build_type);
        file.close();
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